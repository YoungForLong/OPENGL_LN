#include "TextureLN.h"
#include "stb_image.h"
#include <iostream>

OPENGL_LN::TextureLN::TextureLN()
{
	_data = NULL;
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


void OPENGL_LN::TextureLN::flushSingleImgIntoBuffer(const char* path)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	this->loadImg(path);
	if (_data)
	{
		auto color = _image._channelNum == 3 ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, color, _image._width, _image._height, 0, color, GL_UNSIGNED_BYTE, _data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	this->_textureArr.push_back(texture);
	stbi_image_free(_data);
	_data = NULL;
}

void OPENGL_LN::TextureLN::flushMixImgIntoBuffer(const std::initializer_list<std::string> pathList)
{
	for (auto iter = pathList.begin(); iter != pathList.end(); ++iter)
	{
		flushSingleImgIntoBuffer(iter->c_str());
	}
}

void OPENGL_LN::TextureLN::tick(float dt)
{
	unsigned int order = 0;
	for (auto iter = _textureArr.begin(); iter != _textureArr.end(); ++iter)
	{
		glActiveTexture(GL_TEXTURE0 + order);
		glBindTexture(GL_TEXTURE_2D, *iter);
		order++;
		order %= 16;
	}
}

void OPENGL_LN::TextureLN::clearTextureCache()
{
	if (!_textureArr.empty())
	{
		for (auto iter = _textureArr.end(); iter != _textureArr.begin(); ++iter)
		{
			glDeleteTextures(1, &(*iter));
			*iter = 0;
		}
		_textureArr.clear();
	}
}