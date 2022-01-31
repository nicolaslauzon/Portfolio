///\file Capsules.hpp

#ifndef CAPSULES_HPP
#define CAPSULES_HPP

#include "Objet3dColisionable.hpp"
#include "Mesh.hpp"
#include "ResourceManager.hpp"
#include "Matrix44d.hpp"
#include "Character.hpp"
#include "Effect.hpp"


///\class Capsule
///\brief Capsule modèle
class Capsule : public Objet3dColisionable {

public:
    ///\brief Constructeur
    ///\param mesh Modèle de la capsule
    ///\param textureId Identifiant de la texture de la capsule
    Capsule(TexturedMesh* mesh, const unsigned int& textureId) : Objet3dColisionable(mesh, textureId) {

    }
    virtual void Collision(Character* objet) = 0;

};

///\class TeleportationCapsule
///\brief Capsule de téléportation
class TeleportationCapsule : public Capsule {
private:
    Vector3d teleportationCoordinates_; ///< Coordonées auxquelles téléporter la cible
public:
    ///\brief Constructeur
    ///\param mesh Modèle de la capsule
    ///\param textureId Identifiant de la texture de la capsule
    ///\param teleportationCoordinates Coordonées auxquelles téléporter la cible
    TeleportationCapsule(TexturedMesh* mesh, const unsigned int& textureId, Vector3d teleportationCoordinates) : Capsule(mesh, textureId) {
        teleportationCoordinates_ = teleportationCoordinates;
    }

    ///\brief Teste et gère les collisions
    ///\param objet personnage avec lequel la collision est testée
    void Collision(Character* objet) {
        if (Physic::BoundingSphereCollisionTest(objet->GetRayon(), GetRayon(), objet->GetPosition(), GetPosition())) {
            objet->Translate(teleportationCoordinates_ - objet->GetPosition().GetOrigin());
            objet->SetOrigin(teleportationCoordinates_);
        }
    }
};

///\class SpeedCapsule
///\brief Capsule de vitesse
class SpeedCapsule : public Capsule {
private:
    double duration_;   ///< Durée de l'effet
    double multiplier_; ///< Valeur du bonus
public:
    ///\brief Constructeur
    ///\param mesh Modèle de la capsule
    ///\param textureId Identifiant de la texture de la capsule
    ///\param duration Durée de l'effet
    ///\param multiplier Valeur du bonus
    SpeedCapsule(TexturedMesh* mesh, const unsigned int& textureId, double duration, double multiplier) : Capsule(mesh, textureId) {
        multiplier_ = multiplier;
        duration_ = duration;
    }

    ///\brief Teste et gère les collisions
    ///\param objet personnage avec lequel la collision est testée
    void Collision(Character* objet) {
        if (Physic::BoundingSphereCollisionTest(objet->GetRayon(), this->GetRayon(), objet->GetPosition(), this->GetPosition())) {
            if (!EffectManager::EffectExists(MOVESPEED))
                EffectManager::AddEffect(MOVESPEED, new Effect(duration_, multiplier_));
            else
                EffectManager::Refresh(MOVESPEED, duration_, multiplier_);
        }
    }
};

///\class JumpCapsule
///\brief Capsule de saut
class JumpCapsule : public Capsule {
private:
    double duration_;   ///< Durée de l'effet
    double multiplier_; ///< Valeur du bonus
public:
    ///\brief Constructeur
    ///\param mesh Modèle de la capsule
    ///\param textureId Identifiant de la texture de la capsule
    ///\param duration Durée de l'effet
    ///\param multiplier Valeur du bonus
    JumpCapsule(TexturedMesh* mesh, const unsigned int& textureId, double duration, double multiplier) : Capsule(mesh, textureId) {
        multiplier_ = multiplier;
        duration_ = duration;
    }

    ///\brief Teste et gère les collisions
    ///\param objet personnage avec lequel la collision est testée
    void Collision(Character* objet) {
        if (Physic::BoundingSphereCollisionTest(objet->GetRayon(), GetRayon(), objet->GetPosition(), GetPosition())) {
            if (!EffectManager::EffectExists(JUMP))
                EffectManager::AddEffect(JUMP, new Effect(duration_, multiplier_));
            else
                EffectManager::Refresh(JUMP, duration_, multiplier_);
        }
    }
};
#endif // CAPSULES_HPP