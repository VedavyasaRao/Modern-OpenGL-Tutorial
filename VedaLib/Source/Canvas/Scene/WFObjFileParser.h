#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <iterator>
#include <algorithm>
#include <vector>
#include <utility>
#include "..\..\Geometry\Utils\Texture\TextureUtil.h"

class WFObjFileParser 
{
public:
	typedef pair<string, pair<uint, uint>> facemattype;

	void clear()
	{
		objfilename="";
		mtlfilename="";

		vertices.clear();
		texturemap.clear();
		normals.clear();
		faces.clear();
		matlinfomap.clear();
		facematlst.clear();
	}

	bool  Parse(const string& objfilename, bool summary=false)
	{
		string temp;
		bool ret = false;
		HCURSOR hCursor = LoadCursor(NULL, IDC_WAIT);
		SetCursor(hCursor);

		this->summary = summary;
		this->objfilename = objfilename;

		//record_start_time(R"(C:\Github\Modern-OpenGL-Tutorial\timings.txt)");
		if (readfile(objfilename))
			parsegeometrydata();
		else 
			goto cleanup;

		temp = objfilename.substr(0, objfilename.find_last_of('\\')+1);
		mtlfilename = temp + mtlfilename;
		if (readfile(mtlfilename))
			parsematerialdata();
		else
			goto cleanup;
		ret = true;
	cleanup:
		//record_end_time(R"(C:\Github\Modern-OpenGL-Tutorial\timings.txt)");
		hCursor = LoadCursor(NULL, IDC_ARROW);
		SetCursor(hCursor);
		return ret;
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
		vec3 ambientclr{};
		vec3 diffuseclr{};
		vec3 specularclr{};
		vec3 emissiveclr{};
		float shininess{};
		TextureUtil::TexInfo diffusetxt;

	};


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
		auto texunit = 10;

		auto temp = objfilename.substr(0, objfilename.find_last_of('\\') + 1);

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
				matlinfomap[mtlname] = MaterialInfo();
			}
			else
			{
				if (matlinfomap.find(mtlname) == matlinfomap.end())
					continue;
				auto& mat = matlinfomap[mtlname];
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
				{
					string txflname;
					iss >> txflname;

					txflname = temp + txflname;
					mat.diffusetxt = TextureUtil::TexInfo(texunit++, txflname, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

				}
			}
		}
	}

	//https://paulbourke.net/dataformats/obj/
	void parsegeometrydata()
	{
		const string gemvtxstr = "v .*";
		const string gemtxtstr = "vt .*";
		const string gemnorstr = "vn .*";
		const string gemfacmtlstr = "(f |usemtl ).*";
		const string gemfacestr = "f .*";
		const string gemmtllibstr = "mtllib .*";
		const string gemusemtlstr = "usemtl";

		string dummy(100, 0);
		glm::vec3 v3;
		string mtlusename;
		string tempstr = (summary) ? gemfacestr : gemfacmtlstr;
		for (auto s : { gemmtllibstr, gemvtxstr, gemtxtstr, gemnorstr, tempstr })
		{

			regex pattern("^" + s);
			auto match_begin = sregex_iterator(text.begin(), text.end(), pattern);
			auto match_end = sregex_iterator();
			auto kount = std::distance(match_begin, match_end);
			if (s == gemvtxstr)
			{
				vertices.reserve(kount);
				if (summary)
					continue;
			}
			else if (s == gemtxtstr)
			{
				texturemap.reserve(kount);
				if (summary)
					continue;
			}
			else if (s == gemnorstr)
			{
				normals.reserve(kount);
				if (summary)
					continue;
			}
			else if (s == gemfacestr)
			{
				faces.reserve(kount);
				continue;
			}

			for (sregex_iterator i = match_begin; i != match_end; ++i)
			{
				istringstream iss{ i->str() };
				if (s == gemmtllibstr)
				{
					iss >> dummy >> mtlfilename;
				}
				else if (s != gemfacmtlstr)
				{
					iss >> dummy >> v3[0] >> v3[1] >> v3[2];
					if (s == gemvtxstr)
					{
						vertices.emplace_back(v3);
					}
					else if (s == gemtxtstr)
					{
						texturemap.emplace_back(glm::vec2{ v3 });
					}
					else if (s == gemnorstr)
					{
						normals.emplace_back(v3);
					}
				}
				else
				{
					iss >> dummy;
					if (dummy == gemusemtlstr)
					{
						iss >> dummy;
						if (!facematlst.empty())
						{
							facematlst.back().second.second = faces.size() - 1;
						}
						mtlusename = dummy;
						facemattype fm;
						fm.first = mtlusename;
						fm.second.first = faces.size();
						facematlst.push_back(fm);
					}
					else
					{
						for (auto i = 0; i < 3; ++i)
						{
							char item[10] = { 0 };
							iss >> dummy;
							if (iss)
							{
								i16vec3 vi3{ -1 };
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
								faces.emplace_back(vi3);
							}
						}
					}
				}
			}

			if (s == gemfacmtlstr)
			{
				facematlst.back().second.second = faces.size() - 1;
			}
		}
	}

	void record_start_time(string logfile)
	{
		ofstream ofs(logfile, ios_base::app);
		ofs << objfilename << endl;

		char  buf[1000];
		time(&stime);
		ctime_s(buf, 1000, &stime);
		ofs << buf;
	}

	void record_end_time(string logfile)
	{
		ofstream ofs(logfile, ios_base::app);
		ofs << objfilename << endl;

		char  buf[1000];
		time(&etime);
		ctime_s(buf, 1000, &etime);
		ofs << buf;
		ofs << (etime-stime) << endl << endl;
	}


public:
	string objfilename;
	string mtlfilename;

	vector <glm::vec3>		vertices;
	vector <glm::vec2>		texturemap;
	vector <glm::vec3>		normals;
	vector <glm::i16vec3>	faces;
	map <string, MaterialInfo>	matlinfomap;
	vector<facemattype>	facematlst;
	bool summary = false;
	time_t stime, etime;

private:
	string text;
};
