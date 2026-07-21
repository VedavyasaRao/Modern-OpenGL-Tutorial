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
class AssImpParser :public GenericParser
{
public:

	void clear()
	{
		delete pimporter;
		pimporter = nullptr;
		directory.clear();
		matlmeshmap.clear();
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
		auto& aimesh = meshlst[idx];
		uint count = aimesh.size();

		auto& objmesh = scene->mMeshes[idx];
		auto& objvertices = objmesh->mVertices;
		auto& objuvmap = objmesh->mTextureCoords;
		auto& objnormals = objmesh->mNormals;
		
		for (auto i = 0u; i < count; ++i)
		{
			auto& vtx = aimesh[i];
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
		return (scene->mMeshes[idx]->HasTextureCoords(0)) && (!matlinfolst[meshmatlmap[idx]].diffusetxt.filename.empty());
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
		if (meshmatlmap.find(idx) != meshmatlmap.end())
			return;
		
		vector<ivec3>	mesh;

		auto aimesh = scene->mMeshes[idx];
		mesh.reserve(aimesh->mNumVertices);
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
				mesh.emplace_back(ivec3{v,t,n});
			}
		}
		meshlst.push_back(move(mesh));
		auto matlidx = aimesh->mMaterialIndex;
		if (matlmeshmap.find(matlidx) == matlmeshmap.end())
		{
			processamaterial(matlidx);
			matlmeshmap.insert(make_pair(matlidx, matlinfolst.size() - 1));
		}
		meshmatlmap.insert(make_pair(meshlst.size()-1, matlmeshmap[matlidx]));
	}

	void processamaterial(int idx)
	{
		GenericMaterialInfo	mat;
		auto aimat = scene->mMaterials[idx];
		auto texcount = aimat->GetTextureCount(aiTextureType_DIFFUSE);
		for (auto i = 0u; i < texcount; ++i)
		{
			aiString str;
			aiTextureMapMode mm{};
			aimat->GetTexture(aiTextureType_DIFFUSE, i, &str, nullptr, nullptr,nullptr,nullptr,&mm);
			auto df = directory + str.C_Str();
			auto ti = TextureUtil::TexInfo(i + 10, df);
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
			mat.name = aimat->GetName().C_Str();
			mat.diffusetxt = ti;
		}

		aiColor4D color;
		if (AI_SUCCESS == aiGetMaterialColor(aimat, AI_MATKEY_COLOR_AMBIENT, &color))
			mat.ambientclr = vec3(color.r, color.g, color.b);
		if (AI_SUCCESS == aiGetMaterialColor(aimat, AI_MATKEY_COLOR_DIFFUSE, &color))
			mat.diffuseclr = vec3(color.r, color.g, color.b);
		if (AI_SUCCESS == aiGetMaterialColor(aimat, AI_MATKEY_COLOR_SPECULAR, &color))
			mat.specularclr = vec3(color.r, color.g, color.b);
		if (AI_SUCCESS == aiGetMaterialColor(aimat, AI_MATKEY_COLOR_EMISSIVE, &color))
			mat.emissiveclr = vec3(color.r, color.g, color.b);
		aiGetMaterialFloat(aimat, AI_MATKEY_SHININESS, &mat.shininess);
		matlinfolst.push_back(move(mat));
	}

	void processalight(int idx)
	{
		auto ailight = scene->mLights[idx];
		GenericLightSourceInfo lightsrc;
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
	map<uint, uint> matlmeshmap;
};
