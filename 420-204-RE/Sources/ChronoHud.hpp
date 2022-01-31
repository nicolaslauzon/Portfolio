///\file ChronoHud.hpp
///\brief Affichage d'un chrono
#ifndef CHRONOHUD_HPP
#define CHRONOHUD_HPP

#include "Chrono.hpp"
#include "Label.hpp"
///\class ChronoHud.hpp
///\brief Affichage d'un chrono
class ChronoHud : public Label{
protected:

	Chrono chrono_; ///< Chrono

public:
	ChronoHud(SDL_Rect rectchrono, TTF_Font* font, SDL_Color color, const char* content) : Label(rectchrono, font, color, content) {
		rectchrono = rect_;
		chrono_.Reset();
		chrono_.Start();
		SetText((to_string(chrono_.GetElapsedSeconds()).c_str()));
	}
	///\brief Arrete le chrono
	void Stop() {
		chrono_.Stop();
	}
	///\brief Part le chrono
	void Start() {
		chrono_.Start();
	}
	///\brief Retourne la valeur du chrono
	double GetValeurChrono() {
		return (chrono_.GetElapsedSeconds());
	}

	///\brief Affiche le chrono
	void Draw() {
		SetText((to_string(chrono_.GetElapsedSeconds()).c_str()));
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
