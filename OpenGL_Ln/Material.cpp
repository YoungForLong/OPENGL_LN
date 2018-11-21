#include "Material.h"
#include "Shader.h"
#include "Texture.h"

OPENGL_LN::Material::Material(const std::string& refName,float shininess):
	_refName(refName),
	_shininess(shininess)
{
	_texture = new Texture();
}

OPENGL_LN::Material::~Material()
{

}

void OPENGL_LN::Material::applyMaterial(Shader& compiler)
{
	compiler.setVal(_refName + ".shininess", _shininess);
	compiler.setVal(_refName + ".diffuse", 0);
	compiler.setVal(_refName + ".specular", 1);
}

void OPENGL_LN::Material::bindDiffuseMap(const char* img)
{
	_texture->flushSingleImgIntoBuffer(img);
}

void OPENGL_LN::Material::bindSpecularMap(const char* img)
{
	_texture->flushSingleImgIntoBuffer(img);
}

void OPENGL_LN::Material::tick(float dt)
{
	_texture->tick(dt);
}
