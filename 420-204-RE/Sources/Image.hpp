///\file Image.hpp
///\brief Affichage d'image
#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>

#include "Vector.hpp"
#include "Drawable.hpp"
#include "Observer.hpp"

using namespace std;

///\class Image
///\brief Affichage d'image
class Image : public Drawable {
private:
	unsigned int id_;///< Identifiant de la texture

public:
	///\param id Identifiant de la texture
	///\param imageCoord Position et taille de l'image
	///\param queryTexture Utilise la taille originale de la ressource ou non
	Image(const unsigned int& id, SDL_Rect imageCoord, bool queryTexture = true) {
		rect_ = imageCoord;
		ChangeImage(id, queryTexture);
	}

	///\brief Changement de l'image
	///\param id Identifiant de la texture
	///\param queryTexture Utilise la taille originale de la ressource ou non
	void ChangeImage(const unsigned int& id, bool queryTexture = true) {
		id_ = id;
		
		if (queryTexture) {
			glBindTexture(GL_TEXTURE_2D, id_);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &rect_.w);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &rect_.h);
		}		
	}

	///\brief Affiche l'image
	void Draw() {
		glBindTexture(GL_TEXTURE_2D, id_);
		glBegin(GL_QUADS);
			glTexCoord2i(0.0, 0.0); glVertex3d(          rect_.x,	          rect_.y, 0.0);
			glTexCoord2i(1.0, 0.0); glVertex3d(rect_.x + rect_.w,           rect_.y, 0.0);
			glTexCoord2i(1.0, 1.0); glVertex3d(rect_.x + rect_.w, rect_.y + rect_.h, 0.0);
			glTexCoord2i(0.0, 1.0); glVertex3d(          rect_.x, rect_.y + rect_.h, 0.0);
		glEnd();
	}
};

#endif