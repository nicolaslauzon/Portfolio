#ifndef TESTELIE_HPP
#define TESTELIE_HPP

#define DEMIPI M_PI/2
#define RATIO 2.0

#include "Scene.hpp"
#include "TextField.hpp"
#include "LabelList.hpp"
#include "LeaderboardHud.hpp"
#include "Objet3dDrawable.hpp"
#include "Camera.hpp"
#include "Matrix44d.hpp"
#include "Character.hpp"
#include "DebugHud.hpp"
#include "Sprite3dDrawable.hpp"
#include "Plateformes.hpp"
#include "Ventilateur.hpp"
#include "Quiz.hpp"
#include "Aimant.hpp"
#include "Checkpoint.hpp"
#include "Capsules.hpp"

enum CameraType { Dynamic, Flying };

class TestElie : public Scene {
private:
	Character* charac;

	Quiz* quiz_;
	Objet3dDrawable* atmosphere_;
	Trampoline* trampoline_;
	//PlateformeIntermittente* plateformeintermittente_;
	PlateformeMouvantes* plateformemouv1_;
	// Objet3dDrawable* plateforme1_;
	// Objet3dDrawable* couloirporte1_;
	// Objet3dDrawable* couloirporte2_;
	// Objet3dDrawable* couloirporte3_;
	// Objet3dDrawable* couloirporte4_;
	// Objet3dDrawable* couloirporte5_;
	// Objet3dDrawable* couloirporte6_;

	// Objet3dDrawable* plateformeCheckpoint1_;

	Checkpoints* checkpoints_;	

	Objet3dDrawable* couloirVentilateur1_;
	Objet3dDrawable* couloirVentilateur2_;
	Objet3dDrawable* couloirVentilateur3_;

	Ventilateur* Ventilateur1_;
	Ventilateur* Ventilateur2_;
	Ventilateur* Ventilateur3_;

	Aimant* aimant1_;

	// Objet3dDrawable* couloirVentilateur1_;
	// Objet3dDrawable* couloirVentilateur2_;
	// Objet3dDrawable* couloirVentilateur3_;
	//Objet3dDrawable* Ventilateur1_;

	// Objet3dDrawable* plateformejump1_;
	// Objet3dDrawable* plateformejump2_;
	// Objet3dDrawable* plateformejump3_;

	// Objet3dDrawable* couloirParkour1_;
	// Objet3dDrawable* plateformeParkour1_;
	// Objet3dDrawable* couloirParkour2_;
	// Objet3dDrawable* couloirParkour3_;

	// Objet3dDrawable* couloirCheckpoint2_;

	// Objet3dDrawable* couloirpendule1_;
	// Objet3dDrawable* couloirpendule2_;
	// Objet3dDrawable* couloirpendule3_;

	Objet3dDrawable* pendule1_;
	Objet3dDrawable* pendule2_;
	Objet3dDrawable* pendule3_;

	// Objet3dDrawable* plateformeMouvante1_;
	 Objet3dDrawable* plateformeMouvante2_;
	// Objet3dDrawable* plateformeMouvante3_;

	// Objet3dDrawable* couloirCheckpoint3_;

	



	Labyrinthe* labyrinthe_;

	Sprite3Ddrawable* cube_;

	Sprite3Ddrawable* bug;

	CameraType camType;
	FlyingCamera* fcam;
	DynamicCamera* dcam;
	
	unsigned int meshtexid;
	double penduletime;
	double animtime_;

	DebugHud* debugHud_;

	list<PlateformeStatique*> plateformes_;
	list<Capsule*> capsules_;

public:
	~TestElie() {
		UnsubscribeEvent(SDL_MOUSEBUTTONDOWN, debugHud_);
		UnsubscribeEvent(SDL_KEYDOWN, debugHud_);

		delete debugHud_;

		if (camType == Flying) {
			UnsubscribeEvent(SDL_MOUSEMOTION, fcam);
			UnsubscribeEvent(771, fcam);
			UnsubscribeEvent(SDL_KEYUP, fcam);
		}
		else {
			UnsubscribeEvent(SDL_MOUSEMOTION, charac);
			UnsubscribeEvent(771, charac);
			UnsubscribeEvent(SDL_KEYUP, charac);
		}
	}

