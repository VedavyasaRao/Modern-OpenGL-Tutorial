#pragma once
class TextureInfo
{
public:
	TextureInfo(GLushort texunit = 0,
		const string& filename = "",
		uint swrap = GL_REPEAT,
		uint twrap = GL_REPEAT,
		uint minfilter = GL_LINEAR,
		uint magfilter = GL_LINEAR
	)
	{
		this->texunit = texunit;
		this->filename = filename;
		this->swrap = swrap;
		this->twrap = twrap;
		this->minfilter = minfilter;
		this->magfilter = magfilter;
	}

	void updatefilename(const string& sfilename)
	{
		this->filename = sfilename;
	}


public:
	string		filename;
	GLushort	texunit;
	GLuint		textureID;
	uint		swrap, twrap;
	uint		minfilter, magfilter;
};

class TextureUtil
{
public:
	//Load textures  for 3D Objects
	virtual void LoadTexture()
	{
		//activate the texture first
		glActiveTexture(GL_TEXTURE0 + texinfo.texunit);
		glGenTextures(1, &texinfo.textureID);
		glBindTexture(GL_TEXTURE_2D, texinfo.textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texinfo.swrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texinfo.twrap);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texinfo.minfilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texinfo.magfilter);

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(1);
		unsigned char* data = stbi_load(texinfo.filename.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			GLenum format = 0;
			if (nrChannels == 4)
				format = GL_RGBA;
			else if (nrChannels == 3)
				format = GL_RGB;
			else if (nrChannels == 1)
				format = GL_DEPTH_COMPONENT;

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void LoadTextTexture(void *imagedata, int wd, int ht)
	{
		glActiveTexture(GL_TEXTURE0 + texinfo.texunit);
		glGenTextures(1, &texinfo.textureID);
		glBindTexture(GL_TEXTURE_2D, texinfo.textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wd, ht, 0, GL_BGRA, GL_UNSIGNED_BYTE, imagedata);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texinfo.swrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texinfo.twrap);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texinfo.minfilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texinfo.magfilter);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void LoadTextTextureImage(void *imagedata, int wd, int ht)
	{
		glActiveTexture(GL_TEXTURE0 + texinfo.texunit);
		glGenTextures(1, &texinfo.textureID);
		glBindTexture(GL_TEXTURE_2D, texinfo.textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, wd, ht, GL_BGRA, GL_UNSIGNED_BYTE, imagedata);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void MakeActive(GLuint uniloc)
	{
		glActiveTexture(GL_TEXTURE0 + texinfo.texunit);
		glBindTexture(GL_TEXTURE_2D, texinfo.textureID);
		glUniform1i(uniloc, texinfo.texunit);
	}

	void Init(TextureInfo ptexinfo)
	{
		this->texinfo = ptexinfo;
	}

	void Cleanup()
	{
		glDeleteTextures(1, &texinfo.textureID);
	}

public:
	TextureInfo	texinfo;

};
