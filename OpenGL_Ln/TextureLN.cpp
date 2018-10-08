#include "TextureLN.h"
#include "stb_image.h"
#include <iostream>

OPENGL_LN::TextureLN::TextureLN()
{
	_data = NULL;
	_texture = NULL;
}

void OPENGL_LN::TextureLN::loadImg(const char* path)
{
	if (_data)
	{
		stbi_image_free(_data);
		_data = NULL;
	}

	char real_path[MAX_PATH] = { 0 };
	strcpy_s(real_path, path);
#ifdef WIN32
	memset(real_path, 0, MAX_PATH);
	const char* prefix = "../Resources/";
	strcpy_s(real_path, prefix);
	strcat_s(real_path, path);
#endif
	_data = stbi_load(real_path, &(_image._width), &(_image._height), &(_image._channelNum), 0);
	if (!_data)
	{
		std::cout << "Err: STB::IMAGE::LOAD::FAILED" << std::endl;
	}
}

void OPENGL_LN::TextureLN::flushSingleImgIntoBuffer(const char * path)
{
	if (glIsTexture(_texture))
	{
		glDeleteTextures(1, &_texture);
	}
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	this->loadImg(path);
	if (_data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _image._width, _image._height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(_data);
}

