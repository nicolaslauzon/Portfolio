///\file Drawable.hpp
///\brief Objet affichable

#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <SDL2/SDL.h>

class Drawable {
private:
    unsigned int priority_;
    
protected:
    SDL_Rect rect_;
    unsigned int resourceId_;

public:
    Drawable(unsigned int priority = 0, const SDL_Rect& rect = {0,0,0,0}) {
        priority_ = priority;
        rect_ = rect;
    }

    ///\brief Réglage de la priorité d'affichage du rectangle
    ///\param new_priority Nouvelle priorité d'affichage
    void SetPriority(unsigned int new_priority) {
        priority_ = new_priority;
    }

    ///\brief Retourne la priorité d'affichage
    unsigned int GetPriority() {
        return priority_;
    }

    ///\brief Réglage de la position et de la dimension du rectangle
    ///\param new_rect Nouvelle position et dimension
    void SetRect(const SDL_Rect& new_rect) {
        rect_ = new_rect;
    }
    
    void SetPosition(unsigned int x, unsigned int y) {
        rect_.x = x;
        rect_.y = y;
    }
    
    void SetSize(unsigned int w, unsigned int h) {
        rect_.w = w;
        rect_.h = h;
    }

    inline static bool Compare(Drawable* drawable1, Drawable* drawable2) {
        return drawable1->priority_ < drawable2->priority_;
    }

    inline SDL_Rect GetDstRect() {
        return rect_;
    }

    ///\brief Dessine le Drawable
    virtual void Draw() = 0;
};
#endif
