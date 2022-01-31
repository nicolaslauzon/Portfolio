#pragma once

#include <SDL2/SDL.h>

#include "Singleton.hpp"
#include "Observer.hpp"
#include "Observable.hpp"
#include "Image.hpp"
///\class Cursor
///\brief Classe pour g�rer et afficher el curseur
class Cursor : public Singleton<Cursor>, public Observer<SDL_Event>, public Drawable {
private: 

	Image* cursor_image;///< Image du curseur

public:
	~Cursor() {
		
		if (cursor_image) {
			delete cursor_image;
			cursor_image = nullptr;
		}
	}
	///\brief Initialiser la variable cursor_image
	///\param imageId Pointeur vers le id de l'image voulue
	///\param imageCoord Coordonn�es de l'image
	///\param queryTexture Variable qui indique si on veut conserver la taille r�elle de l'image ou non
	void Init(const unsigned int& imageId, SDL_Rect imageCoord, bool queryTexture = true) {
		cursor_image = new Image(imageId,imageCoord, queryTexture);
	}

	///\brief Afficher le curseur
	void Draw() {
		cursor_image->Draw();
	}

	///\brief Initialiser la position de cursor_image
	///\param x La valeur x de la position � laquelle on veut le curseur
	///\param y La valeur y de la position � laquelle on veut le curseur
	void SetCursorPosition(const int& x, const int& y) {
		rect_.x = x;
		rect_.y = y;
		cursor_image->SetPosition(x, y);
	}

	///\brief D�finir la resource � utiliser pour cursor_image
	///\param imageId Pointeur vers le id de l'image voulue
	void SetResource(const unsigned int& imageId) {
		cursor_image->ChangeImage(imageId);

	}

	///\brief Supprimer le curseur
	void DeleteThis() {
		if (cursor_image) {
			delete cursor_image;
			cursor_image = nullptr;
		}
	}
	///\brief Notifier les observateurs
	///\param sdlEvent variable o� sont stock�s tous les �v�nements
	void Notification(SDL_Event sdlEvent) {
		SetCursorPosition(sdlEvent.motion.x, sdlEvent.motion.y);
	}
};