	///\brief Initialisation de la scène
	void Init() {
		penduletime = 0;
		((GlContext*)Application::GetInstance()->GetWindow())->SetDefaultPerspective(_3D);
		
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		GLfloat lightColor[4] = { 0.9, 0.9, 0.9, 1.0 };
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
		float lightPos[3] = { 1000.0, 1000.0, 0.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		debugHud_ = new DebugHud();
				
		SDL_Surface* atmosphereSurface = IMG_Load((FileSystem::imagesPath + "atmosphere.png").c_str());

		// Chargement des nuages
		SDL_Surface* cloudsSurface[8];
		for (unsigned char i = 0; i < 8; i++)
			cloudsSurface[i] = IMG_Load((FileSystem::imagesPath + "cloud" + to_string(i + 1) + ".png").c_str());

		// Application des nuages
		int randomCloud, cloudCount = rand() % 30;
		SDL_Rect cloudRect;
		for (unsigned char i = 0; i < 20; i++) {
			randomCloud = rand() % 8;
			cloudRect = { rand() % 1700, rand() % 500 + 300, cloudsSurface[randomCloud]->w, cloudsSurface[randomCloud]->h };
			SDL_BlitSurface(cloudsSurface[randomCloud], nullptr, atmosphereSurface, &cloudRect);
		}
		ResourceManager::AddResource("atmosphereT", new Texture(Application::GetInstance()->loadTexture(atmosphereSurface)));

		// Libération des nuages
		for (unsigned char i = 0; i < 8; i++)
			SDL_FreeSurface(cloudsSurface[i]);
		SDL_FreeSurface(atmosphereSurface);

		atmosphere_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("atmosphereM"), *ResourceManager::GetResource<unsigned int*>("atmosphereT"));

		//bug = new Sprite3Ddrawable(ResourceManager::GetResource<Sprite3D*>("bugSprite"), *ResourceManager::GetResource<unsigned int*>("shell"));
		
		charac = new Character(ResourceManager::GetResource<Sprite3D*>("bugSprite"), *ResourceManager::GetResource<unsigned int*>("shell"));

		Matrix44d matrix;
		matrix.LoadIdentity();
		matrix.Translate({ 0.0,1.0 * RATIO,0.0 });
		charac->Transform(matrix);
		// charac->SetPosition(Vector3d(0.0, 7.0 * RATIO, 0.0) + charac->GetPosition().GetOrigin());
		charac->SetPosition(Vector3d(0.0, 1.0 * RATIO, 0.0) + charac->GetPosition().GetOrigin());
		//bug->Transform(matrix);
		//bug->SetDelay(0.1);
		debugHud_->AddDebug(charac, charac->GetSpeed());
		SubscribeEvent(SDL_MOUSEBUTTONDOWN, debugHud_);
		SubscribeEvent(SDL_MOUSEBUTTONUP, debugHud_);
		SubscribeEvent(SDL_MOUSEMOTION, debugHud_);
		SubscribeEvent(SDL_KEYDOWN, debugHud_);

		//quiz_ = new Quiz();
		//SubscribeEvent(SDL_KEYDOWN, quiz_);
		//SubscribeEvent(SDL_KEYUP, quiz_);

		checkpoints_ = new Checkpoints();

		camType = Dynamic;
		dcam = new DynamicCamera(charac);
		fcam = new FlyingCamera({ 0.0, 1.0, 0.0 }, { 10.0, 10.0, 10.0 }, { 0.0, 0.0, 0.0 });
		SubscribeEvent(SDL_MOUSEMOTION, charac);
		SubscribeEvent(771, charac);
		SubscribeEvent(SDL_KEYUP, charac);

		PlateformeStatique* plateforme1 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("surfacePlane"));
		plateformes_.push_back(plateforme1);

