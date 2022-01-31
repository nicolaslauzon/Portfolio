///\file Camera.hpp
///\brief Camera 

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <cmath>
#include "GlContext.hpp"
#include "Vector.hpp"
#include "Matrix44d.hpp"
#include "Observer.hpp"
#include "Character.hpp"

#define BASEDISTANCE 10.0
#define SPEEDMODIFIER 20.0

///\class StaticCamera
///\brief Cam�ra qui ne peut pas �tre boug� ou r�orient�
class StaticCamera {

protected: 
	Vector3d up_; ///< Vecteur pointant vers le haut de la cam�ra
	Vector3d side_; ///< Vecteur pointant vers le c�t� de la cam�ra
	Vector3d front_; ///< Vecteur pointant vers le devant de la cam�ra
	Vector3d position_; ///< Vecteur indiquant la position de la cam�ra
	Vector3d target_; ///< Vecteur indiquant la cible de la cam�ra
	Matrix44d viewMatrix_; ///< Matrice de vue de la cam�ra

public:

    ///\param up Vecteur pointant vers le haut de la cam�ra
    ///\param position Position de la cam�ra
    ///\param target Cible de la cam�ra
	StaticCamera(Vector3d up, Vector3d position, Vector3d target) {
        up_ = up;
        position_ = position;
        target_ = target;

		front_ = target_ - position_;
		front_.Normalize();

		side_ = (front_ % up_);
		side_.Normalize();

		up_ = (side_ % front_);
		up_.Normalize();

        viewMatrix_.LoadView(front_, side_, up_);
	}

    ///\brief Modifie la vue de la cam�ra
	void ApplyView() {
		glLoadIdentity();
		glMultMatrixd(viewMatrix_.GetMatrix());
		glTranslated(-position_.x, -position_.y, -position_.z);
	}


};

///\class DynamicCamera
///\brief Caméra suivant le personnage
class DynamicCamera : public StaticCamera {
private:
    Character* character_; ///< Personnage suivi par la caméra
    double speed_; ///< Vitesse à laquelle la caméra se déplace
    double previousDistance_;

public:
    ///\param character Personnage suivi par la caméra
    DynamicCamera(Character* character) : StaticCamera(Vector3d(0.0, 1.0, 0.0), character->GetCoordinates().GetOrigin() - character->GetCoordinates().GetXDirection() * BASEDISTANCE + Vector3d(0.0, 5.0, 0.0), character->GetCoordinates().GetOrigin()) {
        character_ = character;
        speed_ = previousDistance_ = 0.0;
    }

    ///\brief Effectue le d�placement de la cam�ra
    ///\param deltaT Temps entre deux d�placements
    void Move(double deltaT) {
        target_ = character_->GetCoordinates().GetOrigin();

        /* Rotation de la position de la caméra
        Matrix44d rotMatrix;
        Vector3d tempVector1 = {character_->GetCoordinates().GetXDirection().x, 0.0, character_->GetCoordinates().GetXDirection().z}, tempVector2 = {front_.x, 0.0, front_.z};
        rotMatrix.LoadRotation({ 0.0, 1.0, 0.0 }, tempVector1.GetAngle(tempVector2)); // TOFIX : Détermination de l'angle de rotation
        position_ = position_ - target_;
        position_ = rotMatrix * position_;
        position_ = position_ + target_;
        */

        // Mise à jour des vecteurs de la caméra
        up_ = { 0.0, 1.0, 0.0 };
        front_ = target_ - position_; front_.Normalize();
        side_ = (front_ % up_); side_.Normalize();
        up_ = (side_ % front_); up_.Normalize();

        // Mise à jour de la vitesse de déplacement de la caméra
        if ((target_ - position_).GetNorm() > BASEDISTANCE && (target_ - position_).GetNorm() < previousDistance_) {
            if (speed_ > 0.0)
                speed_ = speed_ - 20.0;
        }
        else
            if ((target_ - position_).GetNorm() > BASEDISTANCE)
                if (speed_ < 25.0)
                    speed_ = speed_ + 20.0;
        
        previousDistance_ = (target_ - position_).GetNorm();

        // Mise à jour de la position de la caméra
        position_ = position_ + (front_ * speed_ * deltaT);
        position_.y = character_->GetCoordinates().GetOrigin().y + 15.0;

        front_ = target_ - position_; front_.Normalize();
        side_ = (front_ % up_); side_.Normalize();
        up_ = (side_ % front_); up_.Normalize();

        // Mise à jour de la matrice de la caméra
        viewMatrix_.LoadView(front_, side_, up_);
    }
};


