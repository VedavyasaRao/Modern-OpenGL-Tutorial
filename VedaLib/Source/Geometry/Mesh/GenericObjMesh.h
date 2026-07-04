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
	//
	GenericObjMesh(const string& objfilename, const string& mtlfilename)
	{
		if (readfile(objfilename))
			parsegeometrydata();

		if (readfile(mtlfilename))
			parsematerialdata();
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
				if (vtx[1] != -1)
					vaoutl.AddTexCord(texturemap[vtx[1]]);
			}

			if (att & VAOUtil::NOR)
			{
				if (vtx[2] != -1)
					vaoutl.AddNormal(normals[vtx[2]]);
			}
		}
		return faces.size();
	}

	inline uint vertex_count() const
	{
		return vertices.size();
	}

	inline uint texture_count() const
	{
		return texturemap.size();
	}

	inline uint normals_count() const
	{
		return normals.size();
	}

	struct MaterialInfo
	{
		std::pair<int,int> range;
		vec3 ambientclr{};
		vec3 diffuseclr{};
		vec3 specularclr{};
		vec3 emissiveclr{};
		float shininess{};
		string diffusetxtfilename;
	};

public:
	string mtlfilename;

	vector <glm::vec3>		vertices;
	vector <glm::vec2>		texturemap;
	vector <glm::vec3>		normals;
	vector <glm::i16vec3>	faces;
	map <string, MaterialInfo>	matinfomap;


private:
	bool readfile(const string& filename)
	{
		ifstream  fi(filename.c_str());
		if (!fi)
			return false;

		fi.seekg(0, ios::end);
		auto len = fi.tellg();
		text.assign((unsigned int)(len + (decltype(len))10), 0);

		fi.seekg(0, ios_base::beg);
		fi.read((char*)text.data(), len);
		fi.close();
		return true;
	}

	//https://paulbourke.net/dataformats/mtl/
	void parsematerialdata()
	{
		const string mtlstr = "(newmtl |Ns |Ka |Kd |Ks |Ke |map_Kd ).*";
		const string newmtlstr = "newmtl";
		const string ambstr = "Ka";
		const string difstr = "Kd";
		const string spcstr = "Ks";
		const string emistr = "Ke";
		const string shnstr = "Ns";
		const string txttr = "map_Kd";

		string mtlname;
		string dummy(100, 0);

		regex pattern("^" + mtlstr);
		auto match_begin = sregex_iterator(text.begin(), text.end(), pattern);
		auto match_end = sregex_iterator();

		for (sregex_iterator i = match_begin; i != match_end; ++i)
		{
			istringstream iss{ i->str() };
			iss >> dummy;
			if (dummy == newmtlstr)
			{
				iss >> mtlname;
			}
			else
			{
				if (matinfomap.find(mtlname) == matinfomap.end())
					continue;
				auto& mat = matinfomap[mtlname];
				if (dummy == ambstr)
					iss >> mat.ambientclr[0] >> mat.ambientclr[1] >> mat.ambientclr[2];
				else if (dummy == difstr)
					iss >> mat.diffuseclr[0] >> mat.diffuseclr[1] >> mat.diffuseclr[2];
				else if (dummy == spcstr)
					iss >> mat.specularclr[0] >> mat.specularclr[1] >> mat.specularclr[2];
				else if (dummy == emistr)
					iss >> mat.emissiveclr[0] >> mat.emissiveclr[1] >> mat.emissiveclr[2];
				else if (dummy == shnstr)
					iss >> mat.shininess;
				else if (dummy == txttr)
					iss >> mat.diffusetxtfilename;
			}
		}
	}

	//https://paulbourke.net/dataformats/obj/
	void parsegeometrydata()
	{
		const string gemvtxstr = "v .*";
		const string gemtxtstr = "vt .*";
		const string gemnorstr = "vn .*";
		const string gemfacstr = "(f |usemtl ).*";
		const string gemmtllibstr = "mtllib .*";
		const string gemusemtlstr = "usemtl";

		string dummy(100, 0);
		glm::vec3 v3;
		string mtlusename;
		for (auto s : { gemmtllibstr, gemvtxstr, gemtxtstr, gemnorstr, gemfacstr})
		{
			regex pattern("^" + s);
			auto match_begin = sregex_iterator(text.begin(), text.end(), pattern);
			auto match_end = sregex_iterator();
			auto kount = std::distance(match_begin, match_end);
			if (s == gemvtxstr)
			{
				vertices.reserve(kount);
			}
			else if (s == gemtxtstr)
			{
				texturemap.reserve(kount);
			}
			else if (s == gemnorstr)
			{
				normals.reserve(kount);
			}
			else if (s == gemfacstr)
			{
				faces.reserve(kount);
			}

			for (sregex_iterator i = match_begin; i != match_end; ++i)
			{
				istringstream iss{ i->str() };
				if (s == gemmtllibstr)
				{
					iss >> dummy;
					iss >> mtlfilename;
				}
				else if (s != gemfacstr)
				{
					iss >> dummy >> v3[0] >> v3[1] >> v3[2];
					if (s == gemvtxstr)
					{
						vertices.push_back(v3);
					}
					else if (s == gemtxtstr)
					{
						texturemap.push_back(glm::vec2{ v3 });
					}
					else if (s == gemnorstr)
					{
						normals.push_back(v3);
					}
				}
				else
				{
					iss >> dummy;
					if (dummy == gemusemtlstr)
					{
						iss >> dummy;
						if (!matinfomap.empty())
						{
							auto &matinf = matinfomap[mtlusename];
							matinf.range.second = faces.size() - 1;
						}
						mtlusename = dummy;
						MaterialInfo mat;
						mat.range.first = faces.size();
						matinfomap[mtlusename] = mat;
					}
					else
					{
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
			
			if (s == gemfacstr)
			{
				matinfomap[mtlusename].range.second = faces.size() - 1;
			}
		}
	}


private:
	string text;
};