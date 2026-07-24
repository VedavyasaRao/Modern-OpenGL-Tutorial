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
#include "..\Utils\Generic\WFObjParser.h"
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

private:
	int					idx;
	GenericParser*		 pobjparser = nullptr;
		
};