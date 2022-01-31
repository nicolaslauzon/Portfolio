///\file GlContext.hpp

#ifndef GLCONTEXT_HPP
#define GLCONTEXT_HPP


#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <cmath>
#include <map>

#include "Window.hpp"
#include "Observable.hpp"
#include "Observer.hpp"
#include "Application.hpp"
#include "Vector.hpp"

using namespace std;

#define _2D 1
#define _3D 0

///\class GlContext
///\brief Contexte openGl 
class GlContext : public Window {
    private:
        SDL_GLContext glContext_; ///< Contexte openGl
        bool status_; ///< statut de la window: true-> _2D // false-> _3D

    public:

    ///\brief Constructeur de base
    ///\param title titre de la fenêtre
    ///\param x position verticale de la fenêtre
    ///\param y position horizontale de la fenêtre
    ///\param width largeur de la fenêtre
    ///\param height hauteur de la fenêtre
    ///\param flags Flags de la fenêtre sdl2
    GlContext(const char* title, const int& x, const int& y, const int& width, const int& height, const unsigned int& flags ) : Window(title, x, y, width, height, flags)  {
        glContext_ = SDL_GL_CreateContext(sdlWindow_);
    }

    ~GlContext() {
        SDL_GL_DeleteContext(glContext_);
    }

    ///\brief Efface le contenu du contexte openGl
    void Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    ///\brief Dessine le contenu du buffer dans le contexte openGl
    void Refresh() {
        SDL_GL_SwapWindow(sdlWindow_);
    }
    ///\brief Règle la perspective du contexte openGl
    ///\param fov Champ de vision
    ///\param nearCut Plan rapproché
    ///\param farCut Plan éloigné
    ///\param ortho true -> utilisation d'une matrice orthogonale // false -> utilisation d'une matrice de projection
    void SetPerspective(double fov, double nearCut, double farCut, bool ortho = false) {
        Vector2i size;
        SDL_GetWindowSize(sdlWindow_, &size.x, &size.y);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        if (ortho) {
            glOrtho(0.0, (double)size.x, (double)size.y, 0.0, nearCut, farCut);
        }
        else {
            const double top = tan(fov * 0.5 * M_PI / 180.0) * nearCut;
            const double right = ((double)size.x / (double)size.y) * top;
            glFrustum(-right, right, -top, top, nearCut, farCut);
        }

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        status_ = ortho;
    }

    ///\brief Règle une perspective par défault
    ///\param ortho true -> utilisation d'une matrice orthogonale // false -> utilisation d'une matrice de projection
    void SetDefaultPerspective(bool ortho = false) {
        if (ortho) {
            SetPerspective(0.0, 1.0, -1.0, _2D);
        }
        else {
            SetPerspective(89.0, 0.1, 5000.0, _3D);
        }
    }

    bool GetStatus() { return status_; }
};
#endif // GLCONTEXT_HPP
