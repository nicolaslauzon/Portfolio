///\file Physic.hpp

#ifndef PHYSIC_HPP
#define PHYSIC_HPP

#include "Vector.hpp"
#include "CoordinateSystem.hpp"


///\class Physic
///\brief Formules de physiques utiles
class Physic {
public:

    ///\brief Optient la nouvelle vitesse
    ///\param speed vitesse de la dernière update
    ///\param a accélération
    ///\param delta_t temps écoulé depuis la dernière update
    ///\return nouvelle vitesse
    static const Vector3d UpdateSpeed(const Vector3d& speed, const Vector3d& a, const double& deltaT) {
        return speed + a * deltaT;
    }

    ///\brief Optient la nouvelle position
    ///\param pos position de la dernière update
    ///\param speed vitesse de la dernière update
    ///\param a accélération
    ///\param delta_t temps écoulé depuis la dernière update
    ///\return nouvelle position
    static const Vector3d UpdatePosition(const Vector3d& pos, const Vector3d& speed, const Vector3d& a, const double& deltaT) {
        Vector3d x;
        x  = pos + speed * deltaT - (a * deltaT * deltaT * 0.5);
        return x;
    }

    ///\brief Modifie l'accélération en fonction de la friction
    ///\param frictionFactor coefficient de frottement cinétique
    ///\param acceleration accélération de l'objet (le z est utilisé en tant que aGravitationelle)
    ///\param objectMass masse de l'objet
    ///\return Nouvelle accélération
    static const Vector3d ApplyFriction(double frictionFactor, Vector3d acceleration, double objectMass) {
        double N = abs(acceleration.z * objectMass);
        double aFriction = abs(frictionFactor * N / objectMass);
        if (acceleration.x > 0.0) {
            acceleration.x -= aFriction;
        }
        if (acceleration.x < 0.0) {
            acceleration.x += aFriction;
        }
        if (acceleration.y > 0.0) {
            acceleration.y -= aFriction;
        }
        if (acceleration.y < 0.0) {
            acceleration.y += aFriction;
        }
        return acceleration;
    }

    static const bool BoundingSphereCollisionTest(double rayonObjet1, double rayonObjet2, CoordinateSystem positionObjet1, CoordinateSystem positionObjet2) {
        //sqrt(((positionObjet1.GetOrigin().x - positionObjet2.GetOrigin().x)) + (positionObjet1.GetOrigin().y - positionObjet2.GetOrigin().y) + (positionObjet1.GetOrigin().z - positionObjet1.GetOrigin().z));
        if ((sqrt(((positionObjet1.GetOrigin().x - positionObjet2.GetOrigin().x) * (positionObjet1.GetOrigin().x - positionObjet2.GetOrigin().x)) + ((positionObjet1.GetOrigin().y - positionObjet2.GetOrigin().y) * (positionObjet1.GetOrigin().y - positionObjet2.GetOrigin().y)) + ((positionObjet1.GetOrigin().z - positionObjet2.GetOrigin().z) * (positionObjet1.GetOrigin().z - positionObjet2.GetOrigin().z))) - (rayonObjet1 + rayonObjet2)) <= 0)
            return true;
        return false;
        sqrt(((positionObjet1.GetOrigin().x - positionObjet2.GetOrigin().x) * (positionObjet1.GetOrigin().x - positionObjet2.GetOrigin().x)) + ((positionObjet1.GetOrigin().y - positionObjet2.GetOrigin().y) * (positionObjet1.GetOrigin().y - positionObjet2.GetOrigin().y)) + ((positionObjet1.GetOrigin().z - positionObjet2.GetOrigin().z) * (positionObjet1.GetOrigin().z - positionObjet2.GetOrigin().z)));
    }
};
#endif // PHYSIC_HPP