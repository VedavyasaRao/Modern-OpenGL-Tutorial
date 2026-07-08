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

//Implements IGeometryMesh for Cube object
class GenericObjMesh :public IGeometryMesh
{
public:

	GenericObjMesh(int idx, WFObjFileParser* pwfobparser) : idx(idx), pwfobparser(pwfobparser)	{}

	//Generate VBO vertex data in non Indexed Mode 
	int GenerateVerticesData(int att, VAOUtil& vaoutl)
	{
		auto& fm = pwfobparser->facematlst[idx];

		uint start = fm.second.first, last = fm.second.second, count = last - start + 1;

		vaoutl.Reserve(att, count);

		for (unsigned int i = start; i <= last; ++i)
		{
			auto& vtx = pwfobparser->faces[i];
			if (att & VAOUtil::POS)
			{
				vaoutl.AddPosition(pwfobparser->vertices[vtx[0]]);
			}

			if (att & VAOUtil::TEX)
			{
				vaoutl.AddTexCord(pwfobparser->texturemap[vtx[1]]);
			}

			if (att & VAOUtil::NOR)
			{
				vaoutl.AddNormal(pwfobparser->normals[vtx[2]]);
			}
		}
		return count;
	}


private:
	int					idx;
	WFObjFileParser		*pwfobparser;
		
};