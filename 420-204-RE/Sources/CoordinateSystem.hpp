///\file CoordinateSystem.hpp

#ifndef COORDINATESYSTEM_HPP
#define COORDINATESYSTEM_HPP

#include "Vector.hpp"


///\class CoordinateSystem
///\brief système de coordonée
class CoordinateSystem {
private:
    Vector3d origin_; ///< origine
    Vector3d xDirection_; ///< Axe x
    Vector3d zDirection_; ///< Axe z

public:

    ///\brief Base Constructor
    CoordinateSystem() {
        origin_ = Vector3d();
        xDirection_ = Vector3d(1.0, 0.0, 0.0);
        zDirection_ = Vector3d(0.0, 0.0, 1.0);
    }

    ///\brief Build for a chosen origin
    ///\param origin location of the system
    CoordinateSystem(const Vector3d& origin) {
        origin_ = origin;
        xDirection_ = Vector3d(1.0, 0.0, 0.0);
        zDirection_ = Vector3d(0.0, 0.0, 1.0);
    }

    ///\brief Build for a chosen origin, front and top vector
    ///\param origin location of the system
    ///\param xDirection front of the system
    ///\param zDirection top of the system
    CoordinateSystem(const Vector3d& origin, const Vector3d& xDirection, const Vector3d& zDirection) {
        origin_ = origin;
        xDirection_ = xDirection;
        zDirection_ = zDirection; 
    }

    ///\brief Set l'origine
    void SetOrigin(const Vector3d& origin) {
        origin_ = origin;
    }

    ///\brief Set l'axe x
    void SetXDirection(const Vector3d& xDirection) {
        xDirection_ = xDirection;
    }

    ///\brief Set l'axe y, rotation sur l'axe des z qui reste constant
    void SetYDirection(Vector3d yDirection) {
        yDirection.Normalize();
        xDirection_ =  yDirection % zDirection_;
    }

    ///\brief Set l'axe z
    void SetZDirection(const Vector3d& zDirection) {
        zDirection_ = zDirection;
    }

    ///\brief  obtain origin
    ///\return origin
    const Vector3d& GetOrigin() const {
        return origin_;
    }

    ///\brief  obtain x direction
    ///\return direction x
    const Vector3d& GetXDirection() {
        return xDirection_;
    }

    ///\brief  obtain y direction
    ///\return direction y
    const Vector3d GetYDirection() {
        return xDirection_ % zDirection_;
    }

    ///\brief  obtain z direction
    ///\return direction z
    const Vector3d& GetZDirection() {
        return zDirection_;
    }

    void Rotate() {

    }
    
    void Translate(const Vector3d& translation) {
        origin_ = origin_ + translation;
    }
};
#endif // COORDINATESYSTEM_HPP