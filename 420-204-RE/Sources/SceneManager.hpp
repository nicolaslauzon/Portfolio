///\file SceneManager.hpp
///\brief Classe qui gere les scenes
#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include "Scene.hpp"
#include "Window.hpp"

class SceneManager {
private:
	Scene* currentscene;
	Scene* nextscene;

public:
	SceneManager() {
		currentscene = nullptr;
		nextscene = nullptr;
	}

	~SceneManager() {
		if (currentscene) {
			delete currentscene;
			currentscene = nullptr;
		}
		if (nextscene) {
			delete nextscene;
			nextscene = nullptr;
		}
	}
	void SetScene(Scene* scene) {
		if (currentscene != scene) {
			if (currentscene) {
				currentscene = nullptr;
				delete currentscene;
			}
			//scene->SetWindowId(sdlWindow); -> tout le temps une fenêtre
			currentscene = scene;
		}
	}

	void handleEvent(SDL_Event sdlEvent) {
		currentscene->HandleEvent(sdlEvent);
	}

	void handleUpdate(double deltaTime) {
		currentscene->HandleUpdate(deltaTime);

	}

	void handleRefresh() {
		currentscene->HandleRefresh();
	}
};

#endif //SCENEMANAGER_HPP