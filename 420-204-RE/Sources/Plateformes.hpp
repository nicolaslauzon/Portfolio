///\file Plateformes.hpp

#ifndef PLATEFORMES_HPP
#define PLATEFORMES_HPP

#include "Objet3dColisionable.hpp"
#include "Mesh.hpp"
#include "ResourceManager.hpp"
#include "Matrix44d.hpp"
#include "Chrono.hpp"


///\class Plateforme
///\brief Plateforme 3d
class Plateforme : public Objet3dDrawable {  
public:


    Plateforme(TexturedMesh* mesh, const unsigned int& textureId) : Objet3dDrawable(mesh, textureId) {
        
    }

    virtual bool CollisionWithPlatform(Character* objet) {
        bool r = false;
        const double y = objet->GetSpeed()->y;
        double c;
        y > 0.0 ? c = 1.0 : c = -1.0;
        const Vector3d* result = mesh_->Collision(objet->GetPosition().GetOrigin(), {0.0, c, 0.0});
        if (result != nullptr) {
            // collision
            if (objet->GetJumpStatus()) {
                Vector3d speed = *objet->GetSpeed();
                speed.y = 0.0;
                objet->SetSpeed(speed);
                objet->SetAcceleration(Vector3d());
                objet->SetJump(false);
                const Vector3d tmp = Vector3d(0.0, 0.1, 0.0) + *result;
                const Vector3d translation = tmp - objet->GetPosition().GetOrigin();
                objet->SetPosition(tmp);
                objet->Translate(translation);
                r = true;
            }
            delete result;
        }
        return r;
    }


    virtual void HandleUpdate(double deltaTime) = 0;
};

class PlateformeStatique : public Plateforme {
public:
    PlateformeStatique(TexturedMesh* mesh, const unsigned int& textureId) : Plateforme(mesh, textureId) {}

    virtual void HandleUpdate(double deltaTime) {
        // V_O_I_D
    }
};


///\class PlateformePivotante
///\brief plateforme qui pivote
class PlateformePivotante : public Plateforme {
private:
    Vector3d rotationAxis_; ///< Axe de rotation
    double timeInterval_; ///< Temps pour chaque rotation complète
public:
    PlateformePivotante(TexturedMesh* mesh, const unsigned int& textureId) : Plateforme(mesh, textureId) {
        timeInterval_ = 0.0;
    }

    ///\brief Ajuste l'axe de rotation
    ///\param rotationAxis axe de rotation
    void SetRotationAxis(const Vector3d& rotationAxis) {
        rotationAxis_ = rotationAxis;
    }

    void SetTimeForEachRotation(double time) {
        timeInterval_ = time;
    }

    void HandleUpdate(double deltaTime) {
        const double angle = deltaTime / timeInterval_ * M_PI * 2.0;
        Matrix44d rotationMatrix;
        rotationMatrix.LoadIdentity();
        rotationMatrix.LoadRotation(rotationAxis_, angle);
        Transform(rotationMatrix);
    }
};


///\class PlateformeIntermittente
///\brief Plateforme qui disparait après contact
class PlateformeEffritable : public Plateforme {
private:
    double lifespan_;       ///< Temps entre le contact et la disparition
    Chrono afterContact_;   ///< Temps apres le contact
    bool visible_;          ///< Si la plateforme est presente
public:
    ///\brief Constructeur
    ///\param lifespan Temps entre le contact et la disparition
    ///\param mesh Modèle de la plateforme
    ///\param textureId Identifiant de la texture de la plateforme
    PlateformeEffritable(double lifespan, TexturedMesh* mesh, const unsigned int& textureId):Plateforme(mesh, textureId){
        lifespan_ = lifespan;
        visible_ = true;
     
    }

    ///\brief Collision avec la plateforme
    ///\param objet Objet avec lequel on teste la collision
    bool CollisionWithPlatform(Character* objet) {
        bool collision = false;
        if (visible_) {
            collision = Plateforme::CollisionWithPlatform(objet);
            if(collision && (afterContact_.GetElapsedSeconds() > lifespan_))
                afterContact_.Reset();
        }

        return collision;
    }
    ///\brief Affiche la plateforme
    void Draw() {
        if (visible_) {
            Plateforme::Draw();
        }
    }
    ///\brief Update la plateforme
    ///\param deltaTime Temps ecoule depuis le dernier appel
    void HandleUpdate(double deltaTime) {
        double chronoTime = afterContact_.GetElapsedSeconds();

        if (visible_ && (chronoTime > lifespan_) && (chronoTime < lifespan_ + 2))
            visible_ = false;
        
        if (!visible_ && (chronoTime > lifespan_ + 2))
            visible_ = true;
        
    }

};

