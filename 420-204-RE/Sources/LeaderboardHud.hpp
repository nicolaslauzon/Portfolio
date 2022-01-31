///\file LeaderboardHud.hpp
///\brief Classe qui permet manipuler une liste a partir d'un fichier texte

#ifndef LEADERBOARDHUD_HPP
#define LEADERBOARDHUD_HPP

#include "Leaderboard.hpp"
#include "Label.hpp"
class LeaderboardHud : public Leaderboard, public Drawable{
private:
	LabelList* namelist_; ///\ liste des noms des joueurs
	LabelList* timelist_; ///\ liste de temps des joueurs

public: 
	

	///\param filename nom du fichier texte
	///\param font Police
	///\param color Couleur du texte
	///\param LabelListeRect Rect de la premiere labelList
	///\param additionalspacing espace entre les lignes
	///\param spacinglist espace entre les listes
	
	LeaderboardHud(string filename, TTF_Font* font, SDL_Color color,SDL_Rect LabelListeRect, int additionalspacing = 0, int spacinglist = 0) : Leaderboard(filename){
		
		Rank();
		namelist_ = new LabelList(LabelListeRect, additionalspacing);
		timelist_ = new LabelList({ LabelListeRect.x + spacinglist, LabelListeRect.y, LabelListeRect.w, LabelListeRect.h }, additionalspacing);
		for (auto it : listplayers_) {
			namelist_->AddLabel(new Label(namelist_->GetDstRect(), font, color, it.first.c_str()));
			timelist_->AddLabel(new Label(timelist_->GetDstRect(), font, color, to_string(it.second).c_str()));
		}
	}

	~LeaderboardHud() {
		Reset();
		delete namelist_;
		delete timelist_;
		
	}

	///\brief Afficher le leaderboard
	void Draw() {
		namelist_->Draw();
		timelist_->Draw();
	}
};

#endif //LEADERBOARDHUD_HPP