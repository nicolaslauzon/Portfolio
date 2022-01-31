///\file SceneMenu.hpp
///\brief Classe repr�sentant l'affichage de l'interface du menu

#ifndef SCENEMENU_HPP
#define SCENEMENU_HPP

#include "FileSystem.hpp"
#include "Scene.hpp"
#include "Button.hpp"
#include "RectFill.hpp"
#include "Label.hpp"
#include "GlContext.hpp"
#include "Application.hpp"
#include "Cursor.hpp"
#include "SceneJeu.hpp"
#include "SceneControl.hpp"
#include "SceneOptions.hpp"
#include "SceneClassement.hpp"
#include "SceneManager.hpp"
#include "ResourceManager.hpp"
#include "SceneResultat.hpp"
#include "TestElie.hpp" 
void FonctionBoutonJouer();
void FonctionBoutonCommentJouer();
void FonctionBoutonOptions();
void FonctionBoutonClassement();
void FonctionBoutonQuitter();

///\class ScenMenuPrincipal
///\brief Sc�ne qui g�re et affiche le menu principal
class SceneMenuPrincipal : public Scene {

private:
	bool initBool_ = true;///<Variable pour indiquer si l'initialisation doit �tre faite
	
public:
	static unsigned int currentsceneId_; ///< Scene en ce moment

	ColorButton* boutonJouer_;///< Bouton "Jouer"
	ColorButton* boutonCommentJouer_;///< Bouton "Comment Jouer"
	ColorButton* boutonOptions_;///< Bouton "Options"
	ColorButton* boutonClassement_;///< Bouton "Classement"
	ColorButton* boutonQuitter_;///< Bouton "Quitter"

	Label *etiquetteJouer_;///< �tiquette pour le bouton "Jouer"
	Label* etiquetteCommentJouer_;///< �tiquette pour le bouton "Comment Jouer"
	Label* etiquetteOptions_;///< �tiquette pour le bouton "Options"
	Label* etiquetteClassement_;///< �tiquette pour le bouton "Classement"
	Label* etiquetteQuitter_;///< �tiquette pour le bouton "Quitter"

	Image* backgroundImage_;///< Image de fond
	Image* personnageImage_;///< Image du personnage
	
	void DeleteThis() {

		UnsubscribeEvent(SDL_MOUSEMOTION, boutonJouer_);
		UnsubscribeEvent(SDL_MOUSEBUTTONUP, boutonJouer_);

		UnsubscribeEvent(SDL_MOUSEMOTION, boutonCommentJouer_);
		UnsubscribeEvent(SDL_MOUSEBUTTONUP, boutonCommentJouer_);

		UnsubscribeEvent(SDL_MOUSEMOTION, boutonOptions_);
		UnsubscribeEvent(SDL_MOUSEBUTTONUP, boutonOptions_);

		UnsubscribeEvent(SDL_MOUSEMOTION, boutonClassement_);
		UnsubscribeEvent(SDL_MOUSEBUTTONUP, boutonClassement_);

		UnsubscribeEvent(SDL_MOUSEMOTION, boutonQuitter_);
		UnsubscribeEvent(SDL_MOUSEBUTTONUP, boutonQuitter_);

		ClearDrawableList();

		if (boutonJouer_) {
			delete boutonJouer_;
			boutonJouer_ = nullptr;
		}
		if (boutonCommentJouer_) {
			delete boutonCommentJouer_;
			boutonCommentJouer_ = nullptr;
		}
		if (boutonOptions_) {
			delete boutonOptions_;
			boutonOptions_ = nullptr;
		}
		if (boutonClassement_) {
			delete boutonClassement_;
			boutonClassement_ = nullptr;
		}
		if (boutonQuitter_) {
			delete boutonQuitter_;
			boutonQuitter_ = nullptr;
		}
	}

	void Init() {
		currentsceneId_ = SCENEMENU;

		ClearEventDispatcherMap();
		ClearDrawableList();
		
		backgroundImage_ = new Image(*(ResourceManager::GetResource<unsigned int*>("menuBackground")), { 0,0,1000,700 }, false);// image � changer
		backgroundImage_->SetPriority(1);
		AddDrawable(backgroundImage_);

		etiquetteJouer_ = new Label({ 20, 20, 0, 0 }, ResourceManager::GetResource<TTF_Font*>("font50"), { 0,0,0,255 }, "Jouer");//green, red, alpha,blue
		boutonJouer_ = new ColorButton({ 20, 20, 145, 65 }, { 245, 201, 0, 255 }, { 87, 39, 230, 255 }, 2);
		boutonJouer_->setOnClick(FonctionBoutonJouer);
		SubscribeEvent(SDL_MOUSEMOTION, boutonJouer_);
		SubscribeEvent(SDL_MOUSEBUTTONUP, boutonJouer_);		
		AddDrawable(boutonJouer_);
		AddDrawable(etiquetteJouer_);

		etiquetteCommentJouer_ = new Label({ 20, 90, 0 ,0 }, ResourceManager::GetResource<TTF_Font*>("font50"), { 0,0,0,255 }, "Comment Jouer");
		boutonCommentJouer_ = new ColorButton({ 20, 90, 400, 65 }, { 245, 201, 0, 255 }, { 0, 230, 175, 255 }, 2);
		boutonCommentJouer_->setOnClick(FonctionBoutonCommentJouer);
		SubscribeEvent(SDL_MOUSEMOTION, boutonCommentJouer_);
		SubscribeEvent(SDL_MOUSEBUTTONUP, boutonCommentJouer_);
		AddDrawable(boutonCommentJouer_);
		AddDrawable(etiquetteCommentJouer_);

		etiquetteOptions_ = new Label({ 20, 160,0,0 }, ResourceManager::GetResource<TTF_Font*>("font50"), { 0,0,0,255 }, "Options");
		boutonOptions_ = new ColorButton({ 20, 160, 200, 65 }, { 245, 201, 0, 255 }, { 0, 230, 175, 255 }, 2);
		boutonOptions_->setOnClick(FonctionBoutonOptions);
		SubscribeEvent(SDL_MOUSEMOTION, boutonOptions_);
		SubscribeEvent(SDL_MOUSEBUTTONUP, boutonOptions_);
		AddDrawable(boutonOptions_);
		AddDrawable(etiquetteOptions_);

		etiquetteClassement_ = new Label({ 20, 230,0,0 }, ResourceManager::GetResource<TTF_Font*>("font50"), { 0,0,0,255 }, "Classement");
		boutonClassement_ = new ColorButton({ 20, 230, 295, 65 }, { 245, 201, 0, 255 }, { 0, 230, 175, 255 }, 2);
		boutonClassement_->setOnClick(FonctionBoutonClassement);
		SubscribeEvent(SDL_MOUSEMOTION, boutonClassement_);
		SubscribeEvent(SDL_MOUSEBUTTONUP, boutonClassement_);
		AddDrawable(boutonClassement_);
		AddDrawable(etiquetteClassement_);

		etiquetteQuitter_ = new Label({ 20,300,0,0 }, ResourceManager::GetResource<TTF_Font*>("font50"), { 0,0,0,255 }, "Quitter");
		boutonQuitter_ = new ColorButton({ 20, 300, 190, 65 }, { 245, 201, 0, 255 }, { 255, 55, 28, 255 }, 2);
		boutonQuitter_->setOnClick(FonctionBoutonQuitter);
		SubscribeEvent(SDL_MOUSEMOTION, boutonQuitter_);
		SubscribeEvent(SDL_MOUSEBUTTONUP, boutonQuitter_);
		
		AddDrawable(etiquetteQuitter_);
		AddDrawable(boutonQuitter_);

		Cursor::GetInstance()->Init(*(ResourceManager::GetResource<unsigned int*>("cursor")), {0,0,0,0});
		SubscribeEvent(SDL_MOUSEMOTION, Cursor::GetInstance());		
	}

	///\brief Gère les événements
	///\param sdlEvent variable ou sont stockés tous les événements
	void HandleEvent(SDL_Event sdlEvent) {
		boutonJouer_->Notification(sdlEvent);
		boutonQuitter_->Notification(sdlEvent);
		boutonCommentJouer_->Notification(sdlEvent);
		boutonClassement_->Notification(sdlEvent);
 		boutonOptions_->Notification(sdlEvent);

		Cursor::GetInstance()->Notification(sdlEvent);
	}

	///\brief Gère les mises à jours
	///\param deltaTime temps �coul� depuis le dernier appel de cette m�me fonction
	void HandleUpdate(double deltaTime) {
		if (initBool_) {
			Init();
			initBool_ = false;
		}
	}
	
	///\brief Gère l'affichage
	void HandleRefresh() {
		Application::GetInstance()->GetWindow()->Clear();
		RenderAll();

		etiquetteClassement_->Draw();
		etiquetteCommentJouer_->Draw();
		etiquetteJouer_->Draw();
		etiquetteOptions_->Draw();
		etiquetteQuitter_->Draw();

		Cursor::GetInstance()->Draw();

		switch (currentsceneId_){

		case SCENEJEU:
			Application::GetInstance()->AddScene(SCENEELIE, new TestElie());
			Application::GetInstance()->SetScene(SCENEELIE);
			//Application::GetInstance()->RemoveScene(SCENEMENU);
			break;

		case SCENEOPTIONS:
			Application::GetInstance()->AddScene(SCENEOPTIONS, new SceneOptions());
			Application::GetInstance()->SetScene(SCENEOPTIONS);
			//Application::GetInstance()->RemoveScene(SCENEMENU);
			break;

		case SCENECONTROLE:
			Application::GetInstance()->AddScene(SCENECONTROLE, new SceneControl());
			Application::GetInstance()->SetScene(SCENECONTROLE);
			//Application::GetInstance()->RemoveScene(SCENEMENU);
			break;

		case SCENERESULTAT:
			Application::GetInstance()->AddScene(SCENERESULTAT, new SceneResulat());
			Application::GetInstance()->SetScene(SCENERESULTAT);
			//Application::GetInstance()->RemoveScene(SCENEMENU);
			break;

		}
	}
};
//Pas deux fen�tres donc doit avoir un bouton pour revenir au menu principal

unsigned int SceneMenuPrincipal::currentsceneId_ = 0;

///\brief Action du bouton "Jouer"
void FonctionBoutonJouer() {//Aller vers le jeu
	//nouvelle sc�ne (basculer ou delete de sc�ne )
	SceneMenuPrincipal::currentsceneId_ = SCENEJEU;
}

///\brief Action du bouton "Comment Jouer"
void FonctionBoutonCommentJouer() {//Aller � comment jouer
	SceneMenuPrincipal::currentsceneId_ = SCENECONTROLE;
}

///\brief Action du bouton "Options"
void FonctionBoutonOptions() {//Aller � options et garder les options quand on revient au menu principal
	SceneMenuPrincipal::currentsceneId_ = SCENEOPTIONS;
}

///\brief Action du bouton "Classement"
void FonctionBoutonClassement() {//aller au classement
	SceneMenuPrincipal::currentsceneId_ = SCENERESULTAT;
}

///\brief Action du bouton "Quitter"
void FonctionBoutonQuitter() {// quitter le programme
	//tout delete pour ne pas avoir de fuite de m�moire puis quitter le programme
	SDL_Event Iquit;
	Iquit.type = SDL_QUIT;
	SDL_PushEvent(&Iquit);
}

#endif
