///\file Resource.hpp
#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

///\class Resource
///\brief Modèle d'une ressource
class Resource {
public:
	virtual void* GetData() = 0;
};
#endif //RESOURCE_HPP