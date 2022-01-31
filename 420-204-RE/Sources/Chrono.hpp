///\file Chrono.hpp
///\brief Classe chronom�tre

#ifndef CHRONO_HPP
#define CHRONO_HPP

#include <SDL2/SDL.h>
#include <chrono>

class Chrono {
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> lastRefresh;

public:
	Chrono() {
		Reset();
	}

	void Reset() {
		lastRefresh = std::chrono::steady_clock::now();
		//lastRefresh = {};

		
	}

	double GetElapsedSeconds() {
		return std::chrono::duration<double, std::ratio<1,1>>(std::chrono::steady_clock::now() - lastRefresh).count();
	}
};
#endif

