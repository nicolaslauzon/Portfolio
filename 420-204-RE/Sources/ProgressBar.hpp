///\file ProgressBar.hpp
///\brief Barre de progres pouvant etre incr�menter, de dimentions variables

#ifndef PROGRESSBAR_HPP
#define PROGRESSBAR_HPP

#include "Drawable.hpp"
#include "RectFill.hpp"

///\class ProgressBar
///\brief Barre de progres pouvant etre incr�menter, de dimentions variables
class ProgressBar : public Drawable {
private:
	double max_, current_;				///< Valeur maximale que peut atteindre la barre de progres /< Valeur actuelle de la barre de chargement
	int border_;						///< Largeur de la bordure autour du rectangle qui va progresser
	RectFill* currentUpRect_, * currentDownRect_; ///< Rectangle qui va progresser
	RectFill* backgroundRect_;			///< Rectangle d'arriere plan
	SDL_Color backColor_, frontColor_;	///< Couleur de l'arriere plan /< Couleur de l'avant plan

public:
	///\param rect Taille et position de la barre
	///\param start Valeur de d�part de la barre
	///\param max Valeur maximale de la barre
	///\param backColor Couleur de l'arriere plan
	///\param frontColor Couleur de l'avant plan
	///\param priority Priorit� d'affichage
	ProgressBar(SDL_Rect rect, double start, double max, SDL_Color backColor, SDL_Color frontColor, unsigned int priority = 0 ):Drawable(priority, rect) {
		backColor_ = backColor;
		frontColor_ = frontColor;
		SDL_Color tmpColor = { frontColor.r - 10, frontColor.g - 10, frontColor.b - 10, frontColor.a };
		currentUpRect_ = new RectFill({ 0, 0, 0, 0 }, frontColor_);
		currentDownRect_ = new RectFill({ 0, 0, 0, 0 }, tmpColor);
		backgroundRect_ = new RectFill({ rect.x, rect.y, rect.w, rect.h }, backColor_);		
		current_ = start;
		max_ = max;
	}

	///\brief Modifier la progression de la barre 	
	void updateProgress(double changed) {
		if ((current_ + changed <= max_) and (current_ + changed >= 0.0))
			current_ += changed;
	}

	///\brief Affichage 	
	void Draw() {
		backgroundRect_->Draw();		
		currentUpRect_->SetRect({ rect_.x + 2, rect_.y + 2, (int)(current_ * backgroundRect_->GetDstRect().w / max_) - 4, rect_.h / 2 });
		currentDownRect_->SetRect({ rect_.x + 2, rect_.y + rect_.h / 2, (int)(current_ * backgroundRect_->GetDstRect().w / max_) - 4, rect_.h / 2 - 2 });
		currentUpRect_->Draw();
		currentDownRect_->Draw();
	}
};
#endif	//PROGRESSBAR_HPP