///\class FlyingCamera
///\brief Cam�ra pouvant �tre d�plac�e
class FlyingCamera : public StaticCamera, public Observer<SDL_Event>{
private:
	bool forward_; ///< D�placement de la cam�ra vers le devant
	bool backward_; ///< D�placement de la cam�ra vers le derriere
	bool left_; ///< D�placement de la cam�ra vers la gauche
	bool right_; ///< D�placement de la cam�ra vers la droite
    bool upward_; ///< D�placement de la cam�ra vers le haut
    bool downward_; ///< D�placement de la cam�ra vers le bas
    bool locked_; ///< Si la souris est bloqu�e au centre de l'�cran
        
public:

    ///\param up Vecteur pointant vers le haut de la cam�ra
    ///\param position Position de la cam�ra
    ///\param target Cible de la cam�ra
	FlyingCamera(Vector3d up, Vector3d position, Vector3d target) : StaticCamera(up, position, target) {
		forward_ = backward_ = left_ = right_ = upward_ = downward_ = false;
        locked_ = true;
        SDL_SetRelativeMouseMode(SDL_TRUE);
	}

    ///\brief D�tecte les touches enfonc� par l'utilisateur pour le mouvement
    ///\param sdlEvent Evenement SDL effectu� par l'utilisateur
	void Notification(SDL_Event sdlEvent) {
        switch (sdlEvent.type) {
        case SDL_MOUSEMOTION: {
            Matrix44d rotMatrix;
            up_ = { 0.0, 1.0, 0.0 };

            double angle = (double)sdlEvent.motion.xrel * M_PI / 180.0;
            rotMatrix.LoadRotation(up_, angle);
            front_ = rotMatrix * front_;
            
            angle = (double)sdlEvent.motion.yrel * M_PI / 180.0;
            rotMatrix.LoadRotation(side_, angle);
            front_ = rotMatrix * front_;
            side_ = (front_ % up_);
            side_.Normalize();
            up_ = (side_ % front_);
            up_.Normalize();
            viewMatrix_.LoadView(front_, side_, up_);
        } break;

        case 771:

            switch (sdlEvent.key.state) {
            case SDLK_w:
                forward_ = true;
                break;

            case SDLK_a:
                left_ = true;
                break;

            case SDLK_s:
                backward_ = true;
                break;

            case SDLK_d:
                right_ = true;
                break;

            case SDLK_q:
                upward_ = true;
                break;

            case SDLK_e:
                downward_ = true;
                break;

            case SDLK_SPACE:
                locked_ = !locked_;
                break;
            }
            break;

        case SDL_KEYUP:
            switch (sdlEvent.key.keysym.sym) {
            case SDLK_w:
                forward_ = false;
                break;

            case SDLK_a:
                left_ = false;
                break;

            case SDLK_s:
                backward_ = false;
                break;

            case SDLK_d:
                right_ = false;
                break;

            case SDLK_q:
                upward_ = false;
                break;

            case SDLK_e:
                downward_ = false;
                break;
            }
            break;
        }
	}

    ///\brief Effectue le d�placement de la cam�ra
    ///\param deltaT Temps entre deux d�placements
	void Move(double deltaT) {
        if (locked_) {
            if (forward_)
                position_ = position_ + (front_ * deltaT * SPEEDMODIFIER); // Le modificateur de vitesse peut �tre chang�
            if (backward_)
                position_ = position_ - (front_ * deltaT * SPEEDMODIFIER);
            if (right_)
                position_ = position_ + (side_ * deltaT * SPEEDMODIFIER);
            if (left_)
                position_ = position_ - (side_ * deltaT * SPEEDMODIFIER);
            if (upward_)
                position_.y += (deltaT * SPEEDMODIFIER);
            if (downward_)
                position_.y -= (deltaT * SPEEDMODIFIER);
        }
       
	}
};


#endif //CAMERA_HPP