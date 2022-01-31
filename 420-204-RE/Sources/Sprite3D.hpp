#ifndef SPRITE3D_HPP
#define SPRITE3D_HPP

#include <list>
#include <string>
#include "Mesh.hpp"
#include "Resource.hpp"
#include "Objet3dDrawable.hpp"
class Sprite3D : public Resource{
protected:

	TexturedMesh** modelListe_;
	unsigned int currentFrame_;
	unsigned textureId_;
	unsigned int currentNumberOfFrames_;
	unsigned int numberOfFrames_;
	double delay_;
	double animTime_;
public:

	Sprite3D(Sprite3D* sprite, unsigned int numberOfFrames) {
		numberOfFrames_ = sprite->numberOfFrames_;
		modelListe_ = new TexturedMesh* [numberOfFrames];
		for (size_t i = 0; i < numberOfFrames_; i++) modelListe_[i] = sprite->modelListe_[i];
		currentNumberOfFrames_ = sprite->currentNumberOfFrames_;
		currentFrame_ = sprite->currentFrame_;
		textureId_ = sprite->textureId_;
		delay_ = sprite->delay_;
		animTime_ = 0.0;
	}


	Sprite3D(unsigned int numberOfFrames) {
		numberOfFrames_ = numberOfFrames;
		modelListe_ = new TexturedMesh*[numberOfFrames];
		currentNumberOfFrames_ = 0;
		currentFrame_ = 0;
	}

	inline void SetDelay(double delay) {
		delay_ = delay;
	}

	inline double GetDelay() {
		return delay_;
	}

	void UpdateFrame(double deltatime) {

		if (delay_) {
			animTime_ += deltatime;
			if (animTime_ > delay_) {
				SetNextFrame();
				animTime_ = 0;
			}
		}
	}


	void Transform(Matrix44d matrix) {
		for (int i = 0; i < numberOfFrames_; i++) {
			modelListe_[i]->Transform(matrix);
		}
	}

	void SetModel(unsigned int modelFrame){
		currentFrame_ = modelFrame;
	}

	void AddFrame(TexturedMesh* frame) {
		if (currentNumberOfFrames_ < numberOfFrames_) {
			modelListe_[currentNumberOfFrames_] = frame;
			currentNumberOfFrames_++;
		}
	}

	unsigned int GetNumberOfFrames() {
		return currentNumberOfFrames_;
	}

	void Draw() {
		if (textureId_) {
			glBindTexture(GL_TEXTURE_2D, textureId_);
			modelListe_[currentFrame_]->Draw();
		}
	}

	void SetNextFrame() {
		if (currentFrame_ < numberOfFrames_-1)
			currentFrame_++;
		else
			currentFrame_ = 0;
	}

	void SetAllTexture(unsigned int texturedId) {
		textureId_ = texturedId;
		for (int i = 0; i < numberOfFrames_; i++) {
			modelListe_[i]->setTexture(texturedId);
		}
	}

	
	
	
	TexturedMesh* getFrame(unsigned int frame) {
		return modelListe_[frame];
	}

	TexturedMesh** getModelListe_() {
		return modelListe_;
	}

	void* GetData() {
		return this;
	}
};


#endif 