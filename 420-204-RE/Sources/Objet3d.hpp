///\file Objet3d.hpp

#ifndef OBJET3D_HPP
#define OBJET3D_HPP

#include "CoordinateSystem.hpp"
#include "Objet3dInterface.hpp"

///\class Objet3d
///\brief Objet 3d openGl
class Objet3d {
private:
    unsigned int interfaceId_; ///< signature des interfaces utilisés

public:

    Objet3d() {
        
    }

    bool SupportThisInterface(unsigned int interface) const {
        return (interface & interfaceId_) == interface;
    }
	        
    virtual void Draw() = 0;
};
#endif // OBJET3D_HPP
