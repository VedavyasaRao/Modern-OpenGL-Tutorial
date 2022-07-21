#pragma once
#include "..\GeometryMesh.h"
//Implements IGeometryMesh for Cube object
class CubeMesh :public IGeometryMesh
{
public:
	//Generate VBO vertex data in non Indexed Mode for all except IDX
	int GenerateVerticesDataNonIndexed(int att, VAOUtil& vaoutl)
	{
		int clrs_sz = clrs.size();
		int normals_sz = normals.size();
		int texturemap_sz = texturemap.size();

		for (unsigned short i = 0; i < faces.size(); ++i)
		{
			if (att & VAOUtil::POS)
			{
				vaoutl.AddPosition(vertices[faces[i]]);
			}

			if (att & VAOUtil::CLR)
			{
				vaoutl.AddColor(clrs[i / clrs_sz]);
			}

			if (att & VAOUtil::NOR)
			{
				vaoutl.AddNormal(normals[i / normals_sz]);
			}

			if (att & VAOUtil::TEX)
			{
				vaoutl.AddTexCord(texturemap[i % texturemap_sz]);
			}

			if (att & VAOUtil::IDX)
			{
				vaoutl.AddIndex(faces[i]);
			}
		}
		return faces.size();
	}

	//Generate VBO vertex data in Indexed Mode for all 
	int GenerateVerticesDataIndexed(int att, VAOUtil& vaoutl)
	{
		int clrs_sz = clrs.size();

		for (unsigned short i = 0; i < vertices.size(); ++i)
		{
			if (att & VAOUtil::POS)
			{
				vaoutl.AddPosition(vertices[i]);
			}

			if (att & VAOUtil::CLR)
			{
				vaoutl.AddColor(clrs[i % clrs_sz]);
			}

		}
		return vertices.size();
	}


	//Generate VBO vertex data for all except IDX
	int GenerateVerticesData(bool isindexed, int att, VAOUtil& vaoutl)
	{
		return (isindexed) ? GenerateVerticesDataIndexed(att, vaoutl) : GenerateVerticesDataNonIndexed(att, vaoutl);
	}

	//Generate EBO data
	int GenerateIndicesData(VAOUtil& vaoutl)
	{
		return GenerateVerticesDataNonIndexed(VAOUtil::IDX, vaoutl);
	}

	//override default colors
	void updateColors(const std::vector < glm::vec3>& clrs)
	{
		this->clrs = clrs;
	}

	//override normals
	void updateNormals(const std::vector < glm::vec3>& normals)
	{
		this->normals = normals;
	}

	//override texture maps
	void updateTextureMap(const std::vector < glm::vec2>& texturemap)
	{
		this->texturemap = texturemap;
	}

private:

	/*
	 ___________
	|\0       3|\
	| \        | \
	|  \4______|__\
	|   |      |  7|
	|   |      |   |
	|1__|______|2  |
	 \  |       \  |
	  \ |        \ |
	   \|5_______6\|

	*/

	std::vector < glm::vec3>  vertices =
	{
		//0
		{ -0.5,+0.5,-0.5 },
		//1
		{ -0.5,-0.5,-0.5 },
		//2
		{ +0.5,-0.5,-0.5 },
		//3
		{ +0.5,+0.5,-0.5 },
		//4
		{ -0.5,+0.5,+0.5 },
		//5
		{ -0.5,-0.5,+0.5 },
		//6
		{ +0.5,-0.5,+0.5 },
		//7
		{ +0.5,+0.5,+0.5 },
	};

	std::vector < GLushort> faces =
	{
		//top
		0,4,7, 7,3,0,
		//left
		0,1,5, 5,4,0,
		//bottom
		1,5,6, 6,2,1,
		//right
		6,7,3, 3,2,6,
		//back
		0,1,2, 2,3,0,
		//front
		4,5,6, 6,7,4
	};

	std::vector < glm::vec3>  clrs =
	{
		//blue
		{ 0,0,1 },
		//green
		{ 0,1,0 },
		//aqua
		{ 0,1,1 },
		//red
		{ 1,0,0 },
		//fuchisa
		{ 1,0,1 },
		//yellow
		{ 1,1,0 },
	};

	std::vector < glm::vec3>  normals =
	{
		{  0.0,   1.0,   0.0 },
		{ -1.0,   0.0,   0.0 },
		{  0.0,  -1.0,   0.0 },
		{  1.0,   0.0,   0.0 },
		{  0.0,   0.0,  -1.0 },
		{  0.0,   0.0,   1.0 },
	};


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

	std::vector < glm::vec2> texturemap =
	{
		
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
	};
};