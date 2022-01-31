///\file Effect.hpp
///\brief Effects

#ifndef EFFECT_HPP
#define EFFECT_HPP
#include <map>
#include <string>
#include "Chrono.hpp"

using namespace std;


enum EffectType {
	MOVESPEED = 1,
	JUMP = 2
};


///\class Effect
///\brief Effets
class Effect {
	double lifespan_;		///< Durée de l'effet
	unsigned short value_;	///< Multiplicateur de l'effet
	Chrono  duration_;		///< Temps depuis le début de l'effet
public:
	///\brief Constructeur
	///\param lifespan Durée de l'effet
	///\param value Multiplicateur de l'effet
	Effect(double lifespan, unsigned short value) {
		Refresh(lifespan, value);
	}

	///\brief Recalcul des valeurs
	///\param lifespan Durée de l'effet
	///\param value Multiplicateur de l'effet
	void Refresh(double lifespan, unsigned short value) {
		lifespan_ = lifespan;
		value_ = value;
		duration_.Reset();
	}

	///\brief Durées de vie restante de l'effet
	inline double GetTime() {
		return (lifespan_ - duration_.GetElapsedSeconds());
	}

	///\brief Multiplicateur de l'effet
	inline unsigned short GetValue() {
		return value_;
	}

	///\brief Tester la durée de vie de l'effet
	bool IsExpired() {
		if ((lifespan_ - duration_.GetElapsedSeconds()) <= 0.0)
			return true;
		return false;
	}

};



///\class EffectManager
///\brief Gestionnaire des effets
class EffectManager {
private:
	static map<EffectType, Effect*> effects_; ///< Contient les effets et leur identifiant
public:

	///\brief Ajouter une effet au gestionnaire
	///\param type Identifiant
	///\param effect effet a charger
	static void AddEffect(EffectType type, Effect* effect) {
		effects_[type] = effect;
	}

	///\brief Retirer un effet du gestionnaire
	///\param type Identifiant
	static void RemoveEffect(EffectType type) {
		delete effects_[type];
		effects_.erase(type);
	}

	///\brief Durées de vie restante de l'effet voulu
	///\param type Identifiant
	static double GetRemainingTime(EffectType type) {
		if (effects_.find(type) != effects_.end())
			return effects_[type]->GetTime();
		return NULL;
	}

	///\brief Multiplicateur de l'effet voulu
	///\param type Identifiant
	static unsigned short GetValue(EffectType type) {
		if (effects_.find(type) != effects_.end())
			return effects_[type]->GetValue();
		return NULL;
	}

	///\brief Tester la durée de vie des effets et les retirer au besoin
	static void ExpirationTest() {
		for (auto it : effects_) {
			if (it.second->IsExpired())
				RemoveEffect(it.first);
			break;
		}
	}

	///\brief Teste si l'effet existe
	///\param type Identifiant
	static bool EffectExists(EffectType type) {
		if (effects_.find(type) != effects_.end())
			return true;
		return false;
	}
	///\brief Teste si l'effet existe
	///\param type Identifiant
	///\param lifespan Durée de l'effet
	///\param value Multiplicateur de l'effet
	static void Refresh(EffectType type, double lifespan, unsigned short value) {
		if (effects_.find(type) != effects_.end())
			effects_[type]->Refresh(lifespan, value);
	}

};
map<EffectType, Effect*> EffectManager::effects_;


#endif //EFFECT_HPP
