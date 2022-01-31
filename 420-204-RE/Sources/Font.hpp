///\file Font.hpp
#ifndef FONT_HPP
#define FONT_HPP
#include "Resource.hpp"

///\class Font
///\brief Modèle d'une ressource de type Font
class Font : public Resource {
private:
	TTF_Font* ttfFont_;
public:
	///\brief Constructeur
	///\param ttfFont Objet de police (ouvert)
	Font(TTF_Font* ttfFont) {
		ttfFont_ = ttfFont;
	}

	~Font() {
		 TTF_CloseFont(ttfFont_);
	}

	///\brief Retourne la ressource
	void* GetData() {
		return ttfFont_;
	}
};
#endif //FONT_HPP