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
	double totaltime_ = 0; ///< Temps total du joueur
	TTF_Font* font_; ///< Le font
	TTF_Font* font2_; ///< Le font du leaderboard
	Image* background_; ///< Image de background
	Label* total_; ///< Label du temps total
	Label* segment1time_; ///< Label du temps du premier segment
	Label* segment2time_; ///< Label du temps du premier segment
	Label* segment3time_; ///< Label du temps du premier segment
	Label* position_; ///< Position du joueur
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
		currentsceneId_ = SCENERESULTAT;

		font_ = TTF_OpenFont("font.ttf", 30);
		font2_ = TTF_OpenFont("font.ttf", 20);
		background_ = new Image(*(ResourceManager::GetResource<unsigned int*>("resultats")), { 0,0,0,0 }, true);
		//La ressource n'est actuellement pas charg�e, c'est pour cela que la classe ne l'affichera pas dans cet �tat. Lorsque SceneChargement sera utilis�e avant, l'affichage fonctionnera
		
		background_->SetSize(1000, 700);
		drawables.push_back(background_);

		escapeRect = new ColorButton({ 1,1,110,43 }, { 255,255,255,255 }, { 255, 255, 255, 155 });
		escapeRect->setOnClick(ReturnToMenu);
		drawables.push_back(escapeRect);

		Label* escapeLabel = new Label({ 4, 1,100,40 }, ResourceManager::GetResource<TTF_Font*>("font20"), { 0,0,0,255 }, "Escape");
		drawables.push_back(escapeLabel);

		leaderboard_ = new LeaderboardHud("Leaderboard.txt", font2_, { 255,255,255,255 }, { 750,163,0,0 },3,90);
		drawables.push_back(leaderboard_);
		if (leaderboard_->GetTime(leaderboard_->GetNumberofPlayers()) > totaltime_ || leaderboard_->GetNumberofPlayers() <= 10) {
			textfieldnom_ = new TextField({ 717, 580, 230, 200 }, font_, { 25, 0, 0, 255 }, { 255, 255, 255, 255 });
			drawables.push_back(textfieldnom_);
			SubscribeEvent(SDL_KEYDOWN, textfieldnom_);
			SubscribeEvent(SDL_MOUSEMOTION, textfieldnom_);
			SubscribeEvent(SDL_MOUSEBUTTONDOWN, textfieldnom_);

		}

		Cursor::GetInstance()->Init(*(ResourceManager::GetResource<unsigned int*>("cursor")), { 0,0,0,0 });
		//La ressource n'est actuellement pas charg�e, c'est pour cela que la classe ne l'affichera pas dans cet �tat. Lorsque SceneChargement sera utilis�e avant, l'affichage fonctionnera
		SubscribeEvent(SDL_MOUSEMOTION, Cursor::GetInstance());
		drawables.push_back(Cursor::GetInstance());
		segment1time_ = new Label({ 200, 195,0,0 }, font_, { 255,255,255,255 }, "1X:XX");
		drawables.push_back(segment1time_);
		segment2time_ = new Label({ 200, 240, 0, 0 }, font_, { 255,255,255,255 }, "2X:XX");
		drawables.push_back(segment2time_);
		segment3time_ = new Label({ 200, 280, 0, 0 }, font_, { 255,255,255,255 }, "3X:XX");
		drawables.push_back(segment3time_);
		total_ = new Label({ 250, 420, 0, 0 }, font_, { 255,255,255,255 }, /*to_string(totaltime_).c_str()*/ "TX:XX");
		drawables.push_back(total_);

		position_ = new Label( { 400, 300, 0, 0 }, font_, { 255,255,255,255 }, "Xe");
		drawables.push_back(position_);
		
	}

	///\brief Gestion des �v�nements dans la sc�ne
	///\param sdlEvent Evenement transmis
	void HandleEvent(SDL_Event sdlEvent) {
		textfieldnom_->Notification(sdlEvent);
		Cursor::GetInstance()->Notification(sdlEvent);
		escapeRect->Notification(sdlEvent);

	}

	///\brief Gestion des updates
	void HandleUpdate(double deltaTime) {

	}
	///\brief Gestion de l'affichage
	void HandleRefresh() {
		for (auto it : drawables) {
			it->Draw();
		}
		if (SceneResulat::currentsceneId_ == SCENEMENU) {
			//Application::GetInstance()->AddScene(SCENEMENU, new SceneMenuPrincipal());
			Application::GetInstance()->SetScene(SCENEMENU);
			//Application::GetInstance()->RemoveScene(SCENECONTROLE);

		}
	}

	///\brief Fonction qui prend en parametres les temps et position pour pouvoir les creer les labels
	///\param segment1time_ temps du premier segment
	///\param segment2time_ temps du deuxieme segment
	///\param segment3time_ temps du troisieme segment
	///\param position position du joueur
	void GivePlayerData(double segment1time, double segment2time, double segment3time, unsigned int position) {
		totaltime_ = segment1time + segment2time + segment3time;
		this->segment1time_ = new Label({ 500, 200, 0, 0 }, font_, { 255,255,255,255 }, (to_string(segment1time)).c_str());
		this->segment2time_ = new Label({ 450, 200, 0, 0 }, font_, { 255,255,255,255 }, (to_string(segment2time)).c_str());
		this->segment3time_ = new Label({ 400, 200, 0, 0 }, font_, { 255,255,255,255 }, (to_string(segment3time)).c_str());
	}

};

unsigned int SceneResulat::currentsceneId_ = 0;

///\brief Fontion qui permet de retourner au menu principal
void ReturnToMenu() {
	SceneResulat::currentsceneId_ = SCENEMENU;
}


#endif // SCENERESULTATS


