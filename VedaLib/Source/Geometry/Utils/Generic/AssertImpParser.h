#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <iterator>
#include <algorithm>
#include <vector>
#include <set>
#include <utility>
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//Implements IGeometryMesh for Cube object
class AssertImpParser :public GenericParser
{
public:

	void clear()
	{
		delete pimporter;
		pimporter = nullptr;
		directory.clear();
		aimatlidxmatlinfomap.clear();
		GenericParser::clear();
	}

	bool Parse(const string& objfilename)
	{
		if (pimporter)
			return false;
		startparse(objfilename);
		pimporter = new Assimp::Importer;
		this->objfilename = objfilename;
		pimporter->SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);
		scene = pimporter->ReadFile(this->objfilename,  aiProcess_Triangulate | aiProcess_SortByPType );
		if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
			return false;

		load_model(); 
		endparse();

		return true;
	}

	int GenerateVerticesData(uint idx, int att, vector<vec3>& posvec, vector<vec2>& uvvec, vector<vec3>& norvec)
	{
		auto& aimesh = getmesh(idx);
		uint count = aimesh.data.size();

		auto& objmesh = scene->mMeshes[idx];
		auto& objvertices = objmesh->mVertices;
		auto& objuvmap = objmesh->mTextureCoords;
		auto& objnormals = objmesh->mNormals;
		
		for (auto i = 0u; i < count; ++i)
		{
			auto& vtx = aimesh.data[i];
			if (att & VAOUtil::POS)
			{
				auto& v = objvertices[vtx[0]];
				posvec.emplace_back(vec3{ v.x,v.y,v.z });
			}

			if (att & VAOUtil::TEX)
			{
				auto& t = objuvmap[0][vtx[1]];
				uvvec.emplace_back(vec2{ t.x,t.y });
			}

			if (att & VAOUtil::NOR)
			{
				auto& n = objnormals[vtx[2]];
				norvec.emplace_back(vec3{ n.x,n.y,n.z });
			}
		}
		return count;
	}

	bool hastexture(uint idx)
	{
		auto* mat = &getmat4mesh(idx);
		return (scene->mMeshes[idx]->HasTextureCoords(0)) && (!matltextmap[mat].empty());
	}

	bool hasnormal(uint idx)
	{
		return (scene->mMeshes[idx]->HasNormals());
	}