///\class PlateformeIntermittente
///\brief objet 3d qui se dessine
class PlateformeIntermittente : public Plateforme {
protected:
    bool invisible_; ///< Plateforme n'est pas visible
    double duration_; ///< Durée pour laquelle la plateforme est visible
    double timeBuffer_; ///< Buffer de temps

public:

    //\brief Constructeur
    ///\param mesh Modèle de la plateforme
    ///\param textureId Identifiant de la texture de la plateforme
    PlateformeIntermittente(TexturedMesh* mesh, const unsigned int& textureId) : Plateforme(mesh, textureId) {
        bool invisible = false;
        duration_ = 1.5;
        timeBuffer_ = 0.0;
    }

    //\brief Update la plateforme
    ///\param deltaTime Temps ecoule depuis le dernier appel
    void HandleUpdate(double deltaTime) {
        timeBuffer_ += deltaTime;
        if (timeBuffer_ > duration_) {
            invisible_ = !invisible_;
            timeBuffer_ = 0.0;
        }

    }
    //\brief Affiche la plateforme
    void Draw() {
        if (!invisible_) {
            Plateforme::Draw();
        }
    }

    //\brief Collision avec la plateforme
    ///\param objet Objet avec lequel on teste la collision
    bool CollisionWithPlatform(Character* objet) {
        bool collision = false;
        if (!invisible_)
            collision = Plateforme::CollisionWithPlatform(objet);            
            
        return collision;
    }
};

///\class PlateformePiege
///\brief plateforme qui bloque le joueur
class PlateformePiege : public Plateforme{
private:
    Chrono elapsedTime_; ///< Temps écoulé depuis l'activation de la plateforme
    Chrono cooldown_; ///<Temps avant la réactivation de la plateforme
public:

    ///\brief Constructeur
    ///\param mesh Modèle de la plateforme
    ///\param textureId Identifiant de la texture de la plateforme
    PlateformePiege(TexturedMesh* mesh, const unsigned int& textureId) : Plateforme(mesh, textureId) {
        cooldown_.Reset();
    }
    ///\brief Fonction qui gère les collisions avec cette plateforme
    ///\param mesh Modèle de la plateforme
    ///\param textureId Identifiant de la texture de la plateforme
    bool CollisionWithPlatform(Character* objet){
        bool collision = Plateforme::CollisionWithPlatform(objet);
        if (collision && cooldown_.GetElapsedSeconds() > 3.5) {
            elapsedTime_.Reset();
            objet->Stun(true);
            cooldown_.Reset();
        }
        if (elapsedTime_.GetElapsedSeconds() > 2.0)
            objet->Stun(false);

        return collision;
    }

    ///\brief Fonction obligatoire
    ///\param deltaTime Temps depuis le dernier appel
    void HandleUpdate(double deltaTime) {}
};


class PlateformeMouvantes : public Plateforme {
private:
    double movemax_;
    Matrix44d translation_;
    Vector3d axe_;
    double movingtime;
    double speed_;
    Character* character_;
public:
    PlateformeMouvantes(TexturedMesh* mesh, const unsigned int& textureId, Vector3d axe, double movemax, double speed = 1.0) : Plateforme(mesh, textureId) {
        axe.Normalize();
        axe_ = axe;
        movemax_ = movemax;
        movingtime = 0;
        speed_ = speed;
        character_ = nullptr;
    }

    void SetCharacter(Character* character) {
        character_ = character;
    }


    void HandleUpdate(double deltatime) {

        movingtime += deltatime;
        translation_.LoadIdentity();

        if (movemax_ > movingtime *speed_)
            translation_.Translate(axe_ * deltatime * speed_);

        else if(movemax_*2 > movingtime * speed_)
            translation_.Translate(axe_ * -deltatime * speed_);

        if (movemax_ *2 < movingtime * speed_)
            movingtime = 0;
        
        mesh_->Transform(translation_);
        if (CollisionWithPlatform(character_))
            if (character_) {
                character_->Transform(translation_ );
                character_->SetOrigin(character_->GetPosition().GetOrigin() + translation_.GetTranslation());
            }
    }

};

