///\file Hoverable.hpp
///\brief Événement onHover

#ifndef HOVERABLE_HPP
#define HOVERABLE_HPP

#include <functional>

class Hoverable {
protected:
	bool hover_;      ///< Si le bouton est survolé
	std::function<void()> onHover; ///< Appelée lors de l'événement hover

public:
	Hoverable(std::function<void()> onHover = nullptr, bool hover = false) {
		this->onHover = onHover;
		hover_ = hover;
	}

	inline void setOnHover(std::function<void()> onHover) {
		this->onHover = onHover;
	}
};

#endif
