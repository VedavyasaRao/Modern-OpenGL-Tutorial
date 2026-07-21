#pragma once
#include "..\Texture\TextureUtil.h"
struct GenericMaterialInfo
{
	string name;
	vec3 ambientclr{};
	vec3 diffuseclr{};
	vec3 specularclr{};
	vec3 emissiveclr{};
	float shininess{};
	TextureUtil::TexInfo diffusetxt;
};

class GenericObjInfo
{
public:
	struct Light;

	GenericObjInfo() = default;
	GenericObjInfo(vec3 viewerpos, const Light& light)
		:viewerpos(viewerpos), light(light)
	{
	}

	struct Light
	{
		float ambientCoefficient;
		vec3 Position;
		vec3 Color;
	};

	vec3 viewerpos;
	Light light;

	friend class GenericObj;
};

struct GenericLightSourceInfo
{
	enum LightSourceType { Basic = 0, Directional = 1, Point = 2, Spot = 3 };
	LightSourceType src;

	string name;
	vec3 position;
	vec3 direction;

	float attconstant;
	float attlinear;
	float attquadratic;

	float spotlightinner;
	float spotlightouter;

	float ambientCoefficient;
	float diffuseCoefficient;
	float specularCoefficient;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	vec3 viewerPosition;
};


typedef vector<vector<ivec3>>				Meshlist;
typedef vector<GenericMaterialInfo>			Matlinfolist;
typedef vector<GenericLightSourceInfo>		Lightinfolist;
typedef map<uint, uint>						MeshMatlmap;

class GenericParser
{
public:
	virtual void  clear()
	{
		meshlst.clear();
		matlinfolst.clear();
		lightlst.clear();
		meshmatlmap.clear();
		objfilename.clear();
	}

	virtual bool Parse(const string& objfilename) = 0;
	virtual int GenerateVerticesData(uint idx, int att, vector<vec3>& posvec, vector<vec2>& uvvec, vector<vec3>& norvec) = 0;

	virtual bool hastexture(uint idx) = 0;
	virtual bool hasnormal(uint idx) = 0;
	virtual uint vertex_count(uint idx)
	{
		return meshlst[idx].size();
	}


	long long parsetime()
	{
		return etime - stime;
	}

public:
	Meshlist			meshlst;
	Matlinfolist		matlinfolst;
	Lightinfolist		lightlst;
	MeshMatlmap			meshmatlmap;
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
