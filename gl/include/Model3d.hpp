#ifndef MODEL_HPP
#define MODEL_HPP

#include "Mesh.hpp"
#include "TextureGL.hpp"

namespace acc {

class Model3d : public Resource {
	public:
		Model3d(void);
		Model3d(const std::string &name);
		void addMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, TextureGL *texture);
		void addMesh(Mesh *mesh, TextureGL *texture);
		void addModel(Model3d *other_model);
		void render(Shader *shader);
		~Model3d(void);

	private:
		std::vector<Mesh *> meshes;
		std::vector<TextureGL *> textures;

		bool unload;
};

};

#endif