		plateformemouv1_ = new PlateformeMouvantes(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("surfacePlane"), { 0.0,0.0,1.0 }, 20 * RATIO,2.0);
		plateformemouv1_->Translate({ -4.0 * RATIO,0.0,-10.0* RATIO });
		

		//plateformeintermittente_ = new PlateformeIntermittente(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("surfacePlane"));
		//plateformeintermittente_->Translate({ -4.0 * RATIO,0.0,0.0 * RATIO });

		trampoline_ = new Trampoline(ResourceManager::GetResource<TexturedMesh*>("trampoline"), *ResourceManager::GetResource<unsigned int*>("surfacePlane"));
		trampoline_->Scale({ 5.0,5.0,5.0 });
		trampoline_->Translate({ -7.0 * RATIO,0.0,0.0 * RATIO });
		

		PlateformeStatique* couloirporte1 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirporte1->Translate({ 4.0 * RATIO,0.0,0.0 });
		plateformes_.push_back(couloirporte1);

		PlateformeStatique* couloirporte2 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirporte2->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirporte2->Translate({ 11.0 * RATIO,0.0,0.0 });
		plateformes_.push_back(couloirporte2);
		
		PlateformeStatique* couloirporte3 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirporte3->Translate({ 18.0 * RATIO,0.0,0.0 });
		plateformes_.push_back(couloirporte3);

		PlateformeStatique* couloirporte4 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirporte4->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirporte4->Translate({ 25.0 * RATIO,0.0,0.0 });
		plateformes_.push_back(couloirporte4);

		PlateformeStatique* couloirporte5 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirporte5->Translate({ 32.0 * RATIO,0.0,0.0 });
		plateformes_.push_back(couloirporte5);

		PlateformeStatique* couloirporte6 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirporte6->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirporte6->Translate({ 39 * RATIO,0.0,0.0 });
		plateformes_.push_back(couloirporte6);

		PlateformeStatique* plateformeCheckpoint1 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		plateformeCheckpoint1->Translate({ 46.0 * RATIO,0.0,0.0 });
		plateformes_.push_back(plateformeCheckpoint1);

		PlateformeStatique* couloirVentilateur1 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirVentilateur1->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirVentilateur1->Translate({ 53.0 * RATIO,0.0,0.0 });
		plateformes_.push_back(couloirVentilateur1);

		PlateformeStatique* couloirVentilateur2 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirVentilateur2->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirVentilateur2->Translate({ 63.0 * RATIO,0.0,0.0 });
		plateformes_.push_back(couloirVentilateur2);
		
		PlateformeStatique* couloirVentilateur3 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirVentilateur3->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirVentilateur3->Translate({ 73.0 * RATIO,0.0,0.0 });
		plateformes_.push_back(couloirVentilateur3);
		
		Ventilateur1_ = new Ventilateur(ResourceManager::GetResource<TexturedMesh*>("ventilateurM"), *ResourceManager::GetResource<unsigned int*>("couloir"), { 56.0*RATIO,2.5*RATIO,5.0*RATIO });
		Matrix44d rotMatrix;
		rotMatrix.LoadIdentity();
		rotMatrix.LoadTransformationOrigin(Ventilateur1_->GetPosition().GetOrigin());
		rotMatrix.LoadRotation({ 0.0,1.0,0.0 }, DEMIPI);
		Ventilateur1_->Transform(rotMatrix);
		Ventilateur1_->SetXDirection({ 0.0,0.0,1.0 });
		
		//Ventilateur1_->Translate({ 56.0 * RATIO,2.5 * RATIO,5.0 * RATIO });
		
		Ventilateur2_ = new Ventilateur(ResourceManager::GetResource<TexturedMesh*>("ventilateurM"), *ResourceManager::GetResource<unsigned int*>("couloir"), { 63.0 * RATIO,2.5 * RATIO,5.0 * RATIO });
		rotMatrix.LoadIdentity();
		rotMatrix.LoadTransformationOrigin(Ventilateur2_->GetPosition().GetOrigin());
		rotMatrix.LoadRotation({ 0.0,1.0,0.0 }, DEMIPI);
		Ventilateur2_->Transform(rotMatrix);
		Ventilateur2_->SetXDirection({ 0.0,0.0,1.0 });

