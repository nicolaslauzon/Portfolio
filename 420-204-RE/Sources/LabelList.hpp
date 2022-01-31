///\file LabelList.hpp
///\brief Classe permet de faire des paragraphes
#ifndef LABELLIST_HPP
#define LABELLIST_HPP

#include <list>
#include "Label.hpp"
#include "Drawable.hpp"
class LabelList : public Drawable {
private:
	list<Label*> labellist_; ///\ liste d'etiquette
	unsigned int additionalspacing_; ///\ espace entre les lignes
public:

	///\param LabelListeRect Rect de la liste d'etiquette
	///\param additionalspacing espace entre les lignes
	LabelList(const SDL_Rect& LabelListeRect, unsigned int additionalspacing = 0) {
		rect_ = LabelListeRect;
		additionalspacing_ = additionalspacing;
	}

	~LabelList() {
		while (!labellist_.empty()) {
			delete labellist_.front();
			labellist_.pop_front();
		}
	}


	///\brief Ajoute un etiquette a la liste d'etiquette
	///\param label pointeur d'un etiquette a ajouter
	void AddLabel(Label* label) {
		if(labellist_.empty())
			label->SetPosition(label->GetDstRect().x, label->GetDstRect().y + labellist_.size() * label->GetDstRect().h);
		else
			label->SetPosition(label->GetDstRect().x, label->GetDstRect().y + (labellist_.size() * (label->GetDstRect().h + additionalspacing_)) );
		labellist_.push_back(label);	
	}
	///\brief Retire un etiquette a la liste d'etiquette
	///\param label pointeur d'un etiquette a supprimer
	void RemoveLabel(Label* label) {
		labellist_.remove(label);
		delete label;
	}
	
	///\brief Retourne l'etiquette a partir de la position
	///\param position de l'etiquette voulue
	///\return l'etiquette
	Label* GetLabel(unsigned int position) {
		list<Label*>::iterator it = labellist_.begin();
		std::advance(it, position);
		return (*it);
	}

	///\brief Afficher la liste d'etiquette
	void Draw() {
		if(!labellist_.empty()){
			for (auto it : labellist_) {
				it->Draw();
			}
		}
	}

};

#endif
