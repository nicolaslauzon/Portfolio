#define DEBUG 1

#include "FileSystem.hpp"
#include "Application.hpp"
#include "SceneSingcaster.hpp"
#include "TestNicolas.hpp"
#include "TestZack.hpp"
#include "SceneChargement.hpp"
#include "SceneControl.hpp"
#include "SceneExemple.hpp"
#include "SceneResultat.hpp"
#include "TestElie.hpp"
#include "TestGabriel.hpp"
#include "TestAziz.hpp"
//#include "QuestionMath.hpp"

int main(int argc, char* argv[]) {
    FileSystem::setPaths(argv);

    /*for (int i = 1; i < 10; ++i) {
        Question question = QuestionMath::GenerateQuestion(i);
        std::cout << get<0>(question) << "\t" << get<1>(question) << endl;
    }
    /* 
       Avant de push mettez votre scene de test en commentaire et activez la scene test.
       Vous pouvez mettre la scene exemple en commentaire avant de travailler sur votre scene.
       
       Utilisez le id correspondant a votre scene : 

	     SCENENICOLAS = 5,
	     SCENEAZIZ = 6,
	     SCENEELIE = 7,
	     SCENEZACK = 8,
	     SCENEGAB = 9,
	     SCENERAFF = 10,
	     SCENEPAT = 11
    */

    //----------------------- Scene de chargement-------------------------
    SceneChargement* sceneChargement = new SceneChargement();
    Application::GetInstance()->AddScene(SCENECHARGEMENT, sceneChargement);
    Application::GetInstance()->SetScene(SCENECHARGEMENT);
    //---------------------------------------------------------------------*/


    /*---------------- Scene de développement Nicolas --------------------
    TestNicolas* sceneNicolas = new TestNicolas();
    Application::GetInstance()->AddScene(SCENENICOLAS, sceneNicolas);
    Application::GetInstance()->SetScene(SCENENICOLAS);
    //--------------------------------------------------------------------*/

    /*---------------- Scene de développement Zack -----------------------
    SceneChargement* sceneZack = new SceneChargement();
    Application::GetInstance()->AddScene(SCENECHARGEMENT, sceneZack);
    Application::GetInstance()->SetScene(SCENECHARGEMENT);
    //--------------------------------------------------------------------*/

    /*------------------ Scene de développement Aziz ---------------------
    TestAziz* sceneAziz = new TestAziz();
    Application::GetInstance()->AddScene(SCENEAZIZ, sceneAziz);
    Application::GetInstance()->SetScene(SCENEAZIZ); //*/
    //-------------------- Scene de test Aziz ----------------------------
   
    /*----------------- Scene de développement Elie ----------------------
    Scene* sceneElie = new TestElie();
    Application::GetInstance()->AddScene(SCENEELIE, sceneElie);
    Application::GetInstance()->SetScene(SCENEELIE);
    //-------------------------------------------------------------------*/

    /*----------------- Scene de développement Gabriel ------------------
    Scene* sceneGab = new TestGabriel();
    Application::GetInstance()->AddScene(SCENEGAB, sceneGab);
    Application::GetInstance()->SetScene(SCENEGAB);
    //-------------------------------------------------------------------*/

    Application::GetInstance()->Start();
    Application::GetInstance()->DeleteThis();

    return 0;
}