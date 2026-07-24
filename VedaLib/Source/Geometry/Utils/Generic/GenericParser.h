#pragma once
#include "..\Texture\TextureUtil.h"
#include "..\Lighting\LightingUtil.h"

struct Mesh
{
	Mesh(uint id) :id(id) {}
	vector<ivec3> data;
	uint id;
};

typedef list<Mesh>						Meshlist;
typedef list<MaterialInfo>				Matlinfolist;
typedef map<string,TextureInfo>			Txtureinfomap;
typedef list<LightSrcInfo>				Lightinfolist;
typedef map<Mesh*, MaterialInfo*>		MeshMatlmap;
typedef map<MaterialInfo*, string>		MatlTxturemap;

class GenericParser
{
public:
	virtual void  clear()
	{
		meshlst.clear();
		matlinfolst.clear();
		lightlst.clear();
		meshmatlmap.clear();
		diffusetxtmap.clear();
		matltextmap.clear();
		objfilename.clear();
	}

	Mesh& getmesh(uint idx)
	{
		auto itr = meshlst.begin();
		advance(itr, idx);
		return *itr;
	}

	MaterialInfo& getmat4mesh(uint idx) 
	{
		return *meshmatlmap[&getmesh(idx)];
	}

	virtual bool Parse(const string& objfilename) = 0;
	virtual int GenerateVerticesData(uint idx, int att, vector<vec3>& posvec, vector<vec2>& uvvec, vector<vec3>& norvec) = 0;

	virtual bool hastexture(uint idx) = 0;
	virtual bool hasnormal(uint idx) = 0;
	virtual uint vertex_count(uint idx)
	{
		return getmesh(idx).data.size();
	}


	long long parsetime()
	{
		return etime - stime;
	}

public:
	Meshlist			meshlst;
	Matlinfolist		matlinfolst;
	Lightinfolist		lightlst;
	Txtureinfomap		diffusetxtmap;
	MeshMatlmap			meshmatlmap;
	MatlTxturemap		matltextmap;
	string				objfilename;

protected:
	void startparse(const string& objfilename)
	{
		hCursor = LoadCursor(NULL, IDC_WAIT);
		SetCursor(hCursor);
		this->objfilename = objfilename;
		time(&stime);
	}

	void endparse()
	{
		hCursor = LoadCursor(NULL, IDC_ARROW);
		SetCursor(hCursor);
		time(&etime);

	}


private:
	time_t stime, etime;
	HCURSOR hCursor;
};
