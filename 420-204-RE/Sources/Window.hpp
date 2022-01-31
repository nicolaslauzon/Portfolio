///\file Window.hpp

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>

///\class Window
///\brief Fenêtre sdl2
class Window {
    protected:
        SDL_Window* sdlWindow_; ///< Fenêtre sdl2
    
    public:
        ///\brief Constructeur de base
        ///\param title titre de la fenêtre
        ///\param x position en x de la fenêtre
        ///\param y position en y de la fenêtre
        ///\param width largeur de la fenêtre
        ///\param height hauteur de la fenêtre
        ///\param flags flag en rapport avec fenêtre
        Window(const char* title, const int& x, const int& y, const int& width, const int& height, const unsigned int& flags) {
            sdlWindow_ = SDL_CreateWindow(title, x, y, width, height, flags);
        }
        ///\brief Destructeur de la classe
        virtual ~Window() {
            SDL_DestroyWindow(sdlWindow_);
        }

        ///\brief Retourne la taille de la fenetre
        SDL_Point GetSize() {
            SDL_Point size;
            SDL_GetWindowSize(sdlWindow_, &size.x, &size.y);
            return size;
        }

        ///\brief efface le contenu de la fenêtre
        virtual void Clear() = 0;

        virtual void Refresh() = 0;

        ///\brief Obtenir l'id de la fenêtre
        inline const unsigned int GetId() {
            return SDL_GetWindowID(sdlWindow_);
        }
};
#endif