private:
	void processNode(aiNode* node)
	{
		for (auto i = 0u; i < node->mNumMeshes; i++)
		{
			processamesh(node->mMeshes[i]);

		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i]);
		}
	}

	void processamesh(int idx)
	{
		if (aimeshset.find(idx) != aimeshset.end())
			return;
		
		MeshInfo	mesh(idx);
		auto aimesh = scene->mMeshes[idx];
		mesh.data.reserve(aimesh->mNumVertices);
		bool hastexture = aimesh->HasTextureCoords(0);
		bool hasnormals = aimesh->HasNormals();
		for (auto i = 0u; i < aimesh->mNumFaces; ++i)
		{
			auto& aiface = aimesh->mFaces[i];
			for (auto j = 0u; j < aiface.mNumIndices; ++j)
			{
				auto idx = aiface.mIndices[j];
				auto v = idx;
				auto t = (hastexture)?idx:-1;
				auto n = (hasnormals) ? idx : -1;
				mesh.data.emplace_back(ivec3{v,t,n});
			}
		}
		meshlst.push_back(move(mesh));

		auto matlidx = aimesh->mMaterialIndex;
		if (aimatlidxmatlinfomap.find(matlidx) == aimatlidxmatlinfomap.end())
		{
			processamaterial(matlidx);
			aimatlidxmatlinfomap[matlidx]= &matlinfolst.back();
		}
		meshmatlmap[&meshlst.back()] = aimatlidxmatlinfomap[matlidx];
	}

	void processamaterial(int idx)
	{
		matlinfolst.push_back(MaterialInfo());
		MaterialInfo	&mat = matlinfolst.back();

		auto aimat = scene->mMaterials[idx];
		mat.name = aimat->GetName().C_Str();
		auto texcount = aimat->GetTextureCount(aiTextureType_DIFFUSE);
		for (auto i = 0u; i < texcount; ++i)
		{
			aiString str;
			aiTextureMapMode mm{};
			aimat->GetTexture(aiTextureType_DIFFUSE, i, &str, nullptr, nullptr,nullptr,nullptr,&mm);
			auto df = directory + str.C_Str();
			if (diffusetxtmap.find(df) == diffusetxtmap.end())
			{
				auto ti = TextureInfo(df);
				if (mm == aiTextureMapMode_Wrap)
				{
					ti.swrap = GL_REPEAT;
					ti.twrap = GL_REPEAT;
				}
				else if (mm == aiTextureMapMode_Clamp)
				{
					ti.swrap = GL_CLAMP_TO_EDGE;
					ti.twrap = GL_CLAMP_TO_EDGE;
				}
				else if (mm == aiTextureMapMode_Mirror)
				{
					ti.swrap = GL_MIRRORED_REPEAT;
					ti.twrap = GL_MIRRORED_REPEAT;
				}
				else if (mm == aiTextureMapMode_Decal)
				{
					ti.swrap = GL_CLAMP_TO_BORDER;
					ti.twrap = GL_CLAMP_TO_BORDER;
				}
				diffusetxtmap[df] = ti;
			}
			matltextmap[&mat] = df;
		}
		if (texcount == 0)
			matltextmap[&mat] = "";
		
		aiColor4D color;
		if (AI_SUCCESS == aiGetMaterialColor(aimat, AI_MATKEY_COLOR_AMBIENT, &color))
			mat.ambientColor = vec3(color.r, color.g, color.b);
		if (AI_SUCCESS == aiGetMaterialColor(aimat, AI_MATKEY_COLOR_DIFFUSE, &color))
			mat.diffuseColor = vec3(color.r, color.g, color.b);
		if (AI_SUCCESS == aiGetMaterialColor(aimat, AI_MATKEY_COLOR_SPECULAR, &color))
			mat.specularColor = vec3(color.r, color.g, color.b);
		aiGetMaterialFloat(aimat, AI_MATKEY_SHININESS, &mat.Shininess);
	}

	void processalight(int idx)
	{
		auto ailight = scene->mLights[idx];
		LightSrcInfo lightsrc;
		lightsrc.name = ailight->mName.C_Str();
		lightsrc.position = vec3{ ailight->mPosition.x,ailight->mPosition.y,ailight->mPosition.z };
		lightsrc.direction = vec3{ ailight->mDirection.x, ailight->mDirection.y,ailight->mDirection.z };
		
		lightsrc.attconstant = ailight->mAttenuationConstant;
		lightsrc.attlinear = ailight->mAttenuationLinear;
		lightsrc.attquadratic = ailight->mAttenuationQuadratic;

		lightsrc.spotlightinner = ailight->mAngleInnerCone;
		lightsrc.spotlightouter = ailight->mAngleOuterCone;

		lightsrc.ambientColor = vec3(ailight->mColorAmbient.r, ailight->mColorAmbient.g, ailight->mColorAmbient.b);
		lightsrc.diffuseColor = vec3(ailight->mColorDiffuse.r, ailight->mColorDiffuse.g, ailight->mColorDiffuse.b);
		lightsrc.specularColor = vec3(ailight->mColorSpecular.r, ailight->mColorSpecular.g, ailight->mColorSpecular.b);
		lightlst.push_back(move(lightsrc));
	}

	void load_model()
	{
		directory = objfilename.substr(0, objfilename.find_last_of('\\') + 1);
		processNode(scene->mRootNode);
	}

public:
	const aiScene*	scene = nullptr;

	Assimp::Importer *pimporter = nullptr;
	string directory;
	map<uint, MaterialInfo*> aimatlidxmatlinfomap;
	set<uint> aimeshset;
};
