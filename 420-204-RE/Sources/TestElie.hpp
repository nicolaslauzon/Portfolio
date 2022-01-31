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

class TestElie : public Scene {
private:
	//TTF_Font* font;
	Character* charac;

	Objet3dDrawable* atmosphere_;

	PlateformeMouvantes* plateformemouv1_;
	Objet3dDrawable* plateforme1_;
	Objet3dDrawable* couloirporte1_;
	Objet3dDrawable* couloirporte2_;
	Objet3dDrawable* couloirporte3_;
	Objet3dDrawable* couloirporte4_;
	Objet3dDrawable* couloirporte5_;
	Objet3dDrawable* couloirporte6_;

	Objet3dDrawable* plateformeCheckpoint1_;
	Objet3dDrawable* checkpointPillier1_;
	Objet3dDrawable* checkpointFlag1_;

	Objet3dDrawable* couloirVentilateur1_;
	Objet3dDrawable* couloirVentilateur2_;
	Objet3dDrawable* couloirVentilateur3_;
	Objet3dDrawable* Ventilateur1_;
	Objet3dDrawable* Ventilateur2_;
	Objet3dDrawable* Ventilateur3_;

	Objet3dDrawable* plateformejump1_;
	Objet3dDrawable* plateformejump2_;
	Objet3dDrawable* plateformejump3_;

	Objet3dDrawable* couloirParkour1_;
	Objet3dDrawable* plateformeParkour1_;
	Objet3dDrawable* couloirParkour2_;
	Objet3dDrawable* couloirParkour3_;

	Objet3dDrawable* couloirCheckpoint2_;
	Objet3dDrawable* checkpointPillier2_;
	Objet3dDrawable* checkpointFlag2_;

	Objet3dDrawable* couloirpendule1_;
	Objet3dDrawable* couloirpendule2_;
	Objet3dDrawable* couloirpendule3_;

	Objet3dDrawable* pendule1_;
	Objet3dDrawable* pendule2_;
	Objet3dDrawable* pendule3_;

	Objet3dDrawable* plateformeMouvante1_;
	Objet3dDrawable* plateformeMouvante2_;
	Objet3dDrawable* plateformeMouvante3_;

	Objet3dDrawable* couloirCheckpoint3_;
	Objet3dDrawable* checkpointPillier3_;
	Objet3dDrawable* checkpointFlag3_;

	Objet3dDrawable* pisteGlissante_;

	Objet3dDrawable* labyrinthe_;

	Sprite3Ddrawable* bug;

	//StaticCamera* cam;
	FlyingCamera* fcam;
	DynamicCamera* dcam;
	unsigned int meshtexid;
	double penduletime;
	double animtime_;
	// DebugHud* debugHud_;

	//DebugHud* debugHud_;

public:
	TestElie() {

	}
	
	~TestElie() {
		//delete debugHud_;
	}

	///\brief appel� au d�but de la sc�ne
	///\param windowId identificateur de la fenetre
	void Init() {
		//font = TTF_OpenFont("MrRobot.ttf", 42);
		penduletime = 0;
		((GlContext*)Application::GetInstance()->GetWindow())->SetDefaultPerspective(_3D);
		
		float lightPos[3] = { 1000.0, 1000.0, 0.0 };
		// glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		//debugHud_ = new DebugHud();


		//cam = new StaticCamera({ 0.0,1.0,0.0 }, { 2.0,20.0,0.0 }, { 2.0, 1.0, 0.0 });

		
		//fcam = new FlyingCamera({ 0.0, 1.0, 0.0 }, { 0.0,10.0,0.0 }, { 10.0, 0.0, 0.0 });
		//cam = new StaticCamera({ 0.0, 1.0, 0.0 }, { 5.0, 30.0, 0.0 }, { 0.0, 0.0, 0.0 });
		//fcam = new FlyingCamera({ 0.0, 1.0, 0.0 }, { 150 * RATIO,10.0,0.0 }, { 0.0, 0.0, 0.0 });
		//cam = new StaticCamera({ 0.0, 1.0, 0.0 }, { 5.0, 30.0, 0.0 }, { 0.0, 0.0, 0.0 });
		// cam = new StaticCamera({ 0.0,1.0,0.0 }, { 10.0,0.0,0.0 }, { 2.0, 0.0, 0.0 });
		
		//bug = new Sprite3Ddrawable(ResourceManager::GetResource<Sprite3D*>("bugSprite"), *ResourceManager::GetResource<unsigned int*>("shell"));
		fcam = new FlyingCamera({ 0.0, 1.0, 0.0 }, { 10.0, 10.0, 10.0 }, { 0.0, 0.0, 0.0 });
		
		atmosphere_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("atmosphereM"), *ResourceManager::GetResource<unsigned int*>("atmosphere"));

