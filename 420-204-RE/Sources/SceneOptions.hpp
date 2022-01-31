#ifndef SCENEOPTIONS_HPP
#define SCENEOPTIONS_HPP

#if _WIN32
	#include <Windows.h>
#else
	#include <unistd.h>
#endif

#include <fstream>

#include "Scene.hpp"
#include "CheckBox.hpp"

void FonctionBoutonResolutionA();
void FonctionBoutonResolutionB();
void FonctionBoutonResolutionC();

class SceneOptions : public Scene {
private:
	ColorCheckBox* fullscreen;

public:
	static bool isFullScreen;

	~SceneOptions() {
		ClearDrawableList();
	}

	void Init() {
		SDL_Point windowSize = Application::GetInstance()->GetWindow()->GetSize();

		AddDrawable(new Image(*(ResourceManager::GetResource<unsigned int*>("menuBackground")), { 0, 0, windowSize.x, windowSize.y }, false));

		AddDrawable(new Label({ windowSize.x / 2 - 158, 30, 0, 0 }, ResourceManager::GetResource<TTF_Font*>("font50"), { 0, 0, 0, 255 }, "Resolution"));

		Button* resolutionAButton = new ColorButton({ windowSize.x / 2 - 215, 103, 375, 65 }, { 245, 201, 0, 255 }, { 0, 230, 175, 255 });
		resolutionAButton->setOnClick(FonctionBoutonResolutionA);
		SubscribeEvent(SDL_MOUSEMOTION, resolutionAButton);
		SubscribeEvent(SDL_MOUSEBUTTONUP, resolutionAButton);
		AddDrawable(resolutionAButton);
		AddDrawable(new Label({ windowSize.x / 2 - 155, 100, 0, 0 }, ResourceManager::GetResource<TTF_Font*>("font50"), { 0,0,0,255 }, "1000 x 700"));

		Button* resolutionBButton = new ColorButton({ windowSize.x / 2 - 215, 183, 375, 65 }, { 245, 201, 0, 255 }, { 0, 230, 175, 255 });
		resolutionBButton->setOnClick(FonctionBoutonResolutionB);
		SubscribeEvent(SDL_MOUSEMOTION, resolutionBButton);
		SubscribeEvent(SDL_MOUSEBUTTONUP, resolutionBButton);
		AddDrawable(resolutionBButton);
		AddDrawable(new Label({ windowSize.x / 2 - 155, 180, 0, 0 }, ResourceManager::GetResource<TTF_Font*>("font50"), { 0,0,0,255 }, "1280 x 720"));

		Button* resolutionCButton = new ColorButton({ windowSize.x / 2 - 215, 263, 375, 65 }, { 245, 201, 0, 255 }, { 0, 230, 175, 255 });
		resolutionCButton->setOnClick(FonctionBoutonResolutionC);
		SubscribeEvent(SDL_MOUSEMOTION, resolutionCButton);
		SubscribeEvent(SDL_MOUSEBUTTONUP, resolutionCButton);
		AddDrawable(resolutionCButton);
		AddDrawable(new Label({ windowSize.x / 2 - 165, 260, 0, 0 }, ResourceManager::GetResource<TTF_Font*>("font50"), { 0,0,0,255 }, "1920 x 1080"));

		fullscreen = new ColorCheckBox(windowSize.x / 2 - 215, 500, 50, 50, { 100, 100, 100, 255 }, { 200, 200, 200, 255 });
		SubscribeEvent(SDL_MOUSEBUTTONUP, fullscreen);
		AddDrawable(new Label({ windowSize.x / 2 - 125, 492, 0, 0 }, ResourceManager::GetResource<TTF_Font*>("font50"), { 0, 0, 0 ,255 }, "Plein ecran"));
		AddDrawable(fullscreen);

		Cursor::GetInstance()->Init(*(ResourceManager::GetResource<unsigned int*>("cursor")), {0,0,0,0});
		SubscribeEvent(SDL_MOUSEMOTION, Cursor::GetInstance());
	}

	void HandleEvent(SDL_Event sdlEvent) {
		if (eventDispatcher_.find(sdlEvent.type) != eventDispatcher_.end())
			eventDispatcher_[sdlEvent.type]->Notify(sdlEvent);
	}

	void HandleUpdate(double deltaTime) {
		isFullScreen = fullscreen->isChecked();
	}

	void HandleRefresh() {
		RenderAll();
		Cursor::GetInstance()->Draw();
	}
};

bool SceneOptions::isFullScreen = false;

void applyResolution(const int& width, const int& height) {
	ofstream writer(FileSystem::resourcePath + "config", ifstream::out);
	writer << width << ' ' << height << ' ' << SceneOptions::isFullScreen;
	writer.close();

	// Pour Visual Studio
	ofstream vswriter("config", ifstream::out);
	vswriter << width << ' ' << height << ' ' << SceneOptions::isFullScreen;
	vswriter.close();

	SDL_Event sdlEvent;
	sdlEvent.type = SDL_QUIT;
	SDL_PushEvent(&sdlEvent);

	#if _WIN32
		system("START /B ..\\x64\\Debug\\PI2021.exe");
	#else
		system("./PI2021 &");
	#endif
}

void FonctionBoutonResolutionA() { applyResolution(1000, 700); }
void FonctionBoutonResolutionB() { applyResolution(1280, 720); }
void FonctionBoutonResolutionC() { applyResolution(1920, 1080); }

#endif
