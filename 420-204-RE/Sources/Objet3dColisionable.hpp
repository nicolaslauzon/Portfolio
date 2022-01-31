///\file Objet3dColisionable.hpp
#ifndef OBJET3DCOLISIONABLE_HPP
#define OBJET3DCOLISIONABLE_HPP

#include "Objet3dDrawable.hpp"
#include "CollisionInterface.hpp"

///\class Objet3dColisionable
///\brief Objet3dDrawable qui est colisionable
class Objet3dColisionable : public Objet3dDrawable, public CollisionInterface {

public:
    Objet3dColisionable(TexturedMesh* mesh, const unsigned int& textureId) : Objet3dDrawable(mesh, textureId) {

    }
};
#endif