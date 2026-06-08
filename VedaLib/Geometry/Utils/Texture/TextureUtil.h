#pragma once
//Loads Textures
class TextureUtil
{
public:
	struct TexInfo;

	//Load textures  for 3D Objects
	virtual void LoadTexture()
	{
		//activate the texture first
		glActiveTexture(GL_TEXTURE0 + texinfo.texunit);

		//load the texture from the file, create MIPMAP and invert the image for mapping
		//texinfo.textureID = SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, 0, SOIL_FLAG_MIPMAPS  | SOIL_FLAG_INVERT_Y);
		texinfo.textureID = SOIL_load_OGL_texture(texinfo.filename.c_str(), texinfo.channels, 0, texinfo.flags);

		//bind the texture
		glBindTexture(GL_TEXTURE_2D, texinfo.textureID);

		//wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texinfo.swrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texinfo.twrap);

		//filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texinfo.minfilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texinfo.magfilter);

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

	void Init(TexInfo texinfo)
	{
		this->texinfo = texinfo;
	}

	void Cleanup()
	{
		glDeleteTextures(1, &texinfo.textureID);
	}

public:
	struct TexInfo
	{
		GLushort	texunit;

		GLuint		textureID;
		string		filename;
		uint		channels;
		uint		flags;
		uint		swrap, twrap;
		uint		minfilter, magfilter;
	}texinfo;

};
