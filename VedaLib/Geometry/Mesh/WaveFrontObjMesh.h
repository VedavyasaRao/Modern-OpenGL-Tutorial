#pragma once
#include "Base\GeometryMeshIndexed.h"
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <iterator>
#include <vector>

//Implements IGeometryMesh for Cube object
class WFObjMesh :public IGeometryMesh
{
public:
	//
	WFObjMesh(const string& filename)
	{
		ifstream  fi(filename.c_str());
		if (!fi)
			return;

		fi.seekg(0, ios::end);
		auto len = fi.tellg();
		text.assign((unsigned int)(len + (decltype(len))10), 0);

		fi.seekg(0, ios_base::beg);
		fi.read((char*)text.data(), len);
		fi.close();

		parsegeometrydata();
	}


	//Generate VBO vertex data in non Indexed Mode 
	int GenerateVerticesData(int att, VAOUtil& vaoutl)
	{
		for (unsigned short i = 0; i < faces.size(); ++i)
		{
			auto vtx = faces[i];
			if (att & VAOUtil::POS)
			{
				vaoutl.AddPosition(vertices[vtx[0]]);
			}
			if (att & VAOUtil::TEX)
			{
				vaoutl.AddTexCord(texturemap[vtx[1]]);
			}
			if (att & VAOUtil::NOR)
			{
				vaoutl.AddNormal(normals[vtx[2]]);
			}
			if (att & VAOUtil::CLR)
			{
			}
		}
		return faces.size();
	}

private:
	void parsegeometrydata()
	{
		string dummy(100,0);
		glm::vec3 v3;
		for (auto s:{ vtxstr, txtstr, norstr, facestr })
		{
			regex pattern("^" + s);
			auto match_begin = sregex_iterator(text.begin(), text.end(), pattern);
			auto match_end = sregex_iterator();

			for (sregex_iterator i = match_begin; i != match_end; ++i)
			{
				istringstream iss{ i->str() };
				if (s != facestr)
				{
					iss >> dummy >> v3[0] >> v3[1] >> v3[2];
					if (s == vtxstr)
					{
						vertices.push_back(v3);
					}
					else if (s == txtstr)
					{
						texturemap.push_back(glm::vec2{ v3 });
					}
					else if (s == norstr)
					{
						normals.push_back(v3);
					}
				}
				else
				{
					iss >> dummy;
					for (auto i = 0; i < 3; ++i)
					{
						char item[10] = { 0 };
						i16vec3 vi3{ -1 };
						iss >> dummy;
						istringstream fss{ dummy };
						for (auto j = 0; (j < 3 && fss); ++j)
						{
							fss.get(item, 10, '/');
							if (fss)
								vi3[j] = stoi(item) - 1;
							else
								fss.clear();
							fss.ignore();
						}
						faces.push_back(vi3);
					}
				}
			}
		}
	}

private:
	const string vtxstr = "v .*";
	const string txtstr = "vt .*";
	const string norstr = "vn .*";
	const string facestr = "f .*";
	string text;

	vector <glm::vec3>		vertices;

	/*
	0,1          1,1
	   ___________
	  |           |
	  |           |
	  |           |
	  |           |
	  |___________|

	0,0           1,0


	*/

	vector <glm::vec2>		texturemap;
	vector <glm::vec3>		normals;
	vector <glm::i16vec3>	faces;
};