		Ventilateur3_ = new Ventilateur(ResourceManager::GetResource<TexturedMesh*>("ventilateurM"), *ResourceManager::GetResource<unsigned int*>("couloir"), { 70.0 * RATIO,2.5 * RATIO,5.0 * RATIO });
		rotMatrix.LoadIdentity();
		rotMatrix.LoadTransformationOrigin(Ventilateur3_->GetPosition().GetOrigin());
		rotMatrix.LoadRotation({ 0.0,1.0,0.0 }, DEMIPI);
		Ventilateur3_->Transform(rotMatrix);
		Ventilateur3_->SetXDirection({ 0.0,0.0,1.0 });


		aimant1_= new Aimant(ResourceManager::GetResource<TexturedMesh*>("magnetM"), *ResourceManager::GetResource<unsigned int*>("magnetT"), { 10.0 * RATIO,2 * RATIO,5.0 * RATIO });
		rotMatrix.LoadIdentity();
		rotMatrix.LoadTransformationOrigin(aimant1_->GetPosition().GetOrigin());
		rotMatrix.LoadRotation({ 0.0,1.0,0.0 }, M_PI);
		aimant1_->Transform(rotMatrix);
		aimant1_->SetXDirection({ 0.0,0.0,1.0 });

		PlateformeStatique* plateformejump1 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("surfacePlaneM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		plateformejump1->Translate({ 82.0 * RATIO,0.0,-3.0 * RATIO });
		plateformes_.push_back(plateformejump1);

		PlateformeStatique* plateformejump2 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		plateformejump2->Translate({ 88.0 * RATIO,0.0,2.0 * RATIO });
		plateformes_.push_back(plateformejump2);

		PlateformeStatique* plateformejump3 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		plateformejump3->Translate({ 95.0 * RATIO,0.0,0.0 * RATIO });
		plateformes_.push_back(plateformejump3);

		PlateformeStatique* couloirParkour1 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirParkour1->Rotate({ 0.0,0.0,1.0 }, DEMIPI);
		couloirParkour1->Translate({ 97.5 * RATIO,-1.5 * RATIO,4.0 * RATIO });
		plateformes_.push_back(couloirParkour1);

		PlateformeStatique* plateformeParkour1 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		plateformeParkour1->Rotate({ 0.0,0.0,1.0 }, DEMIPI);
		plateformeParkour1->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		plateformeParkour1->Translate({ 100.0 * RATIO,-1.5 * RATIO,8.5 * RATIO });
		plateformes_.push_back(plateformeParkour1);

		PlateformeStatique* couloirParkour2 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirParkour2->Rotate({ 0.0,0.0,1.0 }, DEMIPI);
		couloirParkour2->Translate({ 102.5 * RATIO,-1.5 * RATIO,4.0 * RATIO });
		plateformes_.push_back(couloirParkour2);

		PlateformeStatique* couloirParkour3 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirParkour3->Rotate({ 0.0,0.0,1.0 }, DEMIPI);
		couloirParkour3->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirParkour3->Translate({ 108.0 * RATIO,-1.5 * RATIO,0.0});
		plateformes_.push_back(couloirParkour3);

		PlateformeStatique* couloirCheckpoint2 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirCheckpoint2->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirCheckpoint2->Translate({ 118.0 * RATIO,0.0,0.0 });
		plateformes_.push_back(couloirCheckpoint2);

		PlateformeStatique* couloirpendule1 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirpendule1->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirpendule1->Translate({ 128.0 * RATIO,0.0,0.0 });
		plateformes_.push_back(couloirpendule1);

		PlateformeStatique* couloirpendule2 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirpendule2->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirpendule2->Translate({ 138.0 * RATIO,0.0,0.0 });
		plateformes_.push_back(couloirpendule2);

		PlateformeStatique* couloirpendule3 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirpendule3->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirpendule3->Translate({ 148.0 * RATIO,0.0,0.0 });
		plateformes_.push_back(couloirpendule3);

		pendule1_ = new Objet3dColisionable(ResourceManager::GetResource<TexturedMesh*>("penduleM"), *ResourceManager::GetResource<unsigned int*>("pendule"));
		pendule1_->Rotate({ 0.0,0.0,1.0 }, -DEMIPI);
		pendule1_->Translate({ 125.0 * RATIO,5.5 * RATIO,4.5 * RATIO });

		pendule2_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("penduleM"), *ResourceManager::GetResource<unsigned int*>("pendule"));
		pendule2_->Rotate({ 0.0,0.0,1.0 }, -DEMIPI);;
		pendule2_->Translate({ 135.0 * RATIO,5.5 * RATIO,4.5 * RATIO });

