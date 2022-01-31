///\file Label.hpp
///\brief Affichage de texte

#ifndef LABEL_HPP
#define LABEL_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

#include "Drawable.hpp"
using namespace std;

///\class Label
///\brief Affichage de texte
class Label : public Drawable{
protected:
	const char* content_; ///< Le contenu du texte
	SDL_Color color_; ///< Couleur du texte
	TTF_Font* font_; ///< Police de charactère
	SDL_Surface* fontSurface_; ///< Surface du texte
	bool needUpdate_; ///< S'il faut apporter des changement au texte
	unsigned int labelId_; ///< Le Id du texte

public:
	///\param x_ La position du texte en x
	///\param y_ La position du texte en y
	///\param font_ Police de charactère
	///\param color_ Couleur du texte
	///\param content_ Le contenu du texte
	Label(const SDL_Rect& LabelRect, TTF_Font* font, SDL_Color color, const char* content) {
		rect_ = LabelRect;
		font_ = font;
		color_ = color;
		SetText(content);
		needUpdate_ = false;
	}

	~Label() {
		glDeleteTextures(1, &labelId_);
	}

	///\brief Change la couleur
	///\param color_ La couleur
	void ChangeColor(SDL_Color color) {
		color_ = color;
		needUpdate_ = true;
	}

	TTF_Font* GetFont() {
		return font_;
	}

	///\brief Change le contenu et/ou met à jour le texte
	///\param content_ le contenu du texte 
	void SetText(const char* content) {
		content_ = content;
		if (content_ != nullptr) {
			if (labelId_ != 0)
				glDeleteTextures(1, &labelId_);
			fontSurface_ = TTF_RenderText_Blended(font_, content_, { color_.b, color_.g, color_.r, color_.a });
			labelId_ = Application::GetInstance()->loadTexture(fontSurface_);
			rect_.w = fontSurface_->w;
			rect_.h = fontSurface_->h;
			SDL_FreeSurface(fontSurface_);
		}
	}

	///\brief Affiche le texte
	virtual void Draw() {
		if (needUpdate_) {
			SetText(content_);
			needUpdate_ = false;
		}

		if (content_ != nullptr) {
			glBindTexture(GL_TEXTURE_2D, labelId_);
			glBegin(GL_QUADS);
				glTexCoord2i(0.0, 0.0); glVertex3d(rect_.x, rect_.y, 0.0);
				glTexCoord2i(1.0, 0.0); glVertex3d(rect_.x + rect_.w, rect_.y, 0.0);
				glTexCoord2i(1.0, 1.0); glVertex3d(rect_.x + rect_.w, rect_.y + rect_.h, 0.0);
				glTexCoord2i(0.0, 1.0); glVertex3d(rect_.x, rect_.y + rect_.h, 0.0);
			glEnd();
		}
	}
};

#endif
