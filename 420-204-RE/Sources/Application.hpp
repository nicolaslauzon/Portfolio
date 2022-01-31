///\file Application.hpp
///\brief Classe servant � g�rer le programme

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#if _WIN32
	#pragma comment(lib, "SDL2.lib")
	#pragma comment(lib, "SDL2main.lib")
	#pragma comment(lib, "SDL2_ttf.lib")
	#pragma comment(lib, "SDL2_image.lib")
	#pragma comment(lib, "opengl32.lib")
#endif

#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Singleton.hpp"
#include "Chrono.hpp"
#include "Scene.hpp"
#include "GlContext.hpp"

enum scene {

	// Voici les constantes pour tout les IDs de scenes test.
	SCENEMENU = 1,
	SCENEJEU = 2,
	SCENERESULTAT = 3,
	SCENECONTROLE = 4,
	SCENENICOLAS = 5,
	SCENEAZIZ = 6,
	SCENEELIE = 7,
	SCENEZACK = 8,
	SCENEGAB = 9,
	SCENERAFF = 10,
	SCENEPAT = 11,
	SCENEEXEMPLE = 12,
	SCENECHARGEMENT = 13,
	SCENEOPTIONS = 14
};

///\class Application
///\brief Gere la window et les scenes
class Application : public Singleton<Application> {
private:
	GlContext* window_;					///< Pointe sur la window
	map<unsigned int, Scene*> scenes_;	///< Contient toute les scenes et leurs identifiants
	scene currentScene_;				///< Scene active
	Chrono deltaT_;						///< Temps ecoule depuis le dernier appel de la fonction handleUpdate
	SDL_Event sdlEvent_;				///< Evenement a gerer

public:

	Application() {
		srand(time(nullptr));
		SDL_Init(SDL_INIT_EVERYTHING); 
		IMG_Init(IMG_INIT_PNG);
		TTF_Init();

		window_ = new GlContext("PI 2021", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 700, SDL_WINDOW_OPENGL);

		SDL_ShowCursor(false);

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, 1000.0, 700.0, 0.0, 1.0, -1.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

	}

	~Application() {
		if (window_)
			delete window_;			

		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}
	///\brief change la scene active
	///\param sceneId identifiant de la scene a afficher
	void SetScene(const scene& sceneId) {
		currentScene_ = sceneId;
		scenes_[currentScene_]->Init();
	}

	///\brief ajoute une scene aux scenes affichables
	///\param sceneId identifiant de la scene a ajouter
	///\param newScene pointeur de la scene a ajouter
	void AddScene(const scene& sceneId, Scene* newScene) {
		scenes_[sceneId] = newScene;
	}

	///\brief enleve une scene aux scenes affichables
	///\param sceneId identifiant de la scene a enlever
	void RemoveScene(const scene& sceneId) {
		delete scenes_[sceneId];
		scenes_[sceneId] = nullptr;
		scenes_.erase(sceneId);
	}

	///\brief fonction gerant tout le programme, qui roule tout le long du programme
	void Start() {
		bool isOpen = true;
		while (isOpen) {
			// Handle events
			while (SDL_PollEvent(&sdlEvent_)) {
				switch (sdlEvent_.type) {
					case SDL_QUIT:
							isOpen = false;
							break;						
					default:
						scenes_[currentScene_]->HandleEvent(sdlEvent_);
						break;
					}
			}

			// Handle updates and refresh
			scenes_[currentScene_]->HandleUpdate(deltaT_.GetElapsedSeconds());
			deltaT_.Reset();
			window_->Clear();
			scenes_[currentScene_]->HandleRefresh();
			window_->Refresh();
		}
	}

	///\brief retourne l'identifiant de la window
	inline Window* GetWindow() {
		return window_;
	}

	///\brief Ferme l'application
	void DeleteThis() {
		if (window_) {
			delete window_;
			window_ = nullptr;
		}
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	unsigned int loadTexture(const char* fileName) {
		unsigned int id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		SDL_Surface* sdlSurface = IMG_Load(fileName);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sdlSurface->w, sdlSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sdlSurface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		SDL_FreeSurface(sdlSurface);

		return id;
	}

	unsigned int loadTexture(SDL_Surface* sdlSurface) {
		unsigned int id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sdlSurface->w, sdlSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sdlSurface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		return id;
	}
};

#endif
