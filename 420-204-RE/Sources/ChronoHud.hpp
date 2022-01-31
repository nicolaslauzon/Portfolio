///\file ChronoHud.hpp
///\brief Affichage d'un chrono
#ifndef CHRONOHUD_HPP
#define CHRONOHUD_HPP

#include "Chrono.hpp"
#include "Label.hpp"
#include "RectFill.hpp"

///\class ChronoHud.hpp
///\brief Affichage d'un chrono
class ChronoHud : public Label{
protected:

	Chrono chrono_; ///< Chrono
	RectFill* background_;
	string temp_;
	double finalTime_;
public:
	ChronoHud(SDL_Rect rectchrono, TTF_Font* font, SDL_Color color, const char* content) : Label({rectchrono.x+5,rectchrono.y,rectchrono.w,rectchrono.h}, font, color, content) {
		background_ = new RectFill(rectchrono, {235,124,200,255});
		rectchrono = rect_;
		chrono_.Reset();
		finalTime_ = 0.0;
	}
	
	///\brief Retourne la valeur du chrono
	double GetValeurChrono() {
		return (chrono_.GetElapsedSeconds());
	}

	void endRun() {
		finalTime_ = chrono_.GetElapsedSeconds();
	}

	double getFinalTime() {
			return finalTime_;
	}

	///\brief Affiche le chrono
	void Draw() {
		background_->Draw();
		temp_ = to_string(chrono_.GetElapsedSeconds());
		
		SetText((temp_.substr(0, temp_.find(".") + 3)).c_str());

		glBindTexture(GL_TEXTURE_2D, labelId_);
		glBegin(GL_QUADS);
			glTexCoord2i(0.0, 0.0); glVertex3d(rect_.x, rect_.y, 0.0);
			glTexCoord2i(1.0, 0.0); glVertex3d(rect_.x + rect_.w, rect_.y, 0.0);
			glTexCoord2i(1.0, 1.0); glVertex3d(rect_.x + rect_.w, rect_.y + rect_.h, 0.0);
			glTexCoord2i(0.0, 1.0); glVertex3d(rect_.x, rect_.y + rect_.h, 0.0);
		glEnd();
	}

};

#endif // CHRONOHUD_HPP