		pendule3_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("penduleM"), *ResourceManager::GetResource<unsigned int*>("pendule"));
		pendule3_->Rotate({ 0.0,0.0,1.0 }, -DEMIPI);
		pendule3_->Translate({ 145.0 * RATIO,5.5 * RATIO,4.5 * RATIO });

		PlateformeStatique* plateformeMouvante1 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		plateformeMouvante1->Translate({ 156.0 * RATIO,0.0,0.0 });
		plateformes_.push_back(plateformeMouvante1);

		PlateformeStatique* plateformeMouvante2 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		plateformeMouvante2->Translate({ 161.0 * RATIO,0.0,0.0 });
		plateformes_.push_back(plateformeMouvante2);

		PlateformeStatique* plateformeMouvante3 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		plateformeMouvante3->Translate({ 166.0 * RATIO,0.0,0.0 });
		plateformes_.push_back(plateformeMouvante3);

		PlateformeStatique* couloirCheckpoint3 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirCheckpoint3->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirCheckpoint3->Translate({ 174.0 * RATIO, 0.0, 0.0 });
		plateformes_.push_back(couloirCheckpoint3);

		labyrinthe_ = new Labyrinthe();
		//labyrinthe_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		labyrinthe_->Rotate({0.0, 1.0, 0.0}, -DEMIPI);
		labyrinthe_->Translate({ 267.0 * RATIO, 0.0 * RATIO, 35.0 * RATIO });
		// charac->SetPosition({ 267.0 * RATIO, RATIO, 35.0 * RATIO });
		// charac->Translate({ 267.0 * RATIO, RATIO, 35.0 * RATIO });
		// dcam = new DynamicCamera(charac);

		PlateformeStatique* pisteGlissante = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("pisteM"), *ResourceManager::GetResource<unsigned int*>("pisteGlissante"));
		pisteGlissante->Rotate({ 1.0,0.0,0.0 }, DEMIPI);
		pisteGlissante->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		pisteGlissante->Translate({ 181 * RATIO, 0.0 * RATIO, 0.0 });
		plateformes_.push_back(pisteGlissante);
		

		plateformeMouvante2_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		plateformeMouvante2_->Translate({ 161.0 * RATIO,0.0,0.0 });

		/*Capsule* capSpeed_ = new SpeedCapsule(ResourceManager::GetResource<TexturedMesh*>("capsuleM"), *ResourceManager::GetResource<unsigned int*>("capsuleT"), 10.0, 1.5);
		capSpeed_->SetOrigin({ 5 * RATIO, 1.0 * RATIO, 0.0 });
		capSpeed_->Translate({ 5 * RATIO, 1.0 * RATIO, 0.0 });
		capsules_.push_back(capSpeed_);
		Capsule* capJump_ = new JumpCapsule(ResourceManager::GetResource<TexturedMesh*>("capsuleM"), *ResourceManager::GetResource<unsigned int*>("capsuleT"), 10.0, 5.0);
		capJump_->SetOrigin({ 10 * RATIO, 1.0 * RATIO, 0.0 });
		capJump_->Translate({ 10 * RATIO, 1.0 * RATIO, 0.0 });
		capsules_.push_back(capJump_);
		*/
		Capsule* capTP_ = new TeleportationCapsule(ResourceManager::GetResource<TexturedMesh*>("capsuleM"), *ResourceManager::GetResource<unsigned int*>("capsuleT"), { 20 * RATIO, 1.0 * RATIO, 0.0 });
		capTP_->SetOrigin({ 5 * RATIO, 2.0 * RATIO, 0.0 });
		capTP_->Translate({ 5 * RATIO, 2.0 * RATIO, 0.0 });
		capsules_.push_back(capTP_);
		// PlateformeStatique* plateformeMouvante2 = new PlateformeStatique(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		// plateformeMouvante2->Translate({ 161.0 * RATIO,0.0,0.0 });
		// plateformes_.push_back(plateformeMouvante2);
	}

	///\brief Gestion d'événement
	///\param sdlEvent Événement à gérer
	void HandleEvent(SDL_Event sdlEvent) {
		if (eventDispatcher_.find(sdlEvent.type) != eventDispatcher_.end())
			eventDispatcher_[sdlEvent.type]->Notify(sdlEvent);						

		if (sdlEvent.type == SDL_KEYUP)
			switch (sdlEvent.key.keysym.sym) {
			case '1':
				if (camType == Flying) {
					UnsubscribeEvent(SDL_MOUSEMOTION, fcam);
					UnsubscribeEvent(771, fcam);
					UnsubscribeEvent(SDL_KEYUP, fcam);

					SubscribeEvent(SDL_MOUSEMOTION, charac);
					SubscribeEvent(771, charac);
					SubscribeEvent(SDL_KEYUP, charac);
					camType = Dynamic;
				}
				break;
			case '2':
				if (camType == Dynamic) {
					
					UnsubscribeEvent(SDL_MOUSEMOTION, charac);
					UnsubscribeEvent(771, charac);
					UnsubscribeEvent(SDL_KEYUP, charac);

					SubscribeEvent(SDL_MOUSEMOTION, fcam);
					SubscribeEvent(771, fcam);
					SubscribeEvent(SDL_KEYUP, fcam);
					camType = Flying;
				}
				break;
			}
	}

	///\brief G�rer les modifications
	///\param deltaTime temps �coul� depuis le dernier appel de cette fonction
	void HandleUpdate(double deltaTime) {	
		//bug->UpdateFrame(deltaTime);

		bool r;
		for (auto it : plateformes_) {
			r = it->CollisionWithPlatform(charac);
			if (r) {
				break;
			}
		}

		r = r || labyrinthe_->CollisionWithPlatform(charac);
		if (r == false) {
			charac->SetJump(true);
		}

		EffectManager::ExpirationTest();

		for (auto it : capsules_) {
			it->Collision(charac);
		}

		switch (camType) {
		case Dynamic:
			dcam->Move(deltaTime);
			break;

		case Flying:
			fcam->Move(deltaTime);
		}

		checkpoints_->HandleUpdate(charac);	

		//charac->UpdateFrame(deltaTime);
		charac->HandleUpdate(deltaTime);
		//Ventilateur1_->HandleUpdate(deltaTime);
		//Ventilateur1_->Translate({ -55.0 * RATIO,-2.5 * RATIO,-5.0 * RATIO });
		plateformemouv1_->SetCharacter(charac);
		plateformemouv1_->HandleUpdate(deltaTime);
		

		aimant1_->CheckEffect(charac,deltaTime);

		Ventilateur1_->HandleUpdate(deltaTime);
		Ventilateur1_->CheckEffect(charac, deltaTime);
		Ventilateur2_->HandleUpdate(deltaTime);
		Ventilateur2_->CheckEffect(charac, deltaTime);
		Ventilateur3_->HandleUpdate(deltaTime);
		Ventilateur3_->CheckEffect(charac, deltaTime);
		//Ventilateur1_->Translate({ -55.0 * RATIO,-2.5 * RATIO,-5.0 * RATIO });
		//Ventilateur2_->Translate({ -63.0 * RATIO,-2.5 * RATIO,-5.0 * RATIO });
		//Ventilateur3_->Translate({ -71.0 * RATIO,-2.5 * RATIO,-5.0 * RATIO });

		//Ventilateur1_->Rotate({ 0.0, 0.0, 1.0 }, deltaTime*15);
		//Ventilateur2_->Rotate({ 0.0, 0.0, 1.0 }, deltaTime*15);
		//Ventilateur3_->Rotate({ 0.0, 0.0, 1.0 }, deltaTime*15);

		//Ventilateur1_->Translate({ 55.0 * RATIO,2.5 * RATIO,5.0 * RATIO });
		//Ventilateur2_->Translate({ 63.0 * RATIO,2.5 * RATIO,5.0 * RATIO });
		//Ventilateur3_->Translate({ 71.0 * RATIO,2.5 * RATIO,5.0 * RATIO });

		// TODO: À mieux rotationer
		pendule1_->Translate({ -125.0 * RATIO,-5.5 * RATIO,0.0 });
		pendule2_->Translate({ -135.0 * RATIO,-5.5 * RATIO,0.0 });
		pendule3_->Translate({ -145.0 * RATIO,-5.5 * RATIO,0.0 });

		penduletime += deltaTime;
		if (penduletime < M_PI) {
			pendule1_->Rotate({ 1.0, 0.0, 0.0 }, -deltaTime);
			pendule2_->Rotate({ 1.0, 0.0, 0.0 }, -deltaTime);
			pendule3_->Rotate({ 1.0, 0.0, 0.0 }, -deltaTime);
		}
		else if(penduletime < 2 * M_PI){
			pendule1_->Rotate({ 1.0, 0.0, 0.0 }, deltaTime);
			pendule2_->Rotate({ 1.0, 0.0, 0.0 }, deltaTime);
			pendule3_->Rotate({ 1.0, 0.0, 0.0 }, deltaTime);
		}

		if (penduletime > 2 * M_PI)
			penduletime = 0;
		
		pendule1_->Translate({ 125.0 * RATIO,5.5 * RATIO,0.0 });
		pendule2_->Translate({ 135.0 * RATIO,5.5 * RATIO,0.0 });
		pendule3_->Translate({ 145.0 * RATIO,5.5 * RATIO,0.0 });
		
		plateformemouv1_->HandleUpdate(deltaTime);
		//plateformeintermittente_->HandleUpdate(deltaTime);
		//plateformeintermittente_->CollisionWithPlatform(charac);
		trampoline_->HandleUpdate(deltaTime);
		trampoline_->CollisionWithPlatform(charac);
		// a translate les plateformes mouvantes

		debugHud_->HandleRefresh(deltaTime);
		//quiz_->HandleUpdate(deltaTime, charac);
	}

	///\brief affichage
	void HandleRefresh() {
		for (auto it : plateformes_) {
			it->Draw();
		}

		for (auto it : capsules_) {
			it->Draw();
		}

		//lead->Draw();
		atmosphere_->Draw();
		charac->Draw();
		aimant1_->Draw();
		Ventilateur1_->Draw();
		Ventilateur2_->Draw();
		Ventilateur3_->Draw();
		pendule1_->Draw();
		pendule2_->Draw();
		pendule3_->Draw();
		trampoline_->HandleRefresh();
		labyrinthe_->Draw();
		

		plateformemouv1_->Draw();
		checkpoints_->Draw();
		debugHud_->Draw3D();

		// 2D rendering	
		GlContext* w = dynamic_cast<GlContext*>(Application::GetInstance()->GetWindow());
		if (w != nullptr && w->GetStatus() == _3D) {
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
			glDisable(GL_DEPTH_TEST);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

			w->SetDefaultPerspective(_2D);
		}

		debugHud_->Draw2D();
		//quiz_->Draw();

		if (w != nullptr && w->GetStatus() == _2D) {
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glEnable(GL_DEPTH_TEST);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			w->SetDefaultPerspective(_3D);
		}
		// 2D rendering	

		switch (camType) {
		case Dynamic:
			dcam->ApplyView();
			break;

		case Flying:
			fcam->ApplyView();
			break;
		}
	}
};

#endif
