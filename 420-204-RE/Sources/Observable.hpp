#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <list>

#include "Observer.hpp"


template <typename T>
class Observable {
private:
	std::list<Observer<T>*> observers;

public:
	inline void Subscribe(Observer<T>* observer) {
		observers.push_back(observer);
	}

	inline void Unsubscribe(Observer<T>* observer) {
		observers.remove(observer);
	}

	void Notify(T data) {
		for (auto it : observers)
			it->Notification(data);
	}
};
#endif