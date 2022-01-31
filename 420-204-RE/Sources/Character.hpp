///\file Character.hpp

#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Observer.hpp"
#include "Mesh.hpp"
#include "TransformInterface.hpp"
#include "Objet3dDrawable.hpp"
#include "Chrono.hpp"
#include "Physic.hpp"

#include <SDL2/SDL.h>

#define ACCELERATION 100.0
#define MAXSPEED 30.0
#define W 0
#define D 1
#define S 2
#define A 3

///\brief Statut du personnage
// enum CharacterStatus {
//     Static = 1,
//     Walking = 2,
//     Jumping = 3
// };

///\class Character
///\brief Personnage du jeu
class Character : public Objet3dDrawable, public Observer<SDL_Event>, public Observer<bool>
{
private:
    Chrono time;             ///< Temps pour la fonction Jump
    bool jumping;            ///< Booleen pour savoir si on saute
    double spacePressedTime; ///< Temps que la touche espace a ete pressee
    bool reset_;             ///< Booleen pour savoir si la touche espace a été relevé
    double animtime_;
    bool isStunned_;  ///< Booleen pour savoir si le personnage est immobilisé
    Vector3d previousPos_;
    Vector3d speed_; ///< Vecteur de vitesse du personnage
    Vector3d acceleration_;
    Vector3d direction;
    bool movDir_[4] = {false, false, false, false}; ///< Tableau qui retiens quelles touches sont enfoncées

public:
    Character(TexturedMesh* mesh, const unsigned int& textureId) : Objet3dDrawable(mesh, textureId)
    {
        direction = { 0.0,0.0,0.0 };
        time.Reset();
        jumping = false;
        animtime_ = 0;
        position_ = CoordinateSystem();
        spacePressedTime = 0.0;
        reset_ = false;
        previousPos_ = Vector3d();
        isStunned_ = false;
        
    }

    ///\brief Update le personnage
    ///\param deltaT temps écoulé depuis la dernière update
    void HandleUpdate(double deltaT)
    {
        animtime_ += deltaT;
        if (animtime_ >= 0.100) {
            //animtime_
        }

        acceleration_ = Vector3d();

        if (position_.GetOrigin().y <= -10.0) {
            jumping = false;
            speed_.y = 0.0;
            acceleration_.y = 0.0;
            position_.SetOrigin(Vector3d(position_.GetOrigin().x, position_.GetOrigin().y+1.0, position_.GetOrigin().z));
        }

        previousPos_ = position_.GetOrigin();
        
        if (movDir_[W] == true)
        {
            direction = position_.GetXDirection() * 25.0;
        }
        if (movDir_[D] == true)
        {
            direction = position_.GetZDirection() * 25.0;
        }
        if (movDir_[S] == true)
        {
            direction = position_.GetXDirection() * -25.0;
        }
        if (movDir_[A] == true)
        {
            direction = position_.GetZDirection() * -25.0;
        }

        if (movDir_[W] == false && movDir_[S] == false && speed_.x != 0.0)
        {
            direction = direction + Vector3d(speed_.x * -1.0 / abs(speed_.x), 0.0, 0.0);
        }
        if (movDir_[D] == false && movDir_[A] == false && speed_.z != 0.0)
        {
            direction = direction + Vector3d(0.0, 0.0, speed_.z * -1.0 / abs(speed_.z));
        }
        if (movDir_[W] == false && movDir_[A] == false && movDir_[S] == false && movDir_[D] == false && speed_.GetNorm() <= 1.0) {
            speed_ = Vector3d();
        }

        direction.Normalize();
        /*
        if (jumping) {            
            acceleration_.y = 0.0;
            acceleration_ = acceleration_ + Vector3d({ 0.0,-1.0,0.0 }, ACCELERATION * 2);
        }
        */

        acceleration_ = acceleration_ + Vector3d(direction, ACCELERATION);

        speed_ = Physic::UpdateSpeed(speed_, acceleration_, deltaT);
        if (speed_.GetNorm() > MAXSPEED + 1.0)
        {
            speed_.Normalize();
            speed_ = speed_ * MAXSPEED;
        }
        position_.SetOrigin(Physic::UpdatePosition(position_.GetOrigin(), speed_, acceleration_, deltaT));

#ifdef DEBUG
        // cout << "X ->"
        //     << "[" << position_.GetOrigin().x << "]\t";
        // cout << "Y ->"
        //     << "[" << position_.GetOrigin().y << "]\t";
        // cout << "Z ->"
        //     << "[" << position_.GetOrigin().z << "]" << endl;
        cout << "X ->"
            << "[" << speed_.x << "]\t";
        cout << "Y ->"
            << "[" << speed_.y << "]\t";
        cout << "Z ->"
            << "[" << speed_.z << "]" << endl;
#endif

        if (jumping) {

            acceleration_ = Vector3d({ 0.0,-1.0,0.0 }, ACCELERATION);

            speed_ = Physic::UpdateSpeed(speed_, (acceleration_), deltaT);

            position_.SetOrigin(Physic::UpdatePosition(position_.GetOrigin(), speed_, acceleration_, deltaT));

            
            //Quand le personnage retouche le sol remmettre le booléen a false
        }

        Vector3d translation(position_.GetOrigin() - previousPos_);
        if (translation.x != 0.0 || translation.y != 0.0 || translation.z != 0.0) {
            Translate(translation);
        }
    }

