// #define DEBUG 1

#include "FileSystem.hpp"
#include "Application.hpp"
#include "SceneChargement.hpp"
#include "SceneControl.hpp"
#include "SceneExemple.hpp"
#include "SceneResultat.hpp"

int main(int argc, char* argv[]) {
    FileSystem::setPaths(argv);

    //----------------------- Scene de chargement-------------------------
    SceneChargement* sceneChargement = new SceneChargement();
    Application::GetInstance()->AddScene(SCENECHARGEMENT, sceneChargement);
    Application::GetInstance()->SetScene(SCENECHARGEMENT);
    //---------------------------------------------------------------------*/

    Application::GetInstance()->Start();
    Application::GetInstance()->DeleteThis();

    return 0;
}