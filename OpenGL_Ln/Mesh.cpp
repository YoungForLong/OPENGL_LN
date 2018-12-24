#include "Mesh.h"
#include "Texture.h"


OPENGL_LN::Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::vector<Texture>& textures) :
	vertices_(move(vertices)),
	indices_(move(indices)),
	textures_(move(textures))
{
	init();
}

OPENGL_LN::Mesh::Mesh(const std::vector<Vertex>&& vertices, const std::vector<unsigned int>&& indices, std::vector<Texture>&& textures):
	vertices_(vertices),
	indices_(indices),
	textures_(textures)
{
	init();
}

OPENGL_LN::Mesh::Mesh(Mesh && other)
{
	this->vertices_ = std::move(other.vertices_);
	this->indices_ = std::move(other.indices_);
	this->textures_ = std::move(other.textures_);
	this->init();
}

void OPENGL_LN::Mesh::render(Shader * shader)
{

}

void OPENGL_LN::Mesh::clone(Mesh & dest, Mesh & src)
{
	dest.VAO = src.VAO;
	dest.VBO = src.VBO;
	dest.EBO = src.EBO;

	dest.indices_ = std::move(src.indices_);
	dest.vertices_ = std::move(src.vertices_);
	dest.textures_ = std::move(src.textures_);
}

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


