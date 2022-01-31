
///\file Character.hpp

#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Observer.hpp"
#include "Mesh.hpp"
#include "TransformInterface.hpp"
#include "Objet3dDrawable.hpp"
#include "Chrono.hpp"
#include "Physic.hpp"
#include "Sprite3dDrawable.hpp"
#include "Effect.hpp"
#include <SDL2/SDL.h>

#define MAXSPEED 20.0
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
class Character : public Sprite3Ddrawable, public Observer<SDL_Event>
{
private:
    TexturedMesh* shell_;    ///< carapace du personnage
    Chrono time;             ///< Temps pour la fonction Jump
    bool jumping;            ///< Booleen pour savoir si on saute
    double spacePressedTime; ///< Temps que la touche espace a ete pressee
    bool reset_;             ///< Booleen pour savoir si la touche espace a été relevé
    double animtime_;
    double angle_;           ///< Angle de rotation
    double maxAccel_;        ///< Accélération maximale
    bool isStunned_;         ///< Booleen pour savoir si le personnage est immobilisé
    bool idle;               ///< Booleen pour savoir si aucune touches est active. 
    Vector3d previousPos_;
    Vector3d speed_;         ///< Vecteur de vitesse du personnage
    Vector3d acceleration_;
    Vector3d direction;
    bool movDir_[4] = {false, false, false, false}; ///< Tableau qui retiens quelles touches sont enfoncées

public:
    Character(Sprite3D* sprite, const unsigned int& textureId) : Sprite3Ddrawable(sprite, textureId) {
        direction = { 0.0,0.0,0.0 };
        time.Reset();
        jumping = false;
        animtime_ = 0;
        position_ = CoordinateSystem();
        spacePressedTime = 0.0;
        maxAccel_ = 100.0;
        reset_ = false;
        previousPos_ = Vector3d();
        isStunned_ = false;
        shell_ = ResourceManager::GetResource<TexturedMesh*>("bugModel");
        shell_->setTexture(*ResourceManager::GetResource<unsigned int*>("shell"));
    }

