///\file ResourceManager.hpp
#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP
#include <map>
#include <string>
#include "Resource.hpp"

using namespace std;

///\class ResourceManager
///\brief G�re et contient les ressources
class ResourceManager {
private:
	static map<string, Resource*> resources_; ///< Contient les ressources et leur identifiant
public:
	
	///\brief Ajouter une ressource au gestionnaire
	///\param Identifiant
	///\param Ressource � charger
	static void AddResource(string name, Resource* resource) {
		resources_[name] = resource;
	}

	///\brief Retirer une ressource du gestionnaire
	///\param Identifiant
	static void RemoveResource(string name) {
		resources_.erase(name);
	}

	///\brief Acc�der � une ressource
	///\param Identifiant
	template <typename T>
	static T GetResource(string name) {
		if (resources_.find(name) != resources_.end())
			return (T)(resources_[name]->GetData());
		return NULL;
	}
};
map<string, Resource*> ResourceManager::resources_;

#endif //RESOURCEMANAGER_HPP