///\file CollisionInterface.hpp

#ifndef COLLISIONINTERFACE_HPP
#define COLLISIONINTERFACE_HPP

#include "Objet3dInterface.hpp"
#include "Objet3d.hpp"

///\class CollisionInterface
///\brief Interface pour la gestion de collision entre Objet3d
class CollisionInterface : public Objet3dInterface {
public:
    const InterfaceId Id() const {
        return COLLISION;
    }
};
#endif // COLLISIONINTERFACE_HPP
