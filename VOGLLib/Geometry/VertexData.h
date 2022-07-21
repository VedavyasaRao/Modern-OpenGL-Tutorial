#pragma once
//This class holds buffers for a VBO or EBO.
//It also binds VBO to its location in the shader. 
//It also binds EBO
template <class T>
class VertexData
{
public:
	VertexData(GLuint valtype, GLuint valsize)
	{
		id = 0;
		this->valtype = valtype;
		this->valsize = valsize;
	}

	void add(const T& p)
	{
		data.push_back(p);
	}

	int itemsize()
	{
		return sizeof T;
	}

	int datasize()
	{
		return data.size()*itemsize();
	}

	//bind EBO
	void bindEBO()
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, datasize(), data.data(), GL_STATIC_DRAW);
	}

	//bind VBO and enable the vertex
	void bindEnableVBO(GLint location)
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, datasize(), data.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(location, valsize, valtype, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(location);
	}
	//cleanup
	void cleanup()
	{
		glDeleteBuffers(1, &id);
	}

private:
	GLuint id;
	vector<T> data;
	GLuint valtype;
	GLuint valsize;

	friend class VAOUtil;
};