class Labyrinthe : public Plateforme {
public:
    Labyrinthe() : Plateforme(ResourceManager::GetResource<TexturedMesh*>("labyrintheM"), *ResourceManager::GetResource<unsigned int*>("labyrinthe")) {

    }

//     bool CollisionWithPlatform(Character* objet) {
//         bool r = false;
//         std::tuple<Vector3d*, Vector3d> result = mesh_->CollisionReturnNormal(objet->GetPosition().GetOrigin(), objet->GetSpeed()->Normalized());
//         if (std::get<0>(result) != nullptr) {
//             // collision
//             // Set Speed
//             Vector3d speed = *objet->GetSpeed();
//             const double angle = speed.GetAngle(std::get<1>(result));
//             if (angle < 1e-5 || (M_PI-angle) < 1e-5 || (M_PI*2-angle) < 1e-5) {
//                 speed = Vector3d();
//             }
//             else {
//                 const double speedNorm = sin(angle) * speed.GetNorm();
                
//                 const Vector3d axis = speed % std::get<1>(result);
//                 Vector3d speedDirection = std::get<1>(result) % axis;
//                 if ((speedDirection * -1.0).GetAngle(speed) < (speedDirection).GetAngle(speed)) {
//                     speedDirection * -1.0;
//                 }
//                 speedDirection.Normalize();
//                 speed = speedDirection.Scaled(speedNorm);
//             }
//             objet->SetSpeed(speed);

//             // Set position
//             const Vector3d tmp = *std::get<0>(result);
//             const Vector3d translation = tmp - objet->GetPosition().GetOrigin();
//             objet->SetPosition(tmp);
//             objet->Translate(translation);
//             r = true;
//             delete std::get<0>(result);
//         }

//         // const Vector3d segment(2.0, 0.0, 0.0);
//         // const Vector3d offset(-1.0, 0.0, 0.0);
//         // std::tuple<Vector3d*, Vector3d> result = mesh_->CollisionReturnNormal(objet->GetPosition().GetOrigin() + offset, segment);
//         // if (std::get<0>(result) != nullptr) {
//         //     // collision
//         //     // Set Speed
//         //     Vector3d speed = *objet->GetSpeed();
//         //     const double angle = speed.GetAngle(std::get<1>(result));
//         //     if (angle < 1e-5 || (M_PI-angle) < 1e-5 || (M_PI*2-angle) < 1e-5) {
//         //         speed = Vector3d();
//         //     }
//         //     else {
//         //         const double speedNorm = sin(angle) * speed.GetNorm();
                
//         //         const Vector3d axis = speed % std::get<1>(result);
//         //         Vector3d speedDirection = std::get<1>(result) % axis;
//         //         if ((speedDirection * -1.0).GetAngle(speed) < (speedDirection).GetAngle(speed)) {
//         //             speedDirection * -1.0;
//         //         }
//         //         speedDirection.Normalize();
//         //         speed = speedDirection.Scaled(speedNorm);
//         //     }
//         //     objet->SetSpeed(speed);

//         //     // Set position
//         //     const Vector3d tmp = *std::get<0>(result) - offset;
//         //     const Vector3d translation = tmp - objet->GetPosition().GetOrigin();
//         //     objet->SetPosition(tmp);
//         //     objet->Translate(translation);
//         //     r = true;
//         //     delete std::get<0>(result);
//         // }

//         // const Vector3d segment1(0.0, 0.0, 2.0);
//         // const Vector3d offset1(0.0, 0.0, -1.0);
//         // result = mesh_->CollisionReturnNormal(objet->GetPosition().GetOrigin() + offset1, segment1);
//         // if (std::get<0>(result) != nullptr) {
//         //     // collision
//         //     // Set Speed
//         //     Vector3d speed = *objet->GetSpeed();
//         //     const double angle = speed.GetAngle(std::get<1>(result));
//         //     if (angle < 1e-5 || (M_PI-angle) < 1e-5 || (M_PI*2-angle) < 1e-5) {
//         //         speed = Vector3d();
//         //     }
//         //     else {
//         //         const double speedNorm = sin(angle) * speed.GetNorm();
                
//         //         const Vector3d axis = speed % std::get<1>(result);
//         //         Vector3d speedDirection = std::get<1>(result) % axis;
//         //         if ((speedDirection * -1.0).GetAngle(speed) < (speedDirection).GetAngle(speed)) {
//         //             speedDirection * -1.0;
//         //         }
//         //         speedDirection.Normalize();
//         //         speed = speedDirection.Scaled(speedNorm);
//         //     }
//         //     objet->SetSpeed(speed);

//         //     // Set Position
//         //     const Vector3d tmp = *std::get<0>(result) - offset1;
//         //     const Vector3d translation = tmp - objet->GetPosition().GetOrigin();
//         //     objet->SetPosition(tmp);
//         //     objet->Translate(translation);
//         //     r = true;
//         //     delete std::get<0>(result);
//         // }

//         r = Plateforme::CollisionWithPlatform(objet);
//         return r;
//    }







