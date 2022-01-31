#ifndef TESTNICOLAS_HPP
#define TESTNICOLAS_HPP

#include <SDL2/SDL.h>

#include "Vector.hpp"
#include "Matrix44d.hpp"
#include "Character.hpp"
#include "Chrono.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "FileSystem.hpp"
#include "DebugHud.hpp"
#include "Texture.hpp"

class TestNicolas : public Scene {
private:
	TexturedMesh* mesh;
	StaticCamera* camera;
	Character* character;
	DebugHud debug;
	Objet3dColisionable* obj;

public:
	TestNicolas() {
	}


	void Init() {
		SDL_ShowCursor(1);
		((GlContext*)Application::GetInstance()->GetWindow())->SetDefaultPerspective(_3D);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		mesh = new TexturedMesh();
		mesh->load("Resources/Obj/atmosphere.obj");
		unsigned int id = Application::GetInstance()->loadTexture((FileSystem::imagesPath + "atmosphere.png").c_str());
		mesh->setTexture(id);

		id = Application::GetInstance()->loadTexture((FileSystem::imagesPath + "shell.png").c_str());
		character = new Character(ResourceManager::GetResource<TexturedMesh*>("bug"), id);

		camera = new StaticCamera({ 0.0,1.0,0.0 }, { 10.0,0.0,0.0 }, { 2.0, 0.0, 0.0 });
		// camera = new StaticCamera({ 1.0,0.0,0.0 }, { 0.0,30.0,0.0 }, { 2.0, 0.0, 0.0 });
		camera->ApplyView();
			
		id = Application::GetInstance()->loadTexture((FileSystem::imagesPath + "debugSpeed.png").c_str());
		id = Application::GetInstance()->loadTexture((FileSystem::imagesPath + "debugCenterMass.png").c_str());
		id = Application::GetInstance()->loadTexture((FileSystem::imagesPath + "debugSphere.png").c_str());
		id = Application::GetInstance()->loadTexture((FileSystem::imagesPath + "shell.png").c_str());

		// debug.AddDebug(character, character->GetSpeed());
		obj = new Objet3dColisionable(ResourceManager::GetResource<TexturedMesh*>("bug"), id);
		debug.AddDebug(obj);
	}

	void HandleEvent(SDL_Event sdlEvent) {
	  	character->Notification(sdlEvent);
		debug.Notification(sdlEvent);
	}

	void HandleUpdate(double deltaTime) {
		
		character->HandleUpdate(deltaTime);
		Matrix44d m;
		m.LoadIdentity();
		m.LoadRotation(Vector3d(0.0, 1.0, 0.0), deltaTime / 100);
		mesh->Transform(m);
		debug.HandleRefresh(deltaTime);
	}

	void HandleRefresh() {
		camera->ApplyView();
		mesh->Draw();
		// character->Draw();
		obj->Draw();
		debug.Draw();
	}
};


#endif // TESTNICOLAS_HPP