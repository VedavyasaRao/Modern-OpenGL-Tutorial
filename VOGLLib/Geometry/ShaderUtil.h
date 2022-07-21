#pragma once
//Loads shaders and compile. 
//Create Program object and attach complied shaders to it.
//Link to generate executables
class ShaderUtil
{
public:
	//create a shader, add the source from a string and compile it
	void LoadFromString(GLenum shadertype, const string& source)
	{
		GLuint shader = glCreateShader(shadertype);

		const char * ptmp = source.c_str();
		glShaderSource(shader, 1, &ptmp, NULL);

		GLint status;
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint infoLogLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
			GLchar *infoLog = new GLchar[infoLogLength];
			glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
			cerr << "Compile log: " << infoLog << endl;
			delete[] infoLog;
		}
		shadermap[shadertype] = shader;
	}

	//create a shader, add the source from a file  and compile it
	void LoadFromFile(GLenum whichShader, const string& filename)
	{
		ifstream fp;
		fp.open(filename.c_str(), ios_base::in);
		if (fp)
		{
			stringstream ShaderStream;
			ShaderStream << fp.rdbuf();
			fp.close();

			LoadFromString(whichShader, ShaderStream.str());
		}
		else
		{
			cerr << "Error loading shader: " << filename << endl;
		}
	}

	//create program object
	//attach shader and link to generate executables
	void CreateAndLinkProgram()
	{
		program = glCreateProgram();
		for (auto itr = shadermap.begin(); itr != shadermap.end(); ++itr)
		{
			glAttachShader(program, itr->second);
		}

		GLint status;
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint infoLogLength;

			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
			GLchar *infoLog = new GLchar[infoLogLength];
			glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
			cerr << "Link log: " << infoLog << endl;
			delete[] infoLog;
		}

		for (auto itr = shadermap.begin(); itr != shadermap.end(); ++itr)
		{
			glDeleteShader(itr->second);
		}
	}

	//use the program to draw
	void Use()
	{
		glUseProgram(program);
	}

	//unuse the program after draw
	void Release()
	{
		glUseProgram(0);
	}

	//get the attribute location from name
	GLuint GetAttributeLocation(const string& attribute)
	{
		if (attmap.find(attribute) == attmap.end())
		{
			attmap[attribute] = glGetAttribLocation(program, attribute.c_str());
		}
		return attmap[attribute];
	}

	//get the uniform location from name
	GLuint GetUniformLocation(const string& uniform)
	{
		if (unimap.find(uniform) == unimap.end())
		{
			unimap[uniform] = glGetUniformLocation(program, uniform.c_str());
		}
		return unimap[uniform];
	}

	//release resources
	void Cleanup()
	{
		glDeleteProgram(program);
	}

private:
	GLuint	program;
	map<GLenum, GLuint> shadermap;
	map<string, GLuint> attmap;
	map<string, GLuint> unimap;
	friend class VaoUtil;
};