    bool CollisionWithPlatform(Character* objet) {
        bool r = false;


        std::tuple<Vector3d*, Vector3d> result = mesh_->CollisionReturnNormal(objet->GetPosition().GetOrigin(), objet->GetSpeed()->Normalized());
        if (std::get<0>(result) != nullptr) {
            // speed
            Vector3d speed = *objet->GetSpeed();
            objet->SetSpeed(speed - speed.Projected(std::get<1>(result)));

            // position
            Matrix44d m;
            m.LoadIdentity();
            m.Translate(*std::get<0>(result) - (objet->GetPosition().GetOrigin()));
            objet->Transform(m);
            objet->SetPosition(objet->GetPosition().GetOrigin() + m.GetTranslation());

            r = true;
        }
        // p1
        // Vector3d point1 = Vector3d(1.0, 0.0, 0.0);
        // std::tuple<Vector3d*, Vector3d> result = mesh_->CollisionReturnNormal(objet->GetPosition().GetOrigin() + point1, objet->GetSpeed()->Normalized());
        // if (std::get<0>(result) != nullptr) {
        //     // speed
        //     Vector3d speed = *objet->GetSpeed();
        //     objet->SetSpeed(speed - speed.Projected(std::get<1>(result)));

        //     // position
        //     Matrix44d m;
        //     m.LoadIdentity();
        //     m.Translate(*std::get<0>(result) - (point1 + objet->GetPosition().GetOrigin()));
        //     objet->Transform(m);
        //     objet->SetPosition(objet->GetPosition().GetOrigin() + m.GetTranslation());

        //     r = true;
        // }

        // // p2
        // Vector3d point2 = Vector3d(-1.0, 0.0, 0.0);
        // std::tuple<Vector3d*, Vector3d> result1 = mesh_->CollisionReturnNormal(objet->GetPosition().GetOrigin() + point2, objet->GetSpeed()->Normalized());
        // if (std::get<0>(result1) != nullptr && r == false) {
        //     // speed
        //     Vector3d speed = *objet->GetSpeed();
        //     objet->SetSpeed(speed - speed.Projected(std::get<1>(result1)));

        //     // position
        //     Matrix44d m;
        //     m.LoadIdentity();
        //     m.Translate(*std::get<0>(result1) - (point1 + objet->GetPosition().GetOrigin()));
        //     objet->Transform(m);
        //     objet->SetPosition(objet->GetPosition().GetOrigin() + m.GetTranslation());

        //     r = true;
        // }

        // // p3
        // Vector3d point3 = Vector3d(1.0, 0.0, 0.0);
        // std::tuple<Vector3d*, Vector3d> result2 = mesh_->CollisionReturnNormal(objet->GetPosition().GetOrigin() + point3, objet->GetSpeed()->Normalized());
        // if (std::get<0>(result2) != nullptr && r == false) {
        //     // speed
        //     Vector3d speed = *objet->GetSpeed();
        //     objet->SetSpeed(speed - speed.Projected(std::get<1>(result2)));

        //     // position
        //     Matrix44d m;
        //     m.LoadIdentity();
        //     m.Translate(*std::get<0>(result2) - (point3 + objet->GetPosition().GetOrigin()));
        //     objet->Transform(m);
        //     objet->SetPosition(objet->GetPosition().GetOrigin() + m.GetTranslation());

        //     r = true;
        // }

        // // p4
        // Vector3d point4 = Vector3d(-1.0, 0.0, 0.0);
        // std::tuple<Vector3d*, Vector3d> result3 = mesh_->CollisionReturnNormal(objet->GetPosition().GetOrigin() + point4, objet->GetSpeed()->Normalized());
        // if (std::get<0>(result3) != nullptr && r == false) {
        //     // speed
        //     Vector3d speed = *objet->GetSpeed();
        //     objet->SetSpeed(speed - speed.Projected(std::get<1>(result3)));

        //     // position
        //     Matrix44d m;
        //     m.LoadIdentity();
        //     m.Translate(*std::get<0>(result3) - (point4 + objet->GetPosition().GetOrigin()));
        //     objet->Transform(m);
        //     objet->SetPosition(objet->GetPosition().GetOrigin() + m.GetTranslation());

        //     r = true;
        // }

        r = Plateforme::CollisionWithPlatform(objet);
        return r;
    }



    virtual void HandleUpdate(double deltaTime) {
        // V_O_I_D
    }
};

//\class Trampoline
///\brief Trampoline
class Trampoline : public Plateforme {
protected:
   

public:

    //\brief Constructeur
    ///\param mesh Modèle de la trampoline
    ///\param textureId Identifiant de la texture de la trampoline
   Trampoline(TexturedMesh* mesh, const unsigned int& textureId) : Plateforme(mesh, textureId) {}

    //\brief Update la trampoline
    ///\param deltaTime Temps ecoule depuis le dernier appel
    void HandleUpdate(double deltaTime) {}


    //\brief Collision avec la trampoline
    ///\param objet Objet avec lequel on teste la collision
    bool CollisionWithPlatform(Character* objet) {
        bool collision = Plateforme::CollisionWithPlatform(objet);
        if (collision) {
            objet->SetSpeed({ objet->GetSpeed()->x, objet->GetSpeed()->y + 100, objet->GetSpeed()->z });
        }

        return collision;
    }
};

#endif // PLATEFORMES_HPP