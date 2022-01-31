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
class Plateforme : public Objet3dColisionable {
protected:
    
public:


    Plateforme(TexturedMesh* mesh, const unsigned int& textureId) : Objet3dColisionable(mesh, textureId) {
        
    }

    virtual void HandleUpdate(double deltaTime) = 0;
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


class PlateformeEffritable : public Plateforme {
private:
    double lifespan;
    Chrono timeOnPlateform;
    bool visible;
    bool colision;
public:
    PlateformeEffritable(double lifespan, TexturedMesh* mesh, const unsigned int& textureId):Plateforme(mesh, textureId){
        this->lifespan = lifespan;
        visible = true;
     
    }

    void Colisioned() {
        visible = false;
        timeOnPlateform.Reset();
    }

    void HandleRefresh() {
        if (visible) {
            Draw();
        }
    }
    void HandleUpdate(double deltaTime) {
        if (!visible) {
            if (timeOnPlateform.GetElapsedSeconds() > lifespan) {
                visible = false;
            }
            if (timeOnPlateform.GetElapsedSeconds() < 2.0 * lifespan) {
                visible = true;
            }
        }
    }
};

///\class PlateformeIntermittente
///\brief objet 3d qui se dessine
class PlateformeIntermittente : public Plateforme {
protected:
    bool invisible;
    Chrono elapsedTime;
    double duration;

public:
    PlateformeIntermittente(TexturedMesh* mesh, const unsigned int& textureId) : Plateforme(mesh, textureId) {
        srand(time(NULL));
        position_ = CoordinateSystem();
        bool invisible = false;
        elapsedTime.Reset();
        duration = rand() % 7 + 4;
    }

    void HandleUpdate(double deltaTime) {
        if (elapsedTime.GetElapsedSeconds() > duration) {
            invisible = !invisible;
            elapsedTime.Reset();
            duration = rand() % 7 + 4;
        }

    }

    void HandleRefresh() {
        if (!invisible) {
            Draw();
        }

    }




};

///\class PlateformePiege
///\brief plateforme qui bloque le joueur
class PlateformePiege : public Plateforme, public Observable<bool> {
private:
    Chrono elapsedTime_;
public:
    PlateformePiege(TexturedMesh* mesh, const unsigned int& textureId) : Plateforme(mesh, textureId) {
    }

    void Colisioned() {
        elapsedTime_.Reset();
        Notify(true);
    }

    void HandleUpdate(double deltaTime) {
        if (elapsedTime_.GetElapsedSeconds() > 2.0)
            Notify(false);
    }
};


class PlateformeMouvantes : public Objet3dColisionable {
private:
    double movemax_;
    Matrix44d translation_;
    Vector3d axe_;
    double movingtime;
    double speed_;
public:
    PlateformeMouvantes(TexturedMesh* mesh, const unsigned int& textureId, Vector3d axe, double movemax, double speed = 1.0) : Objet3dColisionable(mesh, textureId) {
        axe.Normalize();
        axe_ = axe;
        movemax_ = movemax;
        movingtime = 0;
        speed_ = speed;
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
    }

};


#endif // PLATEFORMES_HPP