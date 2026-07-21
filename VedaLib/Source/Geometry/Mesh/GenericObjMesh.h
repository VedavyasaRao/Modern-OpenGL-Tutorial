#pragma once
#include "Base\GeometryMeshIndexed.h"
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <iterator>
#include <algorithm>
#include <vector>
#include <utility>
#include "..\Utils\Generic\WFObjFileParser.h"
#include "..\Utils\Generic\AssImpParser.h"

//Implements IGeometryMesh for Cube object
class GenericObjMesh :public IGeometryMesh
{
public:

	GenericObjMesh(int idx, GenericParser* pobjparser) : idx(idx), pobjparser(pobjparser)	{}

	//Generate VBO vertex data in non Indexed Mode 
	int GenerateVerticesData(int att, VAOUtil& vaoutl)
	{
		auto& posvec = vaoutl.positions.data;
		auto& uvvec = vaoutl.texcrds.data;
		auto& norvec = vaoutl.normals.data;
		vaoutl.Reserve(att, pobjparser->vertex_count(idx));

		return pobjparser->GenerateVerticesData(idx, att, posvec, uvvec, norvec);
	}


	//int GenerateVerticesDataAIObj(int att, VAOUtil& vaoutl)
	//{
	//	auto& aimesh = paibparser->meshmap[idx];

	//	auto& posvec = vaoutl.positions.data;
	//	auto& uvvec = vaoutl.texcrds.data;
	//	auto& norvec = vaoutl.normals.data;

	//	auto& objmesh = paibparser->scene->mMeshes[idx];
	//	auto& objvertices = objmesh->mVertices;
	//	auto& objuvmap = objmesh->mTextureCoords;
	//	auto& objnormals = objmesh->mNormals;
	//	uint count = aimesh.faces.size();

	//	vaoutl.Reserve(att, count);

	//	for (auto i = 0u; i < count; ++i)
	//	{
	//		auto& vtx = aimesh.faces[i];
	//		if (att & VAOUtil::POS)
	//		{
	//			auto& v = objvertices[vtx[0]];
	//			posvec.emplace_back(vec3{ v.x,v.y,v.z });
	//		}

	//		if (att & VAOUtil::TEX)
	//		{
	//			auto& t = objuvmap[0][vtx[1]];
	//			uvvec.emplace_back(vec2{ t.x,t.y });
	//		}

	//		if (att & VAOUtil::NOR)
	//		{
	//			auto& n = objnormals[vtx[2]];
	//			norvec.emplace_back(vec3{ n.x,n.y,n.z });
	//		}
	//	}
	//	return count;
	//}

private:
	int					idx;
	GenericParser*		 pobjparser = nullptr;
		
};