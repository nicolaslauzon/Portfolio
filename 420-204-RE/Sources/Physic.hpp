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
        if ((sqrt(((positionObjet1.GetOrigin().x - positionObjet2.GetOrigin().x) * (positionObjet1.GetOrigin().x - positionObjet2.GetOrigin().x)) + ((positionObjet1.GetOrigin().y - positionObjet2.GetOrigin().y) * (positionObjet1.GetOrigin().y - positionObjet2.GetOrigin().y)) + ((positionObjet1.GetOrigin().z - positionObjet2.GetOrigin().z) * (positionObjet1.GetOrigin().z - positionObjet2.GetOrigin().z))) - (rayonObjet1 + rayonObjet2)) <= 0)
            return true;
        return false;
    }

    static Vector3d* RayThroughPlane(const Vector3d& segment, const Vector3d& segmentOrigin, const Vector3d triangle[3], const Vector3d& normal) {
        const double tmp = segment * normal;
        if (std::abs(tmp) < 1e-5) {
            return nullptr;
        }

        const Vector3d w = triangle[0] - segmentOrigin;

        const double r = (normal * w) / (tmp);
        if (r < 0.0) {
            return nullptr;
        }
        else if (r > 1.0) {
            return nullptr;
        }
        const Vector3d i = segment * r + segmentOrigin;
        // A[0] // B[1] // C[2]
        const Vector3d AB = triangle[1] - triangle[0];
        Vector3d v = AB - AB.Projected(triangle[1] - triangle[2]);

        const double a = 1.0 - (v * (i - triangle[0])) / (v * AB);

        const Vector3d BC = triangle[2] - triangle[1];
        v = BC - BC.Projected(triangle[2] - triangle[0]);

        const double b = 1.0 - (v * (i - triangle[1])) / (v * BC);
        
        if ((a <= 1.0 && a >= 0.0) && (b <= 1.0 && b >= 0.0))
            return new Vector3d(i);
        
        return nullptr;
    }
};
#endif // PHYSIC_HPP