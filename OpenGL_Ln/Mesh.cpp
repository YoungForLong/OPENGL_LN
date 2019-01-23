#include "Mesh.h"
#include "Texture.h"
#include "TextureLoader.h"
#include <iostream>

OPENGL_LN::Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, std::vector<unsigned int>&& diffuseMaps, std::vector<unsigned int>&& specularMaps, std::vector<unsigned int>&& heightMaps, std::vector<unsigned int>&& normalMaps)
{
	std::swap(this->vertices_, vertices);
	std::swap(this->indices_, indices);
	std::swap(this->diffuseMaps_, diffuseMaps);
	std::swap(this->specularMaps_, specularMaps);
	std::swap(this->heightMaps_, heightMaps);
	std::swap(this->normalMaps_, normalMaps);
	init();
}

OPENGL_LN::Mesh::Mesh(aiMesh * mesh, const aiScene * scene)
{
	for (size_t i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex v;
		v.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		v.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		v.texCoords = glm::vec2(0.0f, 0.0f);
		if (mesh->mTextureCoords[0])
		{
			v.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}

		vertices_.push_back(v);
	}

	for (size_t i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j)
		{
			indices_.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		loadTextures(material, aiTextureType_DIFFUSE, diffuseMaps_);
		loadTextures(material, aiTextureType_SPECULAR, specularMaps_);
		loadTextures(material, aiTextureType_HEIGHT, heightMaps_);
		loadTextures(material, aiTextureType_NORMALS, normalMaps_);
	}

	init();
}

void OPENGL_LN::Mesh::loadTextures(aiMaterial * mat, aiTextureType type, std::vector<unsigned int>& container)
{
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// async
		unsigned int id = TEXTUREMNG->initOneObj(str.C_Str());
		std::cout << "load texture: " << str.C_Str() << endl;
		container.push_back(id);
	}
}

OPENGL_LN::Mesh::Mesh(Mesh && other)
{
	std::swap(this->vertices_, other.vertices_);
	std::swap(this->indices_, other.indices_);
	std::swap(this->diffuseMaps_, other.diffuseMaps_);
	std::swap(this->specularMaps_, other.specularMaps_);
	std::swap(this->heightMaps_, other.heightMaps_);
	std::swap(this->normalMaps_, other.normalMaps_);
	this->init();
}

void OPENGL_LN::Mesh::render(Shader * shader)
{
	/*if (!TEXTUREMNG->hasLoadFinished())
		return;*/
	
	GLuint unitCount = 0;
	auto flushFunc = [&](const std::vector<unsigned int>& map, const std::string& refName) {
		size_t nameCount = 1;
		for (auto textureId : map)
		{
			auto texture = TEXTUREMNG->getObjbyKey(textureId);
			glActiveTexture(GL_TEXTURE0 + unitCount);
			auto name = refName + std::to_string(nameCount);
			shader->setVal(name.c_str(), (int)unitCount);
			unitCount++;
			nameCount++;
			glBindTexture(GL_TEXTURE_2D, texture->getGLId());
		}
	};
	
	flushFunc(diffuseMaps_, "diffuse_map");

	flushFunc(specularMaps_, "specular_map");

	flushFunc(heightMaps_, "height_map");

	flushFunc(normalMaps_, "normal_map");

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

//void OPENGL_LN::Mesh::clone(Mesh & dest, Mesh & src)
//{
//	dest.VAO = src.VAO;
//	dest.VBO = src.VBO;
//	dest.EBO = src.EBO;
//	
//	dest.indices_ = vector<unsigned int>(src.indices_.begin(), src.indices_.end());
//	dest.vertices_ = std::move(src.vertices_);
//	dest.diffuseMaps_ = std::move(src.diffuseMaps_);
//	dest.specularMaps_ = std::move(src.specularMaps_);
//	dest.heightMaps_ = std::move(src.heightMaps_);
//	dest.normalMaps_ = std::move(src.normalMaps_);
//}

void OPENGL_LN::Mesh::init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(), &vertices_[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices_.size(), &indices_[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	// normal vec
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(Vertex), (void*)(offsetof(Vertex, texCoords)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}


