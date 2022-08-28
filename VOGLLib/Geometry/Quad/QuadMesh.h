#pragma once
#include "..\GeometryMesh.h"
//Implements IGeometryMesh for Cube object
class QuadMesh :public IGeometryMesh
{
public:
	//Generate VBO vertex data in non Indexed Mode for all except IDX
	int GenerateVerticesData(int att, VAOUtil& vaoutl)
	{
		
		for (uint i = 0; i < vertices.size(); i+=4)
		{
			vaoutl.AddTexCord(texturemap[0]);
			vaoutl.AddPosition(vertices[i]);

			vaoutl.AddTexCord(texturemap[1]);
			vaoutl.AddPosition(vertices[i + 1]);

			vaoutl.AddTexCord(texturemap[2]);
			vaoutl.AddPosition(vertices[i + 2]);

			vaoutl.AddTexCord(texturemap[2]);
			vaoutl.AddPosition(vertices[i + 2]);

			vaoutl.AddTexCord(texturemap[3]);
			vaoutl.AddPosition(vertices[i + 3]);

			vaoutl.AddTexCord(texturemap[0]);
			vaoutl.AddPosition(vertices[i]);
		}

		return vertices.size() + (vertices.size()/2);
	}

	//override texture maps
	void updateTextureMap(float texsize)
	{
		this->texturemap = 
		{
			{ 0.0f, 0.0f },
			{ 0.0f, texsize },
			{ texsize, texsize },
			{ texsize, 0.0f },
		};
	}

	//override vertices
	void updateVertices(const std::vector < glm::vec3>& vertices)
	{
		this->vertices = vertices;
	}

private:
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

	std::vector < glm::vec3> vertices =
	{
	};

	std::vector < glm::vec2> texturemap =
	{
	};
};