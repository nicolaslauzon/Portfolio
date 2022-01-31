///\file DebugHud.hpp
#ifndef DEBUGHUD_HPP
#define DEBUGHUD_HPP

#include <list>
#include <map>

#include "RectFill.hpp"
#include "Label.hpp"
#include "Application.hpp"
#include "ResourceManager.hpp"
#include "FileSystem.hpp"
#include "Font.hpp"
#include "Button.hpp"
#include "Texture.hpp"
#include "Objet3dColisionable.hpp"

///\class DisplaySpeedVector 
///\brief Vecteur visuel 3d openGl
class DisplaySpeedVector : public Objet3dDrawable {
private:
    Vector3d* speed_; ///< valeur numérique du vecteur
    Objet3dDrawable* linkedObj_; ///< objet parent
    double previousNorm_; ///< dernière norme enregistré
    Vector3d previousSpeed_; ///< dernière vitesse enregistré
    Vector3d origin_; ///< origine de l'objet
public:
    DisplaySpeedVector(Objet3dDrawable* objet, Vector3d* speed) 
        : Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("debugSpeed"), *(ResourceManager::GetResource<unsigned int*>("debugSpeed.png")))
    {
        speed_ = speed;
        linkedObj_ = objet;
        position_.SetOrigin(origin_);
        previousNorm_ = 0.0;
        previousSpeed_ = Vector3d(0.0, 0.0, -1.0);
    }

    ///\brief Refresh l'aspect du vecteur selon les valeurs actuels
    void SetSpeed() {
        if ((speed_->x != 0.0 || speed_->y != 0.0 || speed_->z  != 0.0) && *speed_ != previousSpeed_) {
            double norm = speed_->GetNorm();
            double angle = speed_->GetAngle(previousSpeed_);
            Rotate(*speed_ % previousSpeed_, angle);
            previousSpeed_ = *speed_;
            if (norm != 0.0 && previousNorm_ != 0.0) {
                const double value = 1.0 / previousNorm_ * norm; 
                Scale(Vector3d(value, value, value));
            }
            previousNorm_ = norm;
        }
    }

    ///\brief Draw le vecteur
    void Draw() {
        Translate(Vector3d() - position_.GetOrigin());
        position_.SetOrigin(Vector3d());
        SetSpeed();
        position_.SetOrigin(linkedObj_->GetPosition().GetOrigin() + origin_);
        Translate(position_.GetOrigin());
        Objet3dDrawable::Draw();
    }
};

///\class DisplayCollisionSphere
///\brief Sphère de collision visuelle 3d openGl
class DisplayCollisionSphere : public Objet3dDrawable {
private:
    Objet3dColisionable* linkedObj_; ///< Objet parent
    double size_; ///< rayon de la sphère
public:

    DisplayCollisionSphere(Objet3dColisionable* objet)
        : Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("debugSphere"), *(ResourceManager::GetResource<unsigned int*>("debugSphere.png")))
    {
        linkedObj_ = objet;
        double value = objet->GetRayon();
        Scale(Vector3d(value, value, value));
        Translate(objet->GetPosition().GetOrigin());
    }

    ///\brief Draw la sphère
    void Draw() {
        Vector3d currentPosition = linkedObj_->GetPosition().GetOrigin();
        if (currentPosition != position_.GetOrigin()) {
            Translate(currentPosition - position_.GetOrigin());
            position_.SetOrigin(currentPosition);
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        Objet3dDrawable::Draw();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
};

///\class DisplayCenterOfMass 
///\brief dessine le centre de masse d'un objet
class DisplayCenterOfMass : public Objet3dDrawable {
private:
    Objet3dDrawable* linkedObj_; ///< Objet parent
public:

    DisplayCenterOfMass(Objet3dDrawable* objet)
        : Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("debugCenterMass"), *(ResourceManager::GetResource<unsigned int*>("debugCenterMass.png")))
    {
        linkedObj_ = objet;
        position_.SetOrigin(objet->GetPosition().GetOrigin());
        Translate(objet->GetPosition().GetOrigin());
    }

    ///\brief Draw le centre de masse
    void Draw() {
        Vector3d currentPosition = linkedObj_->GetPosition().GetOrigin();
        if (currentPosition != position_.GetOrigin()) {
            Translate(currentPosition - position_.GetOrigin());
            position_.SetOrigin(currentPosition);
        }
        Objet3dDrawable::Draw();
    }
};

enum DebugOverlayIds {
    FPS = 0,
    FPSVALUE = 1,
    SPEED = 2,
    COLLISIONSPHERE = 3,
    CENTREDEMASSE = 4,
    FPSRECT = 5,
    FPSVALUERECT = 6,
    SPEEDRECT = 7,
    COLLISIONSPHERERECT = 8,
    CENTREDEMASSERECT = 9
};

///\class DebugHud
///\brief Hud de debugging
///\note Comment utiliser le DebugHud: \n 1. Enable le debugging: définir la constante DEBUG (1ère ligne du fichier main.cpp). \n 2. Disable le debugging: Commenter la définition de la constante DEBUG (1ère ligne du fichier main.cpp). \n 3. Toggle le debugging: Appuyer sur backspace pour toggle le debugging on / off. \n 4. Toggle les informations de debug individuellement: Utiliez les boutons du côté droit de l'écran.
class DebugHud : public Observer<SDL_Event> {
private:
    map<unsigned int, Drawable*> overlay_; ///< labels du overlay
    map<unsigned int, Drawable*> overlayNotification_; ///< boutons du overlay

    list<DisplayCenterOfMass*> centerOfMass_; ///< centres de masse
    list<DisplayCollisionSphere*> collisionSphere_; ///< sphères de collision
    list<DisplaySpeedVector*> speed_; ///< vecteurs de vitesse

    bool draw_; ///< true-> dessine le debug hud // false-> ne le dessine pas
    bool displayCenterOfMass_; ///< true-> dessine les centres de masse // false ne les dessine pas
    bool displaySpeed_; ///< true-> dessine les vecteurs de vitesse // false ne les dessine pas
    bool displayCollisionSphere_; ///< true-> dessine les sphères de collision // false ne les dessine pas

    double deltaTBuffer_; ///< accumulation du deltaT
    unsigned int framesBuffer_; ///< nombre de refresh dans la seconde
    
public:

    DebugHud() {
        framesBuffer_ = 0;
        deltaTBuffer_ = 0.0;

        draw_ = displaySpeed_ = displayCenterOfMass_ = displayCollisionSphere_ = true;

        //TTF_Font* font = ResourceManager::GetResource<TTF_Font*>("font50");
        TTF_Font* font = TTF_OpenFont((FileSystem::fontsPath + "font.ttf").c_str(), 20);

        int w = 0, h = 0;

        TTF_SizeText(font, "FPS: 60", &w, &h);
        Label* fps = new Label({1000-w, 200, 0, 0}, font, {0, 0, 0, 255},"FPS: ");
        TTF_SizeText(font, "60", &w, &h);
        Label* fpsValue = new Label({1000-w, 200, 0, 0}, font, {0, 0, 0, 255}, " ");
        TTF_SizeText(font, "Speed vectors", &w, &h);
        Label* speed = new Label({1000-w, 200+h, 0, 0}, font, {0, 0, 0, 255}, "Speed vectors");
        TTF_SizeText(font, "Collision sphere", &w, &h);
        Label* collisionSphere = new Label({1000-w, 200+h*2, 0, 0}, font, {0, 0, 0, 255}, "Collision sphere");
        TTF_SizeText(font, "Centre de masse", &w, &h);
        Label* centreDeMasse = new Label({1000-w, 200 + h*3, 0, 0}, font, {0, 0, 0, 255}, "Centre de masse");
        
        SDL_Rect rect = centreDeMasse->GetDstRect();
        rect.y -= h*3;
        ColorButton* fpsRect = new ColorButton(rect, { 245, 201, 0, 255 }, {21, 0, 128, 255});
        rect.y += h;
        ColorButton* speedRect = new ColorButton(rect, { 245, 201, 0, 255 }, {130, 0, 255, 255});
        const auto f1 = [=]() { displaySpeed_ = !displaySpeed_; };
        speedRect->setOnClick(f1);
        rect.y += h;
        ColorButton* collisionSphereRect = new ColorButton(rect, { 245, 201, 0, 255 }, {255, 72, 0, 255});
        const auto f2 = [=]() { displayCollisionSphere_ = !displayCollisionSphere_; };
        collisionSphereRect->setOnClick(f2);
        rect.y += h;
        ColorButton* centreDeMasseRect = new ColorButton(rect, { 245, 201, 0, 255 }, {0, 255, 51, 255});
        const auto f3 = [=]() { displayCenterOfMass_ = !displayCenterOfMass_; };
        centreDeMasseRect->setOnClick(f3);

        overlayNotification_[FPSRECT] = fpsRect;
        overlayNotification_[SPEEDRECT] = speedRect;
        overlayNotification_[COLLISIONSPHERERECT] = collisionSphereRect;
        overlayNotification_[CENTREDEMASSERECT] = centreDeMasseRect;

        overlay_[FPS] = fps;
        overlay_[FPSVALUE] = fpsValue;
        overlay_[SPEED] = speed;
        overlay_[COLLISIONSPHERE] = collisionSphere;
        overlay_[CENTREDEMASSE] = centreDeMasse;
    }

    ~DebugHud() {
        for (auto it : overlay_) {
            if(it.second != nullptr) {
                delete it.second;
                it.second = nullptr;
            }
        }
        for (auto it : overlayNotification_) {
            if(it.second != nullptr) {
                delete it.second;
                it.second = nullptr;
            }
        }
        for (auto it : centerOfMass_) {
            if(it != nullptr) {
                delete it;
                it = nullptr;
            }
        }
        for (auto it : speed_) {
            if(it != nullptr) {
                delete it;
                it = nullptr;
            }
        }
        for (auto it : collisionSphere_) {
            if(it != nullptr) {
                delete it;
                it = nullptr;
            }
        }
    }

    ///\brief Ajoute un objet au systeme de debug visuel
    ///\param objet objet ajouté
    ///\param speed vitesse de l'objet ajouté (s'il y a lieu)
    void AddDebug(Objet3dDrawable* objet, Vector3d* speed = nullptr) {
        DisplayCenterOfMass* m = new DisplayCenterOfMass(objet);
        centerOfMass_.push_back(m);
        Objet3dColisionable* objetCollision = dynamic_cast<Objet3dColisionable*>(objet);
        if (objetCollision) {
            DisplayCollisionSphere* s = new DisplayCollisionSphere(objetCollision);
            collisionSphere_.push_back(s);
        }
        if (speed != nullptr) {
            DisplaySpeedVector* v = new DisplaySpeedVector(objet, speed);
            speed_.push_back(v);
        }
    }

    ///\brief refresh le debug hud
    ///\param deltaT delta time
    void HandleRefresh(double deltaT) {
        ++framesBuffer_;
        deltaTBuffer_ += deltaT;
        if (deltaTBuffer_ >= 1.0) {
            Label* fps = dynamic_cast<Label*>(overlay_[FPSVALUE]);
            if (fps) {
                fps->SetText(to_string(framesBuffer_).c_str());
                framesBuffer_ = 0;
                deltaTBuffer_ = 0.0;
            }

        }
    }

    ///\brief Gestion des évènements
    ///\param sdlEvent évènement
    void Notification(SDL_Event sdlEvent) {
        for (auto it : overlayNotification_) {
            dynamic_cast<ColorButton*>(it.second)->Notification(sdlEvent);
        }
        if (sdlEvent.type == SDL_KEYDOWN) {
            switch (sdlEvent.key.keysym.sym) {
                case SDLK_BACKSPACE:
                    draw_ = !draw_;
                    break;
            }
        }
    }

    ///\brief Draw le debug hud
    void Draw() {
        #ifdef DEBUG
            if (draw_) {
                if (displaySpeed_) {
                    for (auto speed : speed_) {
                        speed->Draw();
                    }
                }
                if (displayCollisionSphere_) {
                    for (auto collision : collisionSphere_) {
                        collision->Draw();
                    }
                }
                if (displayCenterOfMass_) {
                    for (auto mass : centerOfMass_) {
                        mass->Draw();
                    }
                }

                GlContext* w = dynamic_cast<GlContext*>(Application::GetInstance()->GetWindow());
                glDisable(GL_LIGHTING);
                glDisable(GL_LIGHT0);
                glDisable(GL_DEPTH_TEST);

                glDisableClientState(GL_VERTEX_ARRAY);
                glDisableClientState(GL_NORMAL_ARRAY);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                if (w!= nullptr && w->GetStatus() == _3D) {
                    w->SetDefaultPerspective(_2D);
                }

                for (auto drawable : overlayNotification_) {
                    drawable.second->Draw();
                }
                for (auto drawable : overlay_) {
                    drawable.second->Draw();
                }

                glEnable(GL_LIGHTING);
                glEnable(GL_LIGHT0);
                glEnable(GL_DEPTH_TEST);

                glEnableClientState(GL_VERTEX_ARRAY);
                glEnableClientState(GL_NORMAL_ARRAY);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);


                w->SetDefaultPerspective(_3D);
            }
        #endif
    }
};
#endif