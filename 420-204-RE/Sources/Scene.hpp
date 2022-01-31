///\file Scene.hpp
///\brief Classe servant pour la g�n�ralisation des autres sc�nes
#ifndef SCENE_HPP
#define SCENE_HPP
#include <map>
#include <list>
#include "Drawable.hpp"
#include "Observable.hpp"



using namespace std;

///\class Scene
///\brief scene affichable dans un contexte openGl
class Scene {

protected:
	std::list<Drawable*> drawables;	// pt declarer dans chacune des scene vu que c'est situationnel
	unsigned int windowId;			///< Identifiant de la window
	std::map<unsigned int, Observable<SDL_Event>*> eventDispatcher_;	///< Map qui gere les evenements
	TTF_Font* font_;				///< Police de charactere
	bool listeNonRangee = false;	///< Booleen qui dit si la liste de drawable est rangée



public:

	Scene() {}
	virtual ~Scene() {}

	///\brief Change l'identifiant de la window
	///\param window_id Nouvel identifiant de la window
	inline void SetWindowId(const unsigned int& window_id) {
		this->windowId = window_id;
	}

	///\brief Retourne l'identifiant de la window
	inline const unsigned int& GetId() const {
		return windowId;
	}

	///\brief Ajoute un drawable a la liste de drawable
	///\param new_drawable Drawable a ajouter
	inline void AddDrawable(Drawable* new_drawable) {
		listeNonRangee = true;
		drawables.push_back(new_drawable);
	}

	///\brief Retire un drawable a la liste de drawable
	///\param new_drawable Drawable a retirer
	void RemoveDrawable(const Drawable* drawable) { // priorité doit être supérieur à 1
		std::list<Drawable*>::iterator it = drawables.begin();
		for (int i = 0; i < drawables.size(); ++i) {
			if (*it == drawable) {
				i = drawables.size();
			}
			else {
				++it;
			}
		}
		drawables.erase(it);
		//delete *it;
	}

	///\brief Vide la liste de drawable
	void ClearDrawableList() {
		for (std::list<Drawable*>::iterator it = drawables.begin(); it != drawables.end();) {
			delete* it;
			++it;
			drawables.pop_front();
		}
	}

	///\brief Vide la map de l'event dispatcher
	void ClearEventDispatcherMap() {
		for (map<unsigned int, Observable<SDL_Event>*>::iterator it = eventDispatcher_.begin(); it != eventDispatcher_.end(); ++it) {
			delete it->second;
		}
		eventDispatcher_.clear();
	}

	///\brief Tri de la liste de drawable
	void SortDrawables() {
		drawables.sort(Drawable::Compare);
	}

	///\brief Dessine le drawable passé en parametre
	///\param drawable Drawable a dessiner
	inline void Render(Drawable* drawable) {
		drawable->Draw();
	}

	///\brief Dessine tous les drawables
	void RenderAll() {
		if (listeNonRangee) {
			SortDrawables();
			listeNonRangee = false;
		}

		std::list<Drawable*>::const_iterator it = drawables.begin();
		while (it != drawables.end()) {
			Render(*it);
			++it;
		}
	}

	///\brief Ajoute un evenement a gerer
	///\param event_type evenement a ajouter
	///\param observer TODO: A COMMENTER
	void SubscribeEvent(unsigned int event_type, Observer<SDL_Event>* observer) {
		if (eventDispatcher_.find(event_type) == eventDispatcher_.end()) {
			eventDispatcher_[event_type] = new Observable<SDL_Event>();
		}
		eventDispatcher_[event_type]->Subscribe(observer);
	}

	///\brief Retire un evenement a gerer 
	///\param event_type evenement a retirer
	///\param observer TODO: A COMMENTER
	inline void UnsubscribeEvent(const unsigned int& event_type, Observer<SDL_Event>* observer) {
		eventDispatcher_[event_type]->Unsubscribe(observer);
	}

	virtual void Init() = 0;
	virtual void HandleEvent(SDL_Event sdlEvent) = 0;
	virtual void HandleUpdate(double deltaTime) = 0;
	virtual void HandleRefresh() = 0;
};
#endif //SCENE_HPP