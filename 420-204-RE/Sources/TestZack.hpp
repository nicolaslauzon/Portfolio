#ifndef TESTZACK_HPP
#define TESTZACK_HPP

#include "Scene.hpp"
#include "Image.hpp"

class TestZack : public Scene {
private:
	Image* image_;
public:
	TestZack() {

	}


	void Init() {
		SDL_Surface* sdlSurface = IMG_Load("lightBackground.png");//texture ////Texture Provisoire
		unsigned int id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sdlSurface->w, sdlSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sdlSurface->pixels);
		SDL_FreeSurface(sdlSurface);

		image_ = new Image(id, { 100,100,100,100 }, false);
	}

	void HandleEvent(SDL_Event sdlEvent) {
		
	}
	void HandleUpdate(double deltaTime) {

	}
	void HandleRefresh() {
		image_->Draw();
	}
};


#endif