		charac = new Character(ResourceManager::GetResource<TexturedMesh*>("bugModel"), *ResourceManager::GetResource<unsigned int*>("shell"));
		charac->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		charac->Translate({ 0.0,1.0 * RATIO,0.0 });
		charac->SetPosition(Vector3d(0.0, 7.0 * RATIO, 0.0) + charac->GetPosition().GetOrigin());

		//debugHud_->AddDebug(charac, charac->GetSpeed());

		dcam = new DynamicCamera(charac);

		plateforme1_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("surfacePlane"));
		
		plateformemouv1_ = new PlateformeMouvantes(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("surfacePlane"), { 0.0,0.0,1.0 }, 20 * RATIO,10.0);
		plateformemouv1_->Translate({ -4.0 * RATIO,0.0,-10.0* RATIO });

		couloirporte1_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirporte1_->Translate({ 4.0 * RATIO,0.0,0.0 });

		couloirporte2_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirporte2_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirporte2_->Translate({ 11.0 * RATIO,0.0,0.0 });
		
		couloirporte3_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirporte3_->Translate({ 18.0 * RATIO,0.0,0.0 });

		couloirporte4_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirporte4_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirporte4_->Translate({ 25.0 * RATIO,0.0,0.0 });

		couloirporte5_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirporte5_->Translate({ 32.0 * RATIO,0.0,0.0 });

		couloirporte6_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirporte6_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirporte6_->Translate({ 39 * RATIO,0.0,0.0 });


		plateformeCheckpoint1_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		plateformeCheckpoint1_->Translate({ 46.0 * RATIO,0.0,0.0 });

		checkpointPillier1_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("checkpointPillierM"), *ResourceManager::GetResource<unsigned int*>("checkpointPillier"));
		checkpointPillier1_->Translate({ 46.5 * RATIO,0.5 * RATIO,0.0 });

		checkpointFlag1_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("checkpointFlagM"), *ResourceManager::GetResource<unsigned int*>("checkpointBaseFlag"));
		checkpointFlag1_->Translate({ 46.5 * RATIO,0.5 * RATIO,0.0 });
	

		couloirVentilateur1_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirVentilateur1_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirVentilateur1_->Translate({ 53.0 * RATIO,0.0,0.0 });

		couloirVentilateur2_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirVentilateur2_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirVentilateur2_->Translate({ 63.0 * RATIO,0.0,0.0 });
		
		couloirVentilateur3_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirVentilateur3_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirVentilateur3_->Translate({ 73.0 * RATIO,0.0,0.0 });
		

		Ventilateur1_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("ventilateurM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		Ventilateur1_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		Ventilateur1_->Translate({ 55.0 * RATIO,2.5 * RATIO,5.0 * RATIO });

		Ventilateur2_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("ventilateurM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		Ventilateur2_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		Ventilateur2_->Translate({ 63.0 * RATIO,2.5 * RATIO,5.0 * RATIO });

		Ventilateur3_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("ventilateurM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		Ventilateur3_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);		
		Ventilateur3_->Translate({ 71.0 * RATIO,2.5 * RATIO,5.0 * RATIO });


		plateformejump1_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("surfacePlaneM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		plateformejump1_->Translate({ 82.0 * RATIO,0.0,-3.0 * RATIO });

		plateformejump2_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		plateformejump2_->Translate({ 88.0 * RATIO,0.0,2.0 * RATIO });

		plateformejump3_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		plateformejump3_->Translate({ 95.0 * RATIO,0.0,0.0 * RATIO });


		couloirParkour1_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirParkour1_->Rotate({ 0.0,0.0,1.0 }, DEMIPI);
		couloirParkour1_->Translate({ 97.5 * RATIO,-1.5 * RATIO,4.0 * RATIO });

		plateformeParkour1_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		plateformeParkour1_->Rotate({ 0.0,0.0,1.0 }, DEMIPI);
		plateformeParkour1_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		plateformeParkour1_->Translate({ 100.0 * RATIO,-1.5 * RATIO,8.5 * RATIO });

		couloirParkour2_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirParkour2_->Rotate({ 0.0,0.0,1.0 }, DEMIPI);
		couloirParkour2_->Translate({ 102.5 * RATIO,-1.5 * RATIO,4.0 * RATIO });

		couloirParkour3_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirParkour3_->Rotate({ 0.0,0.0,1.0 }, DEMIPI);
		couloirParkour3_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirParkour3_->Translate({ 108.0 * RATIO,-1.5 * RATIO,0.0});


		couloirCheckpoint2_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirCheckpoint2_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirCheckpoint2_->Translate({ 118.0 * RATIO,0.0,0.0 });

		checkpointPillier2_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("checkpointPillierM"), *ResourceManager::GetResource<unsigned int*>("checkpointPillier"));
		checkpointPillier2_->Translate({ 118.0 * RATIO,0.5 * RATIO,0.0 });

		checkpointFlag2_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("checkpointFlagM"), *ResourceManager::GetResource<unsigned int*>("checkpointBaseFlag"));
		checkpointFlag2_->Translate({ 118.0 * RATIO,0.5 * RATIO,0.0 });


		couloirpendule1_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirpendule1_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirpendule1_->Translate({ 128.0 * RATIO,0.0,0.0 });

		couloirpendule2_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirpendule2_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirpendule2_->Translate({ 138.0 * RATIO,0.0,0.0 });

		couloirpendule3_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirpendule3_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirpendule3_->Translate({ 148.0 * RATIO,0.0,0.0 });

		pendule1_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("penduleM"), *ResourceManager::GetResource<unsigned int*>("pendule"));
		pendule1_->Rotate({ 0.0,0.0,1.0 }, -DEMIPI);
		pendule1_->Translate({ 125.0 * RATIO,5.5 * RATIO,4.5 * RATIO });

		pendule2_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("penduleM"), *ResourceManager::GetResource<unsigned int*>("pendule"));
		pendule2_->Rotate({ 0.0,0.0,1.0 }, -DEMIPI);;
		pendule2_->Translate({ 135.0 * RATIO,5.5 * RATIO,4.5 * RATIO });

		pendule3_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("penduleM"), *ResourceManager::GetResource<unsigned int*>("pendule"));
		pendule3_->Rotate({ 0.0,0.0,1.0 }, -DEMIPI);
		pendule3_->Translate({ 145.0 * RATIO,5.5 * RATIO,4.5 * RATIO });

		plateformeMouvante1_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		plateformeMouvante1_->Translate({ 156.0 * RATIO,0.0,0.0 });

		plateformeMouvante2_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		plateformeMouvante2_->Translate({ 161.0 * RATIO,0.0,0.0 });

		plateformeMouvante3_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		plateformeMouvante3_->Translate({ 166.0 * RATIO,0.0,0.0 });


		couloirCheckpoint3_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("couloirM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		couloirCheckpoint3_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		couloirCheckpoint3_->Translate({ 174.0 * RATIO, 0.0, 0.0 });

		checkpointPillier3_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("checkpointPillierM"), *ResourceManager::GetResource<unsigned int*>("checkpointPillier"));
		checkpointPillier3_->Translate({ 172.0 * RATIO, 0.5 * RATIO, 0.0 });

		checkpointFlag3_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("checkpointFlagM"), *ResourceManager::GetResource<unsigned int*>("checkpointBaseFlag"));
		checkpointFlag3_->Translate({ 172.0 * RATIO, 0.5 * RATIO, 0.0 });

		labyrinthe_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("labyrintheM"), *ResourceManager::GetResource<unsigned int*>("labyrinthe"));
		//labyrinthe_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		labyrinthe_->Translate({ 178 * RATIO, 0.0 * RATIO, 4.0 * RATIO });

		pisteGlissante_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("pisteM"), *ResourceManager::GetResource<unsigned int*>("pisteGlissante"));
		pisteGlissante_->Rotate({ 1.0,0.0,0.0 }, DEMIPI);
		pisteGlissante_->Rotate({ 0.0,1.0,0.0 }, DEMIPI);
		pisteGlissante_->Translate({ 181 * RATIO, 0.0 * RATIO, 0.0 });
		
		plateformeMouvante2_ = new Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("plateformeM"), *ResourceManager::GetResource<unsigned int*>("couloir"));
		plateformeMouvante2_->Translate({ 161.0 * RATIO,0.0,0.0 });

		

	}

	///\brief �venements
	///\param sdlEvent �venement � g�rer
	void HandleEvent(SDL_Event sdlEvent) {
		//fcam->Notification(sdlEvent);
		//charac->Notification(sdlEvent);
		// debugHud_->Notification(sdlEvent);
		fcam->Notification(sdlEvent);
		//charac->Notification(sdlEvent);
		//debugHud_->Notification(sdlEvent);
	}

	///\brief G�rer les modifications
	///\param deltaTime temps �coul� depuis le dernier appel de cette fonction
	void HandleUpdate(double deltaTime) {
		//dcam->Move(deltaTime);
		
		fcam->Move(deltaTime);
		//charac->HandleUpdate(deltaTime);

		/*
		animtime_ += deltaTime;
		if (animtime_ > 0.01) {
			bug->SetNextFrame();
			animtime_ = 0;
		}
		*/
		

		Ventilateur1_->Translate({ -55.0 * RATIO,-2.5 * RATIO,-5.0 * RATIO });
		Ventilateur2_->Translate({ -63.0 * RATIO,-2.5 * RATIO,-5.0 * RATIO });
		Ventilateur3_->Translate({ -71.0 * RATIO,-2.5 * RATIO,-5.0 * RATIO });

		Ventilateur1_->Rotate({ 0.0, 0.0, 1.0 }, deltaTime*15);
		Ventilateur2_->Rotate({ 0.0, 0.0, 1.0 }, deltaTime*15);
		Ventilateur3_->Rotate({ 0.0, 0.0, 1.0 }, deltaTime*15);

		Ventilateur1_->Translate({ 55.0 * RATIO,2.5 * RATIO,5.0 * RATIO });
		Ventilateur2_->Translate({ 63.0 * RATIO,2.5 * RATIO,5.0 * RATIO });
		Ventilateur3_->Translate({ 71.0 * RATIO,2.5 * RATIO,5.0 * RATIO });

		//a mieux rotate
		
		
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
		// a translate les plateformes mouvantes

		//debugHud_->HandleRefresh(deltaTime);
	}

	///\brief affichage
	void HandleRefresh() {
		//lead->Draw();
		atmosphere_->Draw();
		//bug->Draw();
		charac->Draw();
		plateforme1_->Draw();
		couloirporte1_->Draw();
		couloirporte2_->Draw();
		couloirporte3_->Draw();
		couloirporte4_->Draw();
		couloirporte5_->Draw();
		couloirporte6_->Draw();
		plateformeCheckpoint1_->Draw();
		checkpointPillier1_->Draw();
		checkpointFlag1_->Draw();
		couloirVentilateur1_->Draw();
		couloirVentilateur2_->Draw();
		couloirVentilateur3_->Draw();
		Ventilateur1_->Draw();
		Ventilateur2_->Draw();
		Ventilateur3_->Draw();
		plateformejump1_->Draw();
		plateformejump2_->Draw();
		plateformejump3_->Draw();
		couloirParkour1_->Draw();
		plateformeParkour1_->Draw();
		couloirParkour2_->Draw();
		couloirParkour3_->Draw();
		couloirCheckpoint2_->Draw();
		checkpointPillier2_->Draw();
		checkpointFlag2_->Draw();
		couloirpendule1_->Draw();
		couloirpendule2_->Draw();
		couloirpendule3_->Draw();
		pendule1_->Draw();
		pendule2_->Draw();
		pendule3_->Draw();
		plateformeMouvante1_->Draw();
		plateformeMouvante2_->Draw();
		plateformeMouvante3_->Draw();
		couloirCheckpoint3_->Draw();
		checkpointPillier3_->Draw();
		checkpointFlag3_->Draw();

		plateformemouv1_->Draw();
		//labyrinthe_->Draw();
		pisteGlissante_->Draw();

		//debugHud_->Draw();
		//labyrinthe_->Draw();
		//dcam->ApplyView();
		//cam->ApplyView();
		fcam->ApplyView();

		//dcam->ApplyView();
		//cam->ApplyView();
		//fcam->ApplyView();
		//mesh->Draw();
	}
};


#endif

//lead = new LeaderboardHud("Leaderboard.txt", font, { 255,0,0,255 }, { 300,200,0,0 },0,200);
/*labeliste = new LabelList({ 500,0,0,0 }, 10);
labeliste->AddLabel(new Label({ 500, 200,0,0 }, ttfFont, { 255,255,255,255 }, "Es hora de comer"));
labeliste->AddLabel(new Label({ 500, 200,0,0 }, ttfFont, { 255,255,255,255 }, "Es hora de beber"));
labeliste->AddLabel(new Label({ 500, 200,0,0 }, ttfFont, { 255,255,255,255 }, "Es hora de robar"));
labeliste->AddLabel(new Label({ 500, 200,0,0 }, ttfFont, { 255,255,255,255 }, "Es hora de dormir"));
labeliste->AddLabel(new Label({ 500, 200,0,0 }, ttfFont, { 255,255,255,255 }, "Es hora de hola"));*/
//label = new Label({ 500, 200,0,0 }, font, { 0,0,255,255 }, "Es hora de comer");


		/*glGenTextures(1, &meshtexid);
		glBindTexture(GL_TEXTURE_2D, meshtexid);
		SDL_Surface* texture = IMG_Load("cubesimpletex.png");
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->w, texture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
		SDL_FreeSurface(texture);*/

		//mesh = new Objet3dDrawable("cubesimple.obj", meshtexid);