    ///\brief Update le personnage
    ///\param deltaT temps écoulé depuis la dernière update
    void HandleUpdate(double deltaT) {
        UpdateFrame(deltaT);

        if (angle_) {
            Matrix44d rotMatrix;
            Matrix44d tMatrix;

            tMatrix.LoadIdentity();
            tMatrix.Translate({ -position_.GetOrigin().x,-position_.GetOrigin().y ,-position_.GetOrigin().z });
            rotMatrix.LoadIdentity();

            rotMatrix.LoadRotation({ 0.0,1.0,0.0 }, angle_);
            position_.SetXDirection(rotMatrix * position_.GetXDirection());
            position_.SetZDirection(position_.GetXDirection() % Vector3d(0.0, 1.0, 0.0));
            position_.SetZDirection(position_.GetZDirection().Normalized());

            sprite_->Transform(tMatrix);
            sprite_->Transform(rotMatrix);
            shell_->Transform(tMatrix);
            shell_->Transform(rotMatrix);

            tMatrix.LoadIdentity();
            tMatrix.Translate({ position_.GetOrigin().x,position_.GetOrigin().y ,position_.GetOrigin().z });
            sprite_->Transform(tMatrix);
            shell_->Transform(tMatrix);

            angle_ = 0.0;
        }
       



        acceleration_ = Vector3d();


        // if (position_.GetOrigin().y <= -10.0) {
        //     jumping = false;
        //     speed_.y = 0.0;
        //     acceleration_.y = 0.0;
        //     position_.SetOrigin(Vector3d(position_.GetOrigin().x, position_.GetOrigin().y+1.0, position_.GetOrigin().z));
        // }

        previousPos_ = position_.GetOrigin();

        if (movDir_[W]) {
            direction = position_.GetZDirection() * -25.0;
        }

        if (movDir_[D]) {
            direction = position_.GetXDirection() * 25.0;

        }
        if (movDir_[S]) {
            direction = position_.GetZDirection() * 25.0;

        }

        if (movDir_[A]) {
            direction = position_.GetXDirection() * -25.0;

        }



        if (movDir_[W] == false && movDir_[S] == false && speed_.x != 0.0)
        {
            direction = direction + Vector3d(speed_.x * -1.0 / abs(speed_.x), 0.0, 0.0);
        }
        if (movDir_[D] == false && movDir_[A] == false && speed_.z != 0.0) {
            direction = direction + Vector3d(0.0, 0.0, speed_.z * -1.0 / abs(speed_.z));
        }
        if (movDir_[W] == false && movDir_[A] == false && movDir_[S] == false && movDir_[D] == false && speed_.GetNorm() <= 1.0) {
            speed_ = Vector3d();
        }



        direction.Normalize();

        acceleration_ = acceleration_ + Vector3d(direction, maxAccel_);

        speed_ = Physic::UpdateSpeed(speed_, acceleration_, deltaT);
        if (speed_.GetNorm() > MAXSPEED + 1.0)
        {
            speed_.Normalize();
            speed_ = speed_ * MAXSPEED;
        }
        
        if (EffectManager::EffectExists(MOVESPEED)) {
            speed_.x = speed_.x * EffectManager::GetValue(MOVESPEED);
            speed_.z = speed_.z * EffectManager::GetValue(MOVESPEED);
        }

        position_.SetOrigin(Physic::UpdatePosition(position_.GetOrigin(), speed_, acceleration_, deltaT));

        // #ifdef DEBUG
                // cout << "X ->"
                //     << "[" << position_.GetOrigin().x << "]\t";
                // cout << "Y ->"
                //     << "[" << position_.GetOrigin().y << "]\t";
                // cout << "Z ->"
                //     << "[" << position_.GetOrigin().z << "]" << endl;
                //cout << "X ->"
                //    << "[" << speed_.x << "]\t";
                //cout << "Y ->"
                //    << "[" << speed_.y << "]\t";
                //cout << "Z ->"
                //    << "[" << speed_.z << "]" << endl;
        // #endif

        if (jumping) {

            acceleration_ = Vector3d({ 0.0,-1.0,0.0 }, maxAccel_);

            speed_ = Physic::UpdateSpeed(speed_, (acceleration_), deltaT);


            position_.SetOrigin(Physic::UpdatePosition(position_.GetOrigin(), speed_, acceleration_, deltaT));


            //Quand le personnage retouche le sol remmettre le booléen a false
        }

        Vector3d translation(position_.GetOrigin() - previousPos_);
        if (translation.x != 0.0 || translation.y != 0.0 || translation.z != 0.0) {
            sprite_->SetDelay(0.003 / (std::abs(translation.x) + std::abs(translation.z)));
            Matrix44d matrix;
            matrix.LoadIdentity();
            matrix.Translate(translation);
            Transform(matrix);

        }
        else {
            sprite_->SetDelay(0.0);
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
            speed_.y = (((0.43 - 0.38) / 0.30) * 105);
        }
        else if (timePressed >= 0.20 && timePressed < 0.70)
        {
            speed_.y = (((0.43 - 0.34) / 0.40) * 110);
        }
        else
        {
            speed_.y = (((0.43 - 0.29) / 0.50) * 115);
            
        }

        if (EffectManager::EffectExists(JUMP)) {
             speed_.y = speed_.y * EffectManager::GetValue(JUMP);
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

    void SetSpeed(const Vector3d& speed) {
        speed_ = speed;
    }

    void SetAcceleration(const Vector3d& a) {
        acceleration_ = a;
    }
    
    Vector3d* GetAcceleration() {
        return &acceleration_;
    }
      inline void SetMaxAcceleration(double acc) {
        maxAccel_ = acc;
    }
    
    void SetJump(bool status) {
        jumping = status;
    }

    bool GetJumpStatus() {
        return jumping;
    }

    void SetPosition(const Vector3d& position) {
        position_.SetOrigin(position);
    }

    void Notification(SDL_Event event) {
        switch (event.type) {
        case SDL_MOUSEMOTION:
            angle_ = (double)event.motion.xrel * M_PI / 180.0;
            break;

        case 771:
            switch (event.key.state) {
            case SDLK_w:
                if (!isStunned_)
                    movDir_[W] = true;
                break;

            case SDLK_a:
                if (!isStunned_)
                    movDir_[A] = true;
                break;

            case SDLK_s:
                if (!isStunned_)
                    movDir_[S] = true;
                break;

            case SDLK_d:
                if (!isStunned_)
                    movDir_[D] = true;
                break;

            case SDLK_SPACE:
                if (!jumping && !isStunned_)
                    if (!reset_) {
                        time.Reset();
                        reset_ = true;
                    }
                break;
            }        
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
            case SDLK_w:
                movDir_[W] = false;
                break;

            case SDLK_a:
                movDir_[A] = false;
                break;

            case SDLK_s:
                movDir_[S] = false;
                break;

            case SDLK_d:
                movDir_[D] = false;
                break;

            case SDLK_SPACE:
                if (speed_.y < 1e-6) {
                    spacePressedTime = time.GetElapsedSeconds();
                    reset_ = false;
                    Jump(spacePressedTime);
                }
                break;
            }
            break;
        break;
        }
    }

    void Stun(bool stun) {
        isStunned_ = stun;
    }

    CoordinateSystem GetCoordinates() {
        return position_;
    }

    void Transform(const Matrix44d& transformationMatrix) {
        sprite_->Transform(transformationMatrix);
        shell_->Transform(transformationMatrix);
    }


    virtual void Draw() {
        shell_->Draw();
        sprite_->Draw();
    }
};

#endif // CHARACTER_HPP
// ///\file Character.hpp

// #ifndef CHARACTER_HPP
// #define CHARACTER_HPP

// #include "Observer.hpp"
// #include "Mesh.hpp"
// #include "TransformInterface.hpp"
// #include "Objet3dDrawable.hpp"
// #include "Chrono.hpp"
// #include "Physic.hpp"
// #include "Sprite3dDrawable.hpp"
// #include "Effect.hpp"
// #include <SDL2/SDL.h>

// #define MAXSPEED 20.0
// #define W 0
// #define D 1
// #define S 2
// #define A 3

// ///\brief Statut du personnage
// // enum CharacterStatus {
// //     Static = 1,
// //     Walking = 2,
// //     Jumping = 3
// // };

// ///\class Character
// ///\brief Personnage du jeu
// class Character : public Sprite3Ddrawable, public Observer<SDL_Event>
// {
// private:
//     TexturedMesh* shell_;    ///< carapace du personnage
//     Chrono time;             ///< Temps pour la fonction Jump
//     bool jumping;            ///< Booleen pour savoir si on saute
//     double spacePressedTime; ///< Temps que la touche espace a ete pressee
//     bool reset_;             ///< Booleen pour savoir si la touche espace a été relevé
//     double animtime_;
//     double angle_;           ///< Angle de rotation
//     double maxAccel_;        ///< Accélération maximale
//     bool isStunned_;         ///< Booleen pour savoir si le personnage est immobilisé
//     bool idle;               ///< Booleen pour savoir si aucune touches est active. 
//     Vector3d previousPos_;
//     Vector3d speed_;         ///< Vecteur de vitesse du personnage
//     Vector3d acceleration_;
//     Vector3d direction;
//     bool movDir_[4] = {false, false, false, false}; ///< Tableau qui retiens quelles touches sont enfoncées

// public:
//     Character(Sprite3D* sprite, const unsigned int& textureId) : Sprite3Ddrawable(sprite, textureId) {
//         direction = { 0.0,0.0,0.0 };
//         time.Reset();
//         jumping = false;
//         animtime_ = 0;
//         position_ = CoordinateSystem();
//         spacePressedTime = 0.0;
//         maxAccel_ = 100.0;
//         reset_ = false;
//         previousPos_ = Vector3d();
//         isStunned_ = false;
//         shell_ = ResourceManager::GetResource<TexturedMesh*>("bugModel");
//         shell_->setTexture(*ResourceManager::GetResource<unsigned int*>("shell"));
//     }

//     ///\brief Update le personnage
//     ///\param deltaT temps écoulé depuis la dernière update
//     void HandleUpdate(double deltaT) {
//         UpdateFrame(deltaT);

//         if (angle_) {
//             Matrix44d rotMatrix;
//             Matrix44d tMatrix;

//             tMatrix.LoadIdentity();
//             tMatrix.Translate({ -position_.GetOrigin().x,-position_.GetOrigin().y ,-position_.GetOrigin().z });
//             rotMatrix.LoadIdentity();

//             rotMatrix.LoadRotation({ 0.0,1.0,0.0 }, angle_);
//             position_.SetXDirection(rotMatrix * position_.GetXDirection());
//             position_.SetZDirection(position_.GetXDirection() % Vector3d(0.0, 1.0, 0.0));
//             position_.SetZDirection(position_.GetZDirection().Normalized());

//             sprite_->Transform(tMatrix);
//             sprite_->Transform(rotMatrix);
//             shell_->Transform(tMatrix);
//             shell_->Transform(rotMatrix);

//             tMatrix.LoadIdentity();
//             tMatrix.Translate({ position_.GetOrigin().x,position_.GetOrigin().y ,position_.GetOrigin().z });
//             sprite_->Transform(tMatrix);
//             shell_->Transform(tMatrix);

//             angle_ = 0.0;
//         }
       



//         acceleration_ = Vector3d();


//         // if (position_.GetOrigin().y <= -10.0) {
//         //     jumping = false;
//         //     speed_.y = 0.0;
//         //     acceleration_.y = 0.0;
//         //     position_.SetOrigin(Vector3d(position_.GetOrigin().x, position_.GetOrigin().y+1.0, position_.GetOrigin().z));
//         // }

//         previousPos_ = position_.GetOrigin();

//         if (movDir_[W]) {
//             direction = position_.GetZDirection() * -25.0;
//         }

//         if (movDir_[D]) {
//             direction = position_.GetXDirection() * 25.0;

//         }
//         if (movDir_[S]) {
//             direction = position_.GetZDirection() * 25.0;

//         }

//         if (movDir_[A]) {
//             direction = position_.GetXDirection() * -25.0;

//         }



//         if (movDir_[W] == false && movDir_[S] == false && speed_.x != 0.0)
//         {
//             direction = direction + Vector3d(speed_.x * -1.0 / abs(speed_.x), 0.0, 0.0);
//         }
//         if (movDir_[D] == false && movDir_[A] == false && speed_.z != 0.0) {
//             direction = direction + Vector3d(0.0, 0.0, speed_.z * -1.0 / abs(speed_.z));
//         }
//         if (movDir_[W] == false && movDir_[A] == false && movDir_[S] == false && movDir_[D] == false && speed_.GetNorm() <= 1.0) {
//             speed_ = Vector3d();
//         }



//         direction.Normalize();

//         acceleration_ = acceleration_ + Vector3d(direction, maxAccel_);

//         speed_ = Physic::UpdateSpeed(speed_, acceleration_, deltaT);
//         if (speed_.GetNorm() > MAXSPEED + 1.0)
//         {
//             speed_.Normalize();
//             speed_ = speed_ * MAXSPEED;
//         }
        
//         if (EffectManager::EffectExists(MOVESPEED)) {
//             speed_.x = speed_.x * EffectManager::GetValue(MOVESPEED);
//             speed_.z = speed_.z * EffectManager::GetValue(MOVESPEED);
//         }

//         position_.SetOrigin(Physic::UpdatePosition(position_.GetOrigin(), speed_, acceleration_, deltaT));

//         // #ifdef DEBUG
//                 // cout << "X ->"
//                 //     << "[" << position_.GetOrigin().x << "]\t";
//                 // cout << "Y ->"
//                 //     << "[" << position_.GetOrigin().y << "]\t";
//                 // cout << "Z ->"
//                 //     << "[" << position_.GetOrigin().z << "]" << endl;
//                 //cout << "X ->"
//                 //    << "[" << speed_.x << "]\t";
//                 //cout << "Y ->"
//                 //    << "[" << speed_.y << "]\t";
//                 //cout << "Z ->"
//                 //    << "[" << speed_.z << "]" << endl;
//         // #endif

//         if (jumping) {

//             acceleration_ = Vector3d({ 0.0,-1.0,0.0 }, maxAccel_);

//             speed_ = Physic::UpdateSpeed(speed_, (acceleration_), deltaT);


//             position_.SetOrigin(Physic::UpdatePosition(position_.GetOrigin(), speed_, acceleration_, deltaT));


//             //Quand le personnage retouche le sol remmettre le booléen a false
//         }

//         Vector3d translation(position_.GetOrigin() - previousPos_);
//         if (translation.x != 0.0 || translation.y != 0.0 || translation.z != 0.0) {
//             sprite_->SetDelay(0.003 / (std::abs(translation.x) + std::abs(translation.z)));
//             Matrix44d matrix;
//             matrix.LoadIdentity();
//             matrix.Translate(translation);
//             Transform(matrix);

//         }
//         else {
//             sprite_->SetDelay(0.0);
//         }

//     }
//     // void HandleUpdate(double deltaT) {
//     //     UpdateFrame(deltaT);

//     //     if (angle_) {
//     //         Matrix44d rotMatrix;
//     //         Matrix44d tMatrix;

//     //         tMatrix.LoadIdentity();
//     //         tMatrix.Translate({ -position_.GetOrigin().x,-position_.GetOrigin().y ,-position_.GetOrigin().z });
//     //         rotMatrix.LoadIdentity();

//     //         rotMatrix.LoadRotation({ 0.0,1.0,0.0 }, angle_);
//     //         position_.SetXDirection(rotMatrix * position_.GetXDirection());
//     //         position_.SetZDirection(position_.GetXDirection() % Vector3d(0.0, 1.0, 0.0));
//     //         position_.SetZDirection(position_.GetZDirection().Normalized());

//     //         sprite_->Transform(tMatrix);
//     //         sprite_->Transform(rotMatrix);
//     //         shell_->Transform(tMatrix);
//     //         shell_->Transform(rotMatrix);

//     //         tMatrix.LoadIdentity();
//     //         tMatrix.Translate({ position_.GetOrigin().x,position_.GetOrigin().y ,position_.GetOrigin().z });
//     //         sprite_->Transform(tMatrix);
//     //         shell_->Transform(tMatrix);

//     //         angle_ = 0.0;
//     //     }
       



//     //     acceleration_ = Vector3d();


         

//     //     previousPos_ = position_.GetOrigin();

//     //     if (movDir_[W]) {
//     //         direction = position_.GetZDirection() * -25.0;
//     //     }

//     //     if (movDir_[D]) {
//     //         direction = position_.GetXDirection() * 25.0;

//     //     }
//     //     if (movDir_[S]) {
//     //         direction = position_.GetZDirection() * 25.0;

//     //     }

//     //     if (movDir_[A]) {
//     //         direction = position_.GetXDirection() * -25.0;

//     //     }



//     //     if (movDir_[W] == false && movDir_[S] == false && speed_.x != 0.0)
//     //     {
//     //         direction = direction + Vector3d(speed_.x * -1.0 / abs(speed_.x), 0.0, 0.0);
//     //     }
//     //     if (movDir_[D] == false && movDir_[A] == false && speed_.z != 0.0) {
//     //         direction = direction + Vector3d(0.0, 0.0, speed_.z * -1.0 / abs(speed_.z));
//     //     }
//     //     if (movDir_[W] == false && movDir_[A] == false && movDir_[S] == false && movDir_[D] == false && speed_.GetNorm() <= 1.0) {
//     //         speed_ = Vector3d();
//     //     }



//     //     direction.Normalize();

//     //     acceleration_ = acceleration_ + Vector3d(direction, maxAccel_);

//     //     speed_ = Physic::UpdateSpeed(speed_, acceleration_, deltaT);
//     //     if (speed_.GetNorm() > MAXSPEED + 1.0)
//     //     {
//     //         speed_.Normalize();
//     //         speed_ = speed_ * MAXSPEED;
//     //     }
        
//     //     if (EffectManager::EffectExists(MOVESPEED)) {
//     //         speed_.x = speed_.x * EffectManager::GetValue(MOVESPEED);
//     //         speed_.z = speed_.z * EffectManager::GetValue(MOVESPEED);
//     //     }

//     //     position_.SetOrigin(Physic::UpdatePosition(position_.GetOrigin(), speed_, acceleration_, deltaT));

//     //     // #ifdef DEBUG
//     //             // cout << "X ->"
//     //             //     << "[" << position_.GetOrigin().x << "]\t";
//     //             // cout << "Y ->"
//     //             //     << "[" << position_.GetOrigin().y << "]\t";
//     //             // cout << "Z ->"
//     //             //     << "[" << position_.GetOrigin().z << "]" << endl;
//     //             //cout << "X ->"
//     //             //    << "[" << speed_.x << "]\t";
//     //             //cout << "Y ->"
//     //             //    << "[" << speed_.y << "]\t";
//     //             //cout << "Z ->"
//     //             //    << "[" << speed_.z << "]" << endl;
//     //     // #endif

//     //     if (jumping) {

//     //         acceleration_ = Vector3d({ 0.0,-1.0,0.0 }, maxAccel_);

//     //         speed_ = Physic::UpdateSpeed(speed_, (acceleration_), deltaT);


//     //         position_.SetOrigin(Physic::UpdatePosition(position_.GetOrigin(), speed_, acceleration_, deltaT));

//     //         // if (position_.GetOrigin().y <= -40.0) {
//     //             // jumping = false;
//     //             // speed_.y = 0.0;
//     //             // acceleration_.y = 0.0;
//     //             // position_.SetOrigin(Vector3d(position_.GetOrigin().x, position_.GetOrigin().y + 1.0, position_.GetOrigin().z));
//     //         // }
//     //         //Quand le personnage retouche le sol remmettre le booléen a false
//     //     }

//     //     Vector3d translation(position_.GetOrigin() - previousPos_);
//     //     if (translation.x != 0.0 || translation.y != 0.0 || translation.z != 0.0) {
//     //         sprite_->SetDelay(0.003 / (std::abs(translation.x) + std::abs(translation.z)));
//     //         Matrix44d matrix;
//     //         matrix.LoadIdentity();
//     //         matrix.Translate(translation);
//     //         Transform(matrix);

//     //     }
//     //     else {
//     //         sprite_->SetDelay(0.0);
//     //     }

//     // }

//     ///\brief Fonction qui fait sauter le personnage
//     ///\param characterStatus Statut actuel du caractère
//     ///\param time Temps à l'appel de la fonction Jump
//     void Jump(double timePressed)
//     {
//         jumping = true;

//         if (timePressed > 0.00 && timePressed < 0.20)
//         {
//             speed_.y = (((0.43 - 0.38) / 0.30) * 105);
//         }
//         else if (timePressed >= 0.20 && timePressed < 0.70)
//         {
//             speed_.y = (((0.43 - 0.34) / 0.40) * 110);
//         }
//         else
//         {
//             speed_.y = (((0.43 - 0.29) / 0.50) * 115);
            
//         }

//         if (EffectManager::EffectExists(JUMP)) {
//              speed_.y = speed_.y * EffectManager::GetValue(JUMP);
//         }
        
//     }

//     Vector3d* GetSpeed() {
//         return &speed_;
//     }

//     inline double GetRadius() {
//         return GetRayon();
//     }

//     inline CoordinateSystem GetPosition() {
//         return position_;
//     }

//     void SetSpeed(const Vector3d& speed) {
//         speed_ = speed;
//     }

//     void SetAcceleration(const Vector3d& a) {
//         acceleration_ = a;
//     }
    
//     Vector3d* GetAcceleration() {
//         return &acceleration_;
//     }
//       inline void SetMaxAcceleration(double acc) {
//         maxAccel_ = acc;
//     }
    
//     void SetJump(bool status) {
//         jumping = status;
//     }

//     bool GetJumpStatus() {
//         return jumping;
//     }

//     void SetPosition(const Vector3d& position) {
//         position_.SetOrigin(position);
//     }

//     void Notification(SDL_Event event) {
//         switch (event.type) {
//         case SDL_MOUSEMOTION:
//             angle_ = (double)event.motion.xrel * M_PI / 180.0;
//             break;

//         case 771:
//             switch (event.key.state) {
//             case SDLK_w:
//                 if (!isStunned_)
//                     movDir_[W] = true;
//                 break;

//             case SDLK_a:
//                 if (!isStunned_)
//                     movDir_[A] = true;
//                 break;

//             case SDLK_s:
//                 if (!isStunned_)
//                     movDir_[S] = true;
//                 break;

//             case SDLK_d:
//                 if (!isStunned_)
//                     movDir_[D] = true;
//                 break;

//             case SDLK_SPACE:
//                 if (!jumping && !isStunned_)
//                     if (!reset_) {
//                         time.Reset();
//                         reset_ = true;
//                     }
//                 break;
//             }        
//             break;

//         case SDL_KEYUP:
//             switch (event.key.keysym.sym) {
//             case SDLK_w:
//                 movDir_[W] = false;
//                 break;

//             case SDLK_a:
//                 movDir_[A] = false;
//                 break;

//             case SDLK_s:
//                 movDir_[S] = false;
//                 break;

//             case SDLK_d:
//                 movDir_[D] = false;
//                 break;

//             case SDLK_SPACE:
//                 if(!jumping) {
//                 spacePressedTime = time.GetElapsedSeconds();
//                 reset_ = false;
//                 Jump(spacePressedTime);
//                 }
//                 break;
//             }
//             break;
//         break;
//         }
//     }

//     void Stun(bool stun) {
//         isStunned_ = stun;
//     }

//     CoordinateSystem GetCoordinates() {
//         return position_;
//     }

//     void Transform(const Matrix44d& transformationMatrix) {
//         sprite_->Transform(transformationMatrix);
//         shell_->Transform(transformationMatrix);
//     }


//     virtual void Draw() {
//         shell_->Draw();
//         sprite_->Draw();
//     }
// };

// #endif // CHARACTER_HPP
