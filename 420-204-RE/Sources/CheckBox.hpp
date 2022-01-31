///\file CheckBox.hpp
///\brief Cases à cocher

#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP

#include "Vector.hpp"
#include "Drawable.hpp"
#include "Observer.hpp"
#include "RectFill.hpp"

///\class CheckBox
///\brief Case à cocher
class CheckBox {
protected:
  bool checked;

public:
  CheckBox() {
    checked = false;
  }
};

///\class CheckBox
///\brief Case à cocher colorée 2D
class ColorCheckBox : public CheckBox, public Drawable, public Observer<SDL_Event> {
private:
  Vector2i position; ///< Position
  Vector2i size; ///< Taille
  RectFill* background; ///< Arrière-plan
  RectFill* check; ///< Crochet

public:
  ///\param x Position sur l'axe des x
  ///\param x Position sur l'axe des y
  ///\param width Largeur
  ///\param height Hauteur
  ColorCheckBox(const int& x, const int& y, const int& width, const int& height, const SDL_Color& backgroundColor, const SDL_Color& checkColor) : position(x, y), size(width, height) {
    rect_ = { x, y, width, height };
    background = new RectFill({ position.x, position.y, size.x, size.y }, backgroundColor, 1);
    check = new RectFill({ position.x + 2, position.y + 2, size.x - 4, size.y - 4 }, checkColor, 2);
  }

  bool isChecked() {
    return checked;
  }

  ///\brief Événement
  void Notification(SDL_Event data) {
    SDL_Point sdlPoint = { data.button.x, data.button.y };
    if (SDL_PointInRect(&sdlPoint, &rect_))
      checked = !checked;
  }

  ///\brief Affichage
  void Draw() {
    background->Draw();
    if (checked) check->Draw();
  }
};

#endif