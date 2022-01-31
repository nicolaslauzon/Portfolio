///\file SceneCONTROL.hpp
#ifndef SCENECONTROL_HPP
#define SCENECONTROL_HPP

#include <fstream>

#include "Scene.hpp"
#include "Label.hpp"
#include "ProgressBar.hpp"
#include "Image.hpp"
#include "Texture.hpp"
#include "Font.hpp"
#include "Model.hpp"
#include "ResourceManager.hpp"
#include "Application.hpp"
#include "SceneMenuPrincipal.hpp"
#include "FileSystem.hpp"
#include "Label.hpp"

void unclickW();
void unclickA();
void unclickS();
void unclickD();
void unclickSpace();
void escape();

///\class SceneControl
///\brief Scene qui permet de changer ses controles
class SceneControl : public Scene {
private:
	

	ColorButton* escapeRect;
	ColorButton* wButton;
	ColorButton* aButton;
	ColorButton* sButton;
	ColorButton* dButton;
	ColorButton* spaceButton;

	Label* wDescription;
	Label* aDescription;
	Label* sDescription;
	Label* dDescription;
	Label* spaceDescription;
public:
	static unsigned int currentsceneId_; ///< Scene en ce moment
	static bool wButtonDown;
	static bool aButtonDown;
	static bool sButtonDown;
	static bool dButtonDown;
	static bool spaceButtonDown;

	SceneControl() {

	}

	void Init() {
		currentsceneId_ = SCENECONTROLE;

		wButtonDown = false;
		aButtonDown = false;
		sButtonDown = false;
		dButtonDown = false;
		spaceButtonDown = false;

		SDL_ShowCursor(false);

		escapeRect = new ColorButton({ 20, 20, 180, 65 }, { 245, 201, 0, 255 }, { 87, 39, 230, 255 }, 0);
		
		TTF_Font* ttfFont = TTF_OpenFont((FileSystem::fontsPath + "font.ttf").c_str(), 30);
		Label* escapeLabel = new Label({ 20, 20, 0, 0 }, ResourceManager::GetResource<TTF_Font*>("font50"), { 0,0,0,255 }, "Retour");

		Label* w = new Label({ 358,247,100,40 }, ttfFont, { 0, 0, 0,255 }, "W");
		wButton = new ColorButton({ 350,247,46,42 }, { 255, 0, 0, 0 }, { 0, 0, 0, 50 });
		wDescription = new Label({ 358,147,100,40 }, ttfFont, { 0, 0, 0,255 }, "Mouvement vers l'avant");

		Label* a = new Label({ 347,298,100,40 }, ttfFont, { 0, 0, 0,255 }, "A");
		aButton = new ColorButton({ 334,301,46,42 }, { 255, 0, 0, 0 }, { 0, 0, 0, 100 });
		aDescription = new Label({ 358,147,100,40 }, ttfFont, { 0, 0, 0,255 }, "Mouvement vers la gauche");

		Label* s = new Label({ 407,298,100,40 }, ttfFont, { 0, 0, 0,255 }, "S");
		sButton = new ColorButton({ 393,301,46,42 }, { 255, 0, 0, 0 }, { 0, 0, 0, 100 });
		sDescription = new Label({ 358,147,100,40 }, ttfFont, { 0, 0, 0,255 }, "Mouvement vers l'arriere");

		Label* d = new Label({ 467,298,100,40 }, ttfFont, { 0, 0, 0,255 }, "D");
		dButton = new ColorButton({ 453,301,46,42 }, { 255, 0, 0, 0 }, { 0, 0, 0, 100 });
		dDescription = new Label({ 358,147,100,40 }, ttfFont, { 0, 0, 0,255 }, "Mouvement vers la droite");

		Label* space = new Label({ 467,410,100,40 }, ttfFont, { 0, 0, 0,255 }, "Barre d'Espace");
		spaceButton = new ColorButton({ 392,410, 350,42 }, { 255, 0, 0, 0 }, { 0, 0, 0, 100 });
		spaceDescription = new Label({ 358,147,100,40 }, ttfFont, { 0, 0, 0,255 }, "Faire un saut");

		//unsigned int id = Application::GetInstance()->loadTexture((FileSystem::imagesPath + "sceneControl_background.png").c_str());
		//Image* background = new Image(id, {0,0,1000, 700}, false);
		Image* background = new Image(*(ResourceManager::GetResource<unsigned int*>("backgroundSceneControle")), { 0,0,1000,700 }, false);


		escapeRect->setOnClick(escape); ///\brief A faire plus tard
		wButton->setOnClick(unclickW);
		aButton->setOnClick(unclickA);
		sButton->setOnClick(unclickS);
		dButton->setOnClick(unclickD);
		spaceButton->setOnClick(unclickSpace);


		///\brief Ajout des drawable a la liste de drawable
		AddDrawable(background);
		AddDrawable(escapeRect);
		AddDrawable(escapeLabel);

		AddDrawable(wButton);
		AddDrawable(aButton);
		AddDrawable(sButton);
		AddDrawable(dButton);
		AddDrawable(spaceButton);

		AddDrawable(w);
		AddDrawable(a);
		AddDrawable(s);
		AddDrawable(d);

		AddDrawable(space);

		Cursor::GetInstance()->Init(*(ResourceManager::GetResource<unsigned int*>("cursor")), { 0,0,0,0 });

	}

	~SceneControl() {
		ClearDrawableList();
	}



	
	void HandleEvent(SDL_Event sdlEvent) {
		wButton->Notification(sdlEvent);
		aButton->Notification(sdlEvent);
		sButton->Notification(sdlEvent);
		dButton->Notification(sdlEvent);
		spaceButton->Notification(sdlEvent);
		escapeRect->Notification(sdlEvent);
		Cursor::GetInstance()->Notification(sdlEvent);

	}
	
	void HandleUpdate(double deltaTime) {
		
	}
	
	void HandleRefresh() {
		Application::GetInstance()->GetWindow()->Clear();


		RenderAll();

		Cursor::GetInstance()->Draw();

		if (wButtonDown) {
			wDescription->Draw();
		}
		if (aButtonDown) {
			aDescription->Draw();
		}
		if (sButtonDown) {
			sDescription->Draw();
		}
		if (dButtonDown) {
			dDescription->Draw();
		}
		if (spaceButtonDown) {
			spaceDescription->Draw();
		}
		if (SceneControl::currentsceneId_ == SCENEMENU) {
			//Application::GetInstance()->AddScene(SCENEMENU, new SceneMenuPrincipal());
			Application::GetInstance()->SetScene(SCENEMENU);
			//Application::GetInstance()->RemoveScene(SCENECONTROLE);
			
		}
		
	}
};

bool SceneControl::wButtonDown = false;
bool SceneControl::aButtonDown = false;
bool SceneControl::sButtonDown = false;
bool SceneControl::dButtonDown = false;
bool SceneControl::spaceButtonDown = false;
unsigned int SceneControl::currentsceneId_ = 0;


void unclickW() {
	SceneControl::wButtonDown = !SceneControl::wButtonDown;
	if (SceneControl::wButtonDown) {
		SceneControl::aButtonDown = false;
		SceneControl::sButtonDown = false;
		SceneControl::dButtonDown = false;
		SceneControl::spaceButtonDown = false;
	}
}
void unclickA() {
	SceneControl::aButtonDown = !SceneControl::aButtonDown;
	if (SceneControl::aButtonDown) {
		SceneControl::wButtonDown = false;
		SceneControl::sButtonDown = false;
		SceneControl::dButtonDown = false;
		SceneControl::spaceButtonDown = false;
	}
}
void unclickS() {
	SceneControl::sButtonDown = !SceneControl::sButtonDown;
	if (SceneControl::sButtonDown) {
		SceneControl::aButtonDown = false;
		SceneControl::wButtonDown = false;
		SceneControl::dButtonDown = false;
		SceneControl::spaceButtonDown = false;
	}
}
void unclickD() {
	SceneControl::dButtonDown = !SceneControl::dButtonDown;
	if (SceneControl::dButtonDown) {
		SceneControl::aButtonDown = false;
		SceneControl::sButtonDown = false;
		SceneControl::wButtonDown = false;
		SceneControl::spaceButtonDown = false;
	}
}
void unclickSpace() {
	SceneControl::spaceButtonDown = !SceneControl::spaceButtonDown;
	if (SceneControl::spaceButtonDown) {
		SceneControl::wButtonDown = false;
		SceneControl::aButtonDown = false;
		SceneControl::sButtonDown = false;
		SceneControl::dButtonDown = false;
	}
}

void escape() {
	SceneControl::currentsceneId_ = SCENEMENU;
}

#endif //SCENECONTROL_HPP
