#pragma once
#include "VertexData.h"
//This class holds buffers for VBO (Position, Color, Normal and Texture Coordinates) and EBO (indices).
//It also binds VBO to its location in the shader. 
//It also binds EBO
class VAOUtil
{
public:
	friend class GenericObjMesh;
		
	enum VtxAts { POS = 1, CLR = 2, NOR = 4, TEX = 8 };

	//vertex 3d coordinate in  x,y,z
	void AddPosition(const vec3& val)
	{
		positions.add(val);
	}

	//vertex clolor
	void AddColor(const vec3& val)
	{
		colors.add(val);
	}

	//vertex texture 2d coordinate u, v
	void AddTexCord(const vec2 val)
	{
		texcrds.add(val);
	}

	//normal vector of the vertex
	void AddNormal(const vec3& val)
	{
		normals.add(val);
	}

	//indices
	void AddIndex(const GLushort val)
	{
		indices.add(val);
	}

	//bind EBO
	void SetupEBO()
	{
		indices.bindEBO();
	}

	//create VAO and bind it first time
	//bind VBO and enable vertex
	void SetupVBO(GLuint location, int att)
	{
		if (id == 0)
			glGenVertexArrays(1, &id);

		glBindVertexArray(id);

		if (att & POS)
		{
			positions.bindEnableVBO(location);
		}

		if (att & CLR)
		{
			colors.bindEnableVBO(location);
		}

		if (att & NOR)
		{
			normals.bindEnableVBO(location);
		}

		if (att & TEX)
		{
			texcrds.bindEnableVBO(location);
		}
	}

	void dumpToTextFile()
	{

		std::ofstream outFile(R"(c:\\abc.txt)", std::ios_base::app);
		if (outFile.is_open())
		{
			outFile << "Positions" << endl << endl;;
			for (const auto& v : positions.data)
			{
				outFile << v.x << " " << v.y << " " << v.z << "\n";
			}
			outFile << endl << endl;;


			outFile << "texcrds" << endl << endl;;
			for (const auto& v : texcrds.data)
			{
				outFile << v.x << " " << v.y << "\n";
			}
			outFile << endl << endl;;

			outFile << "normals" << endl << endl;;
			for (const auto& v : normals.data)
			{
				outFile << v.x << " " << v.y << "\n";
			}
			outFile << endl << endl;;


			outFile.close();
		}
	}

	//release resources
	void Cleanup()
	{
		positions.cleanup();
		colors.cleanup();
		normals.cleanup();
		texcrds.cleanup();
		indices.cleanup();
		glDeleteVertexArrays(1, &id);
	}

	//bind VAO for drawing
	void bindVAO()
	{
		glBindVertexArray(id);
	}

	//unbind VAO after drawing
	void unbindVAO()
	{
		glBindVertexArray(0);
	}

	void Reserve(int att, unsigned int size)
	{
		if (att & VAOUtil::POS)
		{
			positions.reserve(size);
		}

		if (att & VAOUtil::CLR)
		{
			colors.reserve(size);
		}

		if (att & VAOUtil::NOR)
		{
			normals.reserve(size);
		}

		if (att & VAOUtil::TEX)
		{
			texcrds.reserve(size);
		}
	}

	void Reserveindices(unsigned int size)
	{
		indices.reserve(size);
	}

private:
	VertexData<vec3> positions{ GL_FLOAT,3 };
	VertexData<vec3> colors{ GL_FLOAT,3 };
	VertexData<vec3> normals{ GL_FLOAT,3 };
	VertexData<vec2> texcrds{ GL_FLOAT,2 };
	VertexData<GLushort>  indices{ GL_UNSIGNED_SHORT,2 };
	GLuint id;
};
/////////////////////VAOUtil///////////////////////////////////
