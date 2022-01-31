///\file Checkpoint.hpp
#ifndef CHECKPOINT_HPP
#define CHECKPOINT_HPP


#define RATIO 2.0
#include "Objet3dDrawable.hpp"
#include "Character.hpp"
#include "Physic.hpp"

///\class Checkpoint
///\brief checkpoint
class Checkpoint {
private:
    Objet3dDrawable* flag_;
    Objet3dDrawable* support_;
    Vector3d respawnPosition_;
    bool checkpointPassed_;

public:
    Checkpoint(Vector3d position) {
        flag_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("checkpointFlagM"), *ResourceManager::GetResource<unsigned int*>("checkpointBaseFlag"));
        support_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("checkpointPillierM"), *ResourceManager::GetResource<unsigned int*>("checkpointPillier"));

        flag_->Translate(position);
        support_->Translate(position);
        
        position.y += 5.0;
        respawnPosition_ = position;
        checkpointPassed_ = false;
    }

    ~Checkpoint() {
        delete flag_;
        delete support_;
    }

    bool CollisionWithCheckpoint(Character* charac) {
        if (!checkpointPassed_) {
            if (Physic::BoundingSphereCollisionTest(charac->GetRayon(), support_->GetRayon(), charac->GetPosition(), respawnPosition_)) {
                checkpointPassed_ = true;
                flag_->SetTexture(*ResourceManager::GetResource<unsigned int*>("checkpointPassedFlag"));
                return true;
            }
        }
        return false;
    }

    void Draw() {
        flag_->Draw();
        support_->Draw();
    }

    const Vector3d& GetRespawn() {
        return respawnPosition_;
    }
};

///\class Checkpoints
///\brief tout les checkpoints
class Checkpoints {
private:
    list<Checkpoint*> checkpoints_;
    Checkpoint* finishLine_;
    Vector3d respawn_;
    bool gameFinished_ = false;

public:
    Checkpoints() {
        checkpoints_.push_back(new Checkpoint({ 46.5 * RATIO,0.5 * RATIO,0.0 }));
        checkpoints_.push_back(new Checkpoint({ 118.0 * RATIO,0.5 * RATIO,0.0 }));
        checkpoints_.push_back(new Checkpoint({ 172.0 * RATIO, 0.5 * RATIO, 0.0 }));

        finishLine_ = new Checkpoint(Vector3d({225 * RATIO, 0.5 * RATIO, 13.0 * RATIO }));

        respawn_ = Vector3d(0.0, 5.0, 0.0);
    }

    ~Checkpoints() {
        for (auto it : checkpoints_) {
            delete it;
        }
    }

    void HandleUpdate(Character* charac) {
        for (auto it : checkpoints_) {
            if (it->CollisionWithCheckpoint(charac)) {
                respawn_ = it->GetRespawn();
            }
        }
        
        if (charac->GetPosition().GetOrigin().y < -30.0) {
            Matrix44d m;
            m.LoadIdentity();
            m.Translate(respawn_ - charac->GetPosition().GetOrigin());
            charac->Transform(m);
            charac->SetPosition(charac->GetPosition().GetOrigin() + m.GetTranslation());
        }

        if (finishLine_->CollisionWithCheckpoint(charac)) {
            gameFinished_ = true;
        }
    }

    bool GetGameStatus() {
        return gameFinished_;
    }

    void Draw() {
        for (auto it : checkpoints_) {
            it->Draw();
        }
        finishLine_->Draw();
    }
};
#endif