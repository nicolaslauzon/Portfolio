///\file Texture.hpp
#ifndef MODEL_HPP
#define MODEL_HPP

#include <SDL2/SDL_opengl.h>
#include "Mesh.hpp"
#include "Resource.hpp"

///\class Texture
///\brief Modèle d'une ressource de type Texture
class Model : public Resource {
private:
	TexturedMesh* model_;
public:
	///\brief Constructeur
	///\param Identifiant OpenGL de la texture
	Model(string fileName) {
		model_ = new TexturedMesh();
		model_->load((FileSystem::modelsPath + fileName).c_str());
	}

	~Model() {
		delete model_;
	}

	virtual void Draw() {
		model_->Draw();
	}

	///\brief Retourne la ressource
	void* GetData() {
		return model_;
	}
};
#endif//MODEL_HPP
