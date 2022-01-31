#ifndef TESTAZIZ_HPP
#define TESTAZIZ_HPP

#include "Scene.hpp"
#include "TextField.hpp"
#include "LabelList.hpp"
#include "LeaderboardHud.hpp"
#include "Objet3dDrawable.hpp"
#include "Camera.hpp"
#include <iostream>
#include "Character.hpp"
#include "Plateformes.hpp"

class TestAziz : public Scene {
private:

	TexturedMesh* mesh;
	StaticCamera* camera;
	Character* personnage2;
	Character* personnage;
	PlateformeIntermittente* plateforme;

public:
	TestAziz() {
	}



	///\brief appel� au d�but de la sc�ne
	///\param windowId identificateur de la fenetre
	void Init() {
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
		unsigned int id = Application::GetInstance()->loadTexture("Resources/Images/atmosphere.png");
		mesh->setTexture(id);

		id = Application::GetInstance()->loadTexture("Resources/Images/powerUpBase.png");
		//personnage = new Character(ResourceManager::GetResource<TexturedMesh*>("bug"), id);
		//personnage2 = new Character(ResourceManager::GetResource<TexturedMesh*>("bug"), id);
		TexturedMesh* mesh2 = new TexturedMesh();
		mesh2->load("PlateformeIntermittente.obj");
		plateforme = new PlateformeIntermittente(mesh2,id);

		camera = new StaticCamera({ 0.0,1.0,0.0 }, { 2.0,0.0,0.0 }, { 0.0, 0.0, 0.0 });
		camera->ApplyView();
		

		SubscribeEvent(SDLK_SPACE, personnage);
		SubscribeEvent(SDL_KEYUP, personnage);
		SubscribeEvent(SDL_KEYDOWN, personnage);


	}

	///\brief �venements
	///\param sdlEvent �venement � g�rer
	void HandleEvent(SDL_Event sdlEvent) {
		//personnage->Notification(sdlEvent);
	}

	///\brief G�rer les modifications
	///\param deltaTime temps �coul� depuis le dernier appel de cette fonction
	void HandleUpdate(double deltaTime) {
		//std::cout<<Physic::BoundingSphereCollisionTest(personnage->GetRadius(), personnage2->GetRadius(), personnage->GetPosition(), personnage2->GetPosition()) << std::endl;
		//personnage->HandleUpdate(deltaTime);
		Matrix44d m;
		m.LoadIdentity();
		m.LoadRotation(Vector3d(0.0, 1.0, 0.0), deltaTime / 100);
		mesh->Transform(m);
		plateforme->HandleUpdate(deltaTime);
	
	}

	///\brief affichage
	void HandleRefresh() {
		plateforme->HandleRefresh();
		mesh->Draw();
		//personnage->Draw();
		//personnage2->Draw();
	}
	
};


#endif