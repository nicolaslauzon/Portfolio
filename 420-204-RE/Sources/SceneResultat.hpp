///\file SceneResultats.hpp
///\brief Classe montrant le scene apres la partie avec toutes les informations reliees

#ifndef SCENERESULTAT_HPP
#define SCENERESULTAT_HPP

#include "Scene.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "TextField.hpp"
#include "Chrono.hpp"
#include "LeaderboardHud.hpp"
#include "ResourceManager.hpp"

void ReturnToMenu();


///\class SceneResultats
///\brief Classe montrant le scene apres la partie avec toutes les informations reliees
class SceneResulat : public Scene {

	LeaderboardHud* leaderboard_; ///< Le leaderboard
	double totaltime_; ///< Temps total du joueur
	Image* background_; ///< Image de background
	TextField* textfieldnom_; ///< Textfield si le joueur a un bon temps
	ColorButton* escapeRect; ///< Bouton revenir menu principal
public:
	static unsigned int currentsceneId_; ///< Scene en ce moment
	SceneResulat() {


	}

	~SceneResulat() {
		if (textfieldnom_ != nullptr) {
			leaderboard_->AddScore(textfieldnom_->GetContent(), totaltime_);
			leaderboard_->Reset();
		}
		for(auto it: drawables){
			delete it;
			drawables.pop_front();
		}
	}

	///\brief Initialise toute les variables
	void Init() {
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_DEPTH_TEST);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		((GlContext*)Application::GetInstance()->GetWindow())->SetDefaultPerspective(_2D);
		currentsceneId_ = SCENERESULTAT;

		TTF_Font* font_ = ResourceManager::GetResource<TTF_Font*>("font30");
		TTF_Font* font2_ = ResourceManager::GetResource<TTF_Font*>("font20");

		Image* background_ = new Image(*(ResourceManager::GetResource<unsigned int*>("resultats")), { 0,0,0,0 }, true);
		//La ressource n'est actuellement pas charg�e, c'est pour cela que la classe ne l'affichera pas dans cet �tat. Lorsque SceneChargement sera utilis�e avant, l'affichage fonctionnera
		
		SDL_Point size = Application::GetInstance()->GetWindow()->GetSize();
		background_->SetSize(size.x, size.y);
		drawables.push_back(background_);

		escapeRect = new ColorButton({ 1,1,110,43 }, { 255,255,255,255 }, { 255, 255, 255, 155 });
		escapeRect->setOnClick(ReturnToMenu);
		SubscribeEvent(SDL_MOUSEMOTION, escapeRect);
		SubscribeEvent(SDL_MOUSEBUTTONUP, escapeRect);
		drawables.push_back(escapeRect);

		Label* escapeLabel = new Label({ 4, 1,100,40 }, font2_, { 0,0,0,255 }, "Escape");
		drawables.push_back(escapeLabel);

		leaderboard_ = new LeaderboardHud((FileSystem::resourcePath + "Leaderboard.txt").c_str(), font2_, { 255,255,255,255 }, { 750,163,0,0 },3,90);
		drawables.push_back(leaderboard_);
		if (leaderboard_->GetTime(leaderboard_->GetNumberofPlayers()) > totaltime_ || leaderboard_->GetNumberofPlayers() <= 10) {
			textfieldnom_ = new TextField({ 717, 580, 230, 200 }, font_, { 25, 0, 0, 255 }, { 255, 255, 255, 255 });
			drawables.push_back(textfieldnom_);
			
			SubscribeEvent(SDL_KEYUP, textfieldnom_);
			SubscribeEvent(SDL_MOUSEMOTION, textfieldnom_);
			SubscribeEvent(SDL_MOUSEBUTTONUP, textfieldnom_);



		}

		Cursor::GetInstance()->Init(*(ResourceManager::GetResource<unsigned int*>("cursor")), { 0,0,0,0 });
		//La ressource n'est actuellement pas charg�e, c'est pour cela que la classe ne l'affichera pas dans cet �tat. Lorsque SceneChargement sera utilis�e avant, l'affichage fonctionnera
		SubscribeEvent(SDL_MOUSEMOTION, Cursor::GetInstance());
		
		
		Label* total_ = new Label({ 250, 420, 0, 0 }, font_, { 255,255,255,255 }, to_string(totaltime_).c_str());
		drawables.push_back(total_);

		Label* position_ = new Label( { 400, 300, 0, 0 }, font_, { 255,255,255,255 }, "1er");
		drawables.push_back(position_);
		
	}

	///\brief Gestion des �v�nements dans la sc�ne
	///\param sdlEvent Evenement transmis
	void HandleEvent(SDL_Event sdlEvent) {
		if (sdlEvent.type == SDL_KEYDOWN) {
			sdlEvent = sdlEvent;
		}

		if (eventDispatcher_.find(sdlEvent.type) != eventDispatcher_.end())
			eventDispatcher_[sdlEvent.type]->Notify(sdlEvent);

	}

	///\brief Gestion des updates
	void HandleUpdate(double deltaTime) {

	}
	///\brief Gestion de l'affichage
	void HandleRefresh() {
		for (auto it : drawables) {
			it->Draw();
		}
		Cursor::GetInstance()->Draw();

		if (SceneResulat::currentsceneId_ == SCENEMENU) {
			DeleteScene();
			//Application::GetInstance()->AddScene(SCENEMENU, new SceneMenuPrincipal());
			Application::GetInstance()->SetScene(SCENEMENU);
			//Application::GetInstance()->RemoveScene(SCENECONTROLE);

		}
	}

	///\brief Fonction qui prend en parametres les temps et position pour pouvoir les creer les labels
	///\param time temps du joueur
	void GivePlayerData(double time) {
		totaltime_ = time;
	}


	void DeleteScene() {
		if (textfieldnom_->GetContent() != "") {
			leaderboard_->AddScore(textfieldnom_->GetContent(), totaltime_);
			leaderboard_->Reset();
		}

		while (!drawables.empty()) {
			delete drawables.front();
			drawables.pop_front();
		}
	}
};

unsigned int SceneResulat::currentsceneId_ = 0;

///\brief Fontion qui permet de retourner au menu principal
void ReturnToMenu() {
	SceneResulat::currentsceneId_ = SCENEMENU;
}


#endif // SCENERESULTATS


