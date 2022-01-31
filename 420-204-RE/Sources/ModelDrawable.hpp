#pragma once
//\file Objet3dDrawable.hpp

#ifndef MODELDRAWABLE_HPP
#define MODELDDRAWABLE_HPP

#include "Objet3d.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "ResourceManager.hpp"
#include "TransformInterface.hpp"
///\class Objet3dDrawable
///\brief objet 3d qui se dessine

class ModelDrawable : public TransformInterface {
protected:


};
/*class ModelDrawable {
protected:
    unsigned int modelId_; ///< Objet affichable

public:
    ModelDrawable(string modelname) {
        
        
        modelId_ = *(ResourceManager::GetResource<unsigned int*>(modelname));
       
    }

    ///\brief Dessine l'objet
    virtual void Draw() {
        
    }
};*/

#endif