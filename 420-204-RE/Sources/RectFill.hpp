///\file RectFill.hpp
///\brief Rectangle 2D de couleur openGl

#ifndef RECTFILL_HPP
#define RECTFILL_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "Drawable.hpp"

class RectFill : public Drawable {
private:
    SDL_Color color_; ///< Couleur du rectangle RGBA 32 bits

public:
    ///\brief Constructeur de base
    ///\param rect Position et dimension du rectangle
    ///\param color Couleur du rectangle RGBA 32 bits
    ///\param priority Priorité d'affichage
    RectFill(const SDL_Rect& rect, const SDL_Color& color, unsigned int priority = 0) {
        SetColor(color);
        SetRect(rect);
        SetPriority(priority);
    }

    ///\brief Réglage de la couleur du rectangle
    ///\param newColor Nouvelle couleur du rectangle RGBA 32 bits
    void SetColor(const SDL_Color& newColor) {
        color_ = newColor;
    }

    ///\brief Obtention de la couleur du rectangle
    ///\return couleur du rectangle RGBA 32 bits
    const SDL_Color& GetColor() const {
        return color_;
    }

    ///\brief Dessin du rectangle de couleur openGl
    void Draw() {
        glDisable(GL_TEXTURE_2D);
        glColor4ub(color_.r, color_.g, color_.b, color_.a);
        glBegin(GL_QUADS);
            glVertex3d(          rect_.x,           rect_.y, 0.0);
            glVertex3d(rect_.x + rect_.w,           rect_.y, 0.0);
            glVertex3d(rect_.x + rect_.w, rect_.y + rect_.h, 0.0);
            glVertex3d(          rect_.x, rect_.y + rect_.h, 0.0);
        glEnd();
        glColor4d(1.0, 1.0, 1.0, 1.0);
        glEnable(GL_TEXTURE_2D);
    }
};

#endif
