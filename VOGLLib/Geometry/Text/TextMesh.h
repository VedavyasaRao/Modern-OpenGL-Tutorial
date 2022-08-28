#pragma once
#include "..\GeometryMesh.h"

class TextMesh:public IGeometryMesh
{
public:
	int GenerateVerticesData(int att, VAOUtil& vaoutl)
	{
		for (unsigned short  i = 0; i < vertices.size(); ++i)
		{
			if (att & VAOUtil::POS)
			{
				vaoutl.AddPosition(vertices[i]);
			}

			if (att & VAOUtil::TEX)
			{
				vaoutl.AddTexCord(texturemap[i]);
			}

		}
		return vertices.size();
	}


private:
	/*
	0___________3  
 	|           |
	|           |
	|           |
	|___________|
	1           2    
	*/

	std::vector < glm::vec3>  vertices =
	{
		{ -0.5, +0.5, 0.0 },
		{ -0.5, -0.5, 0.0 },
		{ +0.5,-0.5,  0.0 },

		{ +0.5,-0.5,  0.0 },
		{ +0.5,+0.5,  0.0 },
		{ -0.5, +0.5, 0.0 },
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

	glm::vec2 texturemap[6] =
	{
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
	};

};