    ///\brief Fonction qui fait sauter le personnage
    ///\param characterStatus Statut actuel du caractère
    ///\param time Temps à l'appel de la fonction Jump
    void Jump(double timePressed)
    {
        jumping = true;

        if (timePressed > 0.00 && timePressed < 0.20)
        {
            speed_.y = (((0.43 - 0.38) / 0.30) * 100);
        }
        else if (timePressed >= 0.20 && timePressed < 0.70)
        {
            speed_.y = (((0.43 - 0.34) / 0.40) * 105);
        }
        else
        {
            speed_.y = (((0.43 - 0.29) / 0.50) * 110);
            
        }
        
    }

    Vector3d* GetSpeed() {
        return &speed_;
    }

    inline double GetRadius() {
        return GetRayon();
    }

    inline CoordinateSystem GetPosition() {
        return position_;
    }

    void SetPosition(const Vector3d& position) {
        position_.SetOrigin(position);
    }

    void Notification(SDL_Event event){

        if (event.type == SDL_MOUSEMOTION) {
            Matrix44d rotMatrix;
            Matrix44d tMatrix;

            tMatrix.LoadIdentity();
            tMatrix.Translate({ -position_.GetOrigin().x,-position_.GetOrigin().y ,-position_.GetOrigin().z });
            rotMatrix.LoadIdentity();

            double angle = (double)event.motion.xrel * M_PI / 180.0;
            rotMatrix.LoadRotation({ 0.0,1.0,0.0 }, angle);
            position_.SetXDirection(rotMatrix * position_.GetXDirection());
            position_.SetZDirection(position_.GetXDirection() % Vector3d(0.0, 1.0, 0.0));
            position_.SetZDirection(position_.GetZDirection().Normalized());

            mesh_->Transform(tMatrix);
            mesh_->Transform(rotMatrix);

            tMatrix.LoadIdentity();
            tMatrix.Translate({ position_.GetOrigin().x,position_.GetOrigin().y ,position_.GetOrigin().z });
            mesh_->Transform(tMatrix);

        }
        switch (event.key.keysym.sym)
        {
        case SDLK_w:
            if (event.type == SDL_KEYDOWN && !isStunned_)
            {
                movDir_[W] = true;
            }
            else if (event.type == SDL_KEYUP)
            {
                movDir_[W] = false;
            }
            break;

        case SDLK_a:
            if (event.type == SDL_KEYDOWN && !isStunned_)
            {
                movDir_[A] = true;
            }
            else if (event.type == SDL_KEYUP)
            {
                movDir_[A] = false;
            }
            break;

        case SDLK_s:
            if (event.type == SDL_KEYDOWN && !isStunned_)
            {
                movDir_[S] = true;
            }
            else if (event.type == SDL_KEYUP)
            {
                movDir_[S] = false;
            }
            break;

        case SDLK_d:
            if (event.type == SDL_KEYDOWN && !isStunned_)
            {
                movDir_[D] = true;
            }
            else if (event.type == SDL_KEYUP)
            {
                movDir_[D] = false;
            }
            break;

        case SDLK_SPACE:
            if (!jumping && !isStunned_)
            {

                if (event.type == SDL_KEYDOWN)
                {
                    if (reset_ == false)
                    {
                        time.Reset();
                        reset_ = true;
                    }
                }
                else if (event.type == SDL_KEYUP)
                {
                    spacePressedTime = time.GetElapsedSeconds();
                    reset_ = false;
                    Jump(spacePressedTime);
                }
            }
            break;
        }
    }

    void Notification(bool stun) {
        isStunned_ = stun;
    }

    CoordinateSystem GetCoordinates() {
        return position_;
    }

    void Transform(const Matrix44d& transformationMatrix) {
        mesh_->Transform(transformationMatrix);
    }
};

#endif // CHARACTER_HPP