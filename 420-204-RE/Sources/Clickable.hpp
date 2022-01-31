///\file Clickable.hpp
///\brief Événement onClick

#ifndef CLICKABLE_HPP
#define CLICKABLE_HPP

#include <functional>

class Clickable {
protected:
	std::function<void()> onClick; ///< Appelée lors de l'événement click

public:
	Clickable(std::function<void()> onClick = nullptr) {
		this->onClick = onClick;
	}

	inline void setOnClick(std::function<void()> onClick) {
		this->onClick = onClick;
	}
};

#endif
