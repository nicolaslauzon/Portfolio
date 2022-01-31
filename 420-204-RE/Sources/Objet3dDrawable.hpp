///\file Objet3dDrawable.hpp

#ifndef OBJET3DDRAWABLE_HPP
#define OBJET3DDRAWABLE_HPP

#include "Objet3d.hpp"
#include "Mesh.hpp"
#include "TransformInterface.hpp"
#include "ResourceManager.hpp"
#include "Matrix44d.hpp"

///\class Objet3dDrawable
///\brief objet 3d qui se dessine
class Objet3dDrawable : public Objet3d, public TransformInterface  {
protected:
    TexturedMesh* mesh_; ///< Objet affichable
    CoordinateSystem position_; ///< Position

 
public:
    Objet3dDrawable(TexturedMesh* mesh, const unsigned int& textureId) {
      mesh_ = new TexturedMesh(mesh);
      mesh_->setTexture(textureId);
      position_ = CoordinateSystem();
    }

    virtual void Transform(const Matrix44d& transformationMatrix) {
        mesh_->Transform(transformationMatrix);
        //position_.SetOrigin({})
    }

    virtual void Scale(const Vector3d& scaleValue) {
        mesh_->Scale(scaleValue);
    }

    ///\brief Dessine l'objet
    virtual void Draw() {
        mesh_->Draw();
    }

    ///\brief set la texture
    void SetTexture(unsigned int texId) {
        mesh_->setTexture(texId);
    }

    double GetRayon() const {
        return mesh_->GetFarthestPoint();
    }

    const CoordinateSystem& GetPosition() {
        return position_;
    }

    void SetOrigin(const Vector3d& position) {
        position_.SetOrigin(position);
    }

    void SetZDirection(const Vector3d& position) {
        position_.SetZDirection(position);
    }
    void SetXDirection(const Vector3d& position) {
        position_.SetXDirection(position);
    }




};

#endif // OBJET3DDRAWABLE_HPP