///\file TransformInterface.hpp

#ifndef TRANSFORMINTERFACE_HPP
#define TRANSFORMINTERFACE_HPP

#include "Objet3dInterface.hpp"
#include "Objet3d.hpp"
#include "Matrix44d.hpp"

///\class TransformInterface
///\brief Interface de transformation
class TransformInterface : public Objet3dInterface {
public:

    ///\brief Transforme l'objet à partir de la matrice de transformation
    ///\param transformationMatrix matrice de transformation
    virtual void Transform(const Matrix44d& transformationMatrix) = 0;

    ///\brief translate l'objet
    ///\param translation vecteur qui représente la translation
    virtual void Translate(const Vector3d& translation) {
        Matrix44d matrix;
        matrix.LoadIdentity();
        matrix.Translate(translation);
        Transform(matrix);
    }

    ///\brief tourne l'objet selon un point, un axe et un angle déterminé
    ///\param rotationAxis axe de rotation
    ///\param angle angle de rotation en radian
    virtual void Rotate(const Vector3d& rotationAxis, double angle) {
        Matrix44d matrix;
        matrix.LoadIdentity();
        matrix.LoadRotation(rotationAxis, angle);
        Transform(matrix);
        
    }

    ///\brief effectue une transformation sur l'objet
    ///\param transformationMatrix matrice de transformation
    virtual void operator*(const Matrix44d& transformationMatrix) {
        Transform(transformationMatrix);
    }

    ///\brief effectue une translation sur l'objet
    ///\param translation vecteur de translation
    virtual void operator+(const Vector3d& translation) {
        Translate(translation);
    }

    const InterfaceId Id() const {
        return TRANSFORM;
    }

};
#endif // TRANSFORMINTERFACE_HPP