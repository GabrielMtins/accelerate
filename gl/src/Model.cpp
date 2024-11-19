#include "Model.hpp"
#include "glad/glad.h"

namespace acc {

Model3d::Model3d(void){
}

Model3d::Model3d(const std::string &name){
	setName(name);
}

void Model3d::addMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, TextureGL *texture){
	Mesh *mesh = new Mesh();
	mesh->load(vertices, indices);

	addMesh(mesh, texture);
}

void Model3d::addMesh(Mesh *mesh, TextureGL *texture){
	meshes.push_back(mesh);
	textures.push_back(texture);
}

void Model3d::render(Shader *shader){
	for(size_t i = 0; i < meshes.size(); i++){
		glBindTexture(GL_TEXTURE_2D, textures[i]->getId());
		meshes[i]->render(shader);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

Model3d::~Model3d(void){
	for(Mesh *i : meshes)
		delete i;
}

};
