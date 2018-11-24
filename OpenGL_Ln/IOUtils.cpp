#include "IOUtils.h"
#include "stb_image.h"

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

using namespace OPENGL_LN;

const char * IOUtils::getFilePath(const char * filename)
{
	char real_path[MAX_PATH] = { 0 };
	strcpy_s(real_path, filename);
#ifdef WIN32
	memset(real_path, 0, MAX_PATH);
	const char* prefix = "../Resources/";
	strcpy_s(real_path, prefix);
	strcat_s(real_path, filename);
#endif
	return real_path;
}
