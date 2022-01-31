///\file Texture.hpp
#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <SDL2/SDL_opengl.h>

#include "Resource.hpp"

///\class Texture
///\brief Mod�le d'une ressource de type Texture
class Texture : public Resource {
private:
	unsigned int id_;

public:
	///\brief Constructeur
	///\param id Identifiant OpenGL de la texture
	Texture(const unsigned int& id) {
		id_ = id;
	}

	~Texture() {
		glDeleteTextures(1, &id_);
	}
	///\brief Retourne la ressource
	void* GetData() {
		return &id_;
	}
};

#endif//TEXTURE_HPP
