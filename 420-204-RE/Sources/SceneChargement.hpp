///\file SceneChargement.hpp
#ifndef SCENECHARGEMENT_HPP
#define SCENECHARGEMENT_HPP

#include <fstream>

#include "Application.hpp"
#include "Scene.hpp"
#include "FileSystem.hpp"
#include "ResourceManager.hpp"
#include "Font.hpp"
#include "Texture.hpp"
#include "RectFill.hpp"
#include "ProgressBar.hpp"
#include "Mesh.hpp"
#include "SceneMenuPrincipal.hpp"
//#include "Sprite3D.hpp"

///\class SceneChargement
///\brief Scène qui gère le chargement des ressources
class SceneChargement : public Scene {
private:
	string fileName_;				///< Nom du fichier
	string name_;						///< Nom de la ressource
	ProgressBar* progress_;	///< Barre de progression
	ifstream reader_;				///< Lecteur de fichier texte
	int size_;							///< Taille de police de caractères | Nombre de cadres
	SDL_Surface* surface_;	///< Surface SDL
	char type_;							///< Type de ressource

public:	
	void Init() {
		SDL_Point windowSize = Application::GetInstance()->GetWindow()->GetSize();
		AddDrawable(new RectFill({ 0, 0, windowSize.x, windowSize.y }, { 255, 255, 115, 255 }));

		progress_ = nullptr;
		reader_.open(FileSystem::resourcePath + "ResourceList.txt", ifstream::in);
	}

	~SceneChargement() {
		ClearDrawableList();

		reader_.close();
	}

	///\brief Gestion des événements dans la scène
	void HandleEvent(SDL_Event sdlEvent) {}
	
	///\brief Chargement des fichiers
  ///\param deltaTime Temps entre deux utilisation de la méthode
	void HandleUpdate(double deltaTime) {
		reader_ >> type_;
		if (!reader_.eof()) {
			reader_ >> name_;
			reader_ >> fileName_;
			switch (type_) {
				case 'C': { // Compte
					int resourceCount;
					reader_ >> resourceCount;
					SDL_Point windowSize = Application::GetInstance()->GetWindow()->GetSize();
					progress_ = new ProgressBar({ 50, windowSize.y - 100, windowSize.x - 100, 50 }, 0, resourceCount, { 50, 50, 50, 255 }, { 150, 150, 150, 255 });
					AddDrawable(progress_);
				} break;

				case 'T':	// Texture
						ResourceManager::AddResource(name_, new Texture(Application::GetInstance()->loadTexture((FileSystem::imagesPath + fileName_).c_str())));
						break;

				case 'F':	// Police de caractères
					reader_ >> size_;
					ResourceManager::AddResource(name_, new Font(TTF_OpenFont((FileSystem::fontsPath + fileName_).c_str(), size_)));
					break;

				case 'M': { // Modèle
					TexturedMesh* mesh = new TexturedMesh();
					mesh->load((FileSystem::modelsPath + fileName_).c_str());
					ResourceManager::AddResource(name_, mesh);
				} break;

				case 'S': {	// Animation
					reader_ >> size_;
					Sprite3D* sprite = new Sprite3D(size_);
					for (size_t i = 0; i < size_; i++) {
						string zeroes = "000";
						if (i < 9)
							zeroes = ("00000");
						else if (i < 99)
							zeroes = "0000";

						TexturedMesh* mesh = new TexturedMesh();
						mesh->load((FileSystem::spritePath + fileName_ + zeroes + to_string(i + 1) + ".obj").c_str());
						sprite->AddFrame(mesh);
					}
					ResourceManager::AddResource(name_, sprite);
				} break;

				default:	// Commentaire
					reader_.ignore(256, 10);
					break;
			}

			if (progress_)
				progress_->updateProgress(1);
		}
		else {				
			Application::GetInstance()->AddScene(SCENEMENU, new SceneMenuPrincipal());
			Application::GetInstance()->SetScene(SCENEMENU);
			Application::GetInstance()->RemoveScene(SCENECHARGEMENT);

		}
	}

	///\brief Affichage de la scène
	void HandleRefresh() {
		RenderAll();
	}
};

#endif
