#pragma once
//Loads Textures
class TextureUtil
{
public:
	//Load textures  for 3D Objects
	virtual void LoadTexture(const string& filename)
	{
		//activate the texture first
		glActiveTexture(GL_TEXTURE0 + texunit);

		//load the texture from the file, create MIPMAP and invert the image for mapping
		textureID = SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, 0, SOIL_FLAG_MIPMAPS  | SOIL_FLAG_INVERT_Y);

		//bind the texture
		glBindTexture(GL_TEXTURE_2D, textureID);

		//wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void LoadTextTexture(void *imagedata, int htwd)
	{
		glActiveTexture(GL_TEXTURE0 + texunit);
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, htwd, htwd, 0, GL_BGRA, GL_UNSIGNED_BYTE, imagedata);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void LoadTextTextureImage(void *imagedata, int htwd)
	{
		glActiveTexture(GL_TEXTURE0 + texunit);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, htwd, htwd, GL_BGRA, GL_UNSIGNED_BYTE, imagedata);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void MakeActive(GLuint uniloc)
	{
		glActiveTexture(GL_TEXTURE0 + texunit);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glUniform1i(uniloc, texunit);
	}

	void Init(GLushort texunit)
	{
		this->texunit = texunit;
	}

	void Cleanup()
	{
		glDeleteTextures(1, &textureID);
	}
protected:
	GLuint textureID;
	GLushort texunit;
};
