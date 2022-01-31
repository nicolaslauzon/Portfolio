#pragma once
///\file Sprite3dDdrawable.hpp

#ifndef SPRITE3DDRAWABLE_HPP
#define SPRITE3DDRAWABLE_HPP

#include "Sprite3D.hpp"
#include "Objet3dDrawable.hpp"
///\class Objet3dDrawable
///\brief objet 3d qui se dessine
class Sprite3Ddrawable : public Objet3dDrawable {
protected:
    Sprite3D* sprite_; ///< Sprite affichable

public:
  
   Sprite3Ddrawable(Sprite3D* sprite, unsigned int textureId) : Objet3dDrawable((sprite->getFrame(0)),textureId) {
     sprite_ = new Sprite3D(sprite, sprite->GetNumberOfFrames());
     sprite_->SetAllTexture(textureId);
     position_ = CoordinateSystem();
   }

    virtual void Transform(const Matrix44d& transformationMatrix) {
      sprite_->Transform(transformationMatrix);
    }

    void SetDelay(double delay) {
        sprite_->SetDelay(delay);
    }

    double GetDelay() {
        return sprite_->GetDelay();
    }

   /* virtual void Scale(const Vector3d& scaleValue) {
        sprite_->Scale(scaleValue);
    }*/

    ///\brief Dessine l'objet
    virtual void Draw() {
        sprite_->Draw();
    }

   /* double GetRayon() const {
        return mesh_->GetFarthestPoint();
    }*/
   
    void UpdateFrame(double deltatime) {
        sprite_->UpdateFrame(deltatime);
    }


    

    void SetNextFrame() {
        sprite_->SetNextFrame();
    }
    const CoordinateSystem GetPosition() {
        return position_;
    }
};

#endif // OBJET3DDRAWABLE_HPP