#include "Objet3dDrawable.hpp"



class Ventaimants : public Objet3dDrawable {

public:
    Ventaimants(TexturedMesh* mesh, const unsigned int& textureId, Vector3d position) : Objet3dDrawable(mesh, textureId) {
        Matrix44d tmatrix;
        tmatrix.LoadIdentity();
        tmatrix.Translate(position);
        position_.SetOrigin({ position.x,position.y,position.z });
        mesh_->Transform(tmatrix);
    }


    virtual void HandleUpdate(double deltatime) = 0;

    virtual void CheckEffect(Character* character, double deltatime) = 0;



};


class Ventilateur : public Ventaimants {
protected:

    //Matrix44d rotMatrix;
   // Matrix44d tmatrix;
public:


    Ventilateur(TexturedMesh* mesh, const unsigned int& textureId, Vector3d position) : Ventaimants(mesh, textureId, position) {

        }


        void CheckEffect(Character * character, double deltatime) {
            //Vector3d ventiperso = (Vector3d(character->GetCoordinates().GetOrigin().x, character->GetCoordinates().GetOrigin().y, character->GetCoordinates().GetOrigin().z).GetNorm() + position_.GetOrigin().x, position_.GetOrigin().y, position_.GetOrigin().z);
            Vector3d ventiperso = position_.GetOrigin() - character->GetCoordinates().GetOrigin();
            if (ventiperso.GetNorm() < 80.0) {
                double angle = ventiperso.GetAngle(position_.GetXDirection());
                if (angle <  M_PI / 4 && angle > -M_PI / 4) {
                    character->SetSpeed({ character->GetSpeed()->x - deltatime * 500 * sin(angle) / ventiperso.GetNorm(),character->GetSpeed()->y,character->GetSpeed()->z - deltatime * 500 * cos(angle) / ventiperso.GetNorm() });
                    //character->SetAcceleration({ character->GetAcceleration()->x + deltatime * 10000 * sin(angle) / ventiperso.GetNorm(),character->GetSpeed()->y,character->GetAcceleration()->z + deltatime * 10000 * cos(angle) / ventiperso.GetNorm() });

                }

            }
        }


        virtual void HandleUpdate(double deltaTime) {
            Matrix44d rotMatrix;
            rotMatrix.LoadIdentity();
            rotMatrix.LoadRotation(position_.GetXDirection(), deltaTime * 15);
            rotMatrix.LoadTransformationOrigin(position_.GetOrigin());
            mesh_->Transform(rotMatrix);

        }

    };

    class Aimant : public Ventaimants {

    public:


        Aimant(TexturedMesh* mesh, const unsigned int& textureId, Vector3d position) : Ventaimants(mesh, textureId, position) {
        }


        void CheckEffect(Character* character, double deltatime) {
            Vector3d ventiperso = position_.GetOrigin() - character->GetCoordinates().GetOrigin();
            if (ventiperso.GetNorm() < 80.0) {
                double angle = ventiperso.GetAngle(position_.GetXDirection());
                if (angle <  M_PI / 4 && angle > -M_PI / 4) {
                    character->SetSpeed({ character->GetSpeed()->x + deltatime * 500 * sin(angle) / ventiperso.GetNorm(),character->GetSpeed()->y,character->GetSpeed()->z + deltatime * 500 * cos(angle) / ventiperso.GetNorm() });
                }

            }
        }


        virtual void HandleUpdate(double deltaTime) {
        }
    };
