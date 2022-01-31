#ifndef TESTGABRIEL_HPP
#define TESTGABRIEL_HPP

#include "Scene.hpp"
#include "Label.hpp"
#include "Character.hpp"
class TestGabriel : public Scene {
private:
	TTF_Font* font;
	Label* label;
	Character* character;
	string str;
public:
	TestGabriel () {
	}

	~TestGabriel() {
		delete label;
		delete character;
		TTF_CloseFont(font);
	}

	///\brief appel� au d�but de la sc�ne
	///\param windowId identificateur de la fenetre
	void Init() {
		font = TTF_OpenFont("MrRobot.ttf", 42);
		label = new Label({ 500, 200,0,0 }, font, { 0,0,255,255 }, "Es hora de comer");
		//character = new Character();
		

	}

	///\brief �venements
	///\param sdlEvent �venement � g�rer
	void HandleEvent(SDL_Event sdlEvent) {

	}

	///\brief G�rer les modifications
	///\param deltaTime temps �coul� depuis le dernier appel de cette fonction
	void HandleUpdate(double deltaTime) {
		
		
	}

	///\brief affichage
	void HandleRefresh() {
		
		label->Draw();
	}
};


#endif