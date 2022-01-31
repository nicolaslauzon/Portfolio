///\file Button.hpp
///\brief Bouton 

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Drawable.hpp"
#include "Clickable.hpp"
#include "Hoverable.hpp"
#include "RectFill.hpp"
#include "Observer.hpp"
#include "Image.hpp"

///\class Button
///\brief Base d'un bouton
class Button : public Drawable, public Observer<SDL_Event>, public Clickable {
public:
	///\param rect Position et Taille
	///\param priority Priorité d'affichage
	Button(SDL_Rect rect, unsigned int priority) : Drawable(priority, rect) {}

	///\brief Affichage du bouton
	virtual void Draw() = 0;

	///\brief Notification d'événement
	///\param sdlEvent Événement SDL2
	virtual void Notification(SDL_Event sdlEvent) = 0;
};

/*
///\class ButtonImage
///\brief Bouton pouvant etre represente par n'importe quel objet affichable(Drawable)
class ButtonImage: public Button {
private:
	Image* image_;	///< Image a afficher sur le bouton

public:
	///\param pFonction Fonction � effectuer
	///\param content Image du bouton
	///\param rect Taille et position du bouton
	///\param priority Priorit� d'affichage
	ButtonImage(void(*pFonction)(), Image* image, SDL_Rect rect = { 0,0,0,0 }, unsigned int priority = 0) : Button(pFonction, rect, priority) {
		image_ = image;
		if(image_)
			image_->SetRect(rect_);
	}

	///\brief Permet de modifier la taille de l'image avec celle du bouton
	///\param rect Taille a modifier
	void SetRect(SDL_Rect rect) {
		rect_ = rect;
		image_->SetRect(rect_);
	}

	///\brief Affiche le bouton
	void Draw() {
		if (image_)
			image_->Draw();
		if (hover_)
			shade_.Draw();
	}
};
*/

///\class ColorButton
///\brief Bouton de couleur
class ColorButton : public Button, public Hoverable {
private:
	RectFill background_;  ///< Rectangle d'arrière-plan
	SDL_Color color_,      ///< Couleur de base
	          hoverColor_; ///< Couleur de survol

public:
	///\param rect Taille et Position
	///\param priority Priorité d'affichage
	ColorButton(SDL_Rect rect, SDL_Color color, SDL_Color hoverColor, unsigned int priority = 0) : Button(rect, priority), background_(rect_, color) {
		color_ = color;
		hoverColor_ = hoverColor;
		hover_ = false;
	}

	///\brief Affichage du bouton
	void Draw() {
		(hover_) ? background_.SetColor(hoverColor_) : background_.SetColor(color_);
		background_.Draw();
	}

	///\brief Gestion d'événement
	void Notification(SDL_Event sdlEvent) {
		SDL_Point cursor = { sdlEvent.motion.x, sdlEvent.motion.y };
		switch (sdlEvent.type) {
			case SDL_MOUSEBUTTONUP:			
				if (hover_ && onClick)
					onClick();
				break;

			case SDL_MOUSEMOTION:
				hover_ = SDL_PointInRect(&cursor, &rect_);
				break;
		}
	}
};

#endif
