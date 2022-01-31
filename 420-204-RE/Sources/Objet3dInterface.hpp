///\file Objet3dInterface.hpp

#ifndef OBJET3DINTERFACE_HPP
#define OBJET3DINTERFACE_HPP

///\brief Interfaces disponibles
enum InterfaceId {
    COLLISION = 0x00000001, // Collision interface
    TRANSFORM = 0x00000002, // Transformation interface
};


///\class Objet3dInterface
///\brief Interface pour améliorer un Objet3d
class Objet3dInterface {
public:

    virtual const InterfaceId Id() const = 0;
};
#endif // OBJET3DINTERFACE_HPP
