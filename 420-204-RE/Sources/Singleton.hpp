#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton {
private:
	static T* instance;

public:
	static T* GetInstance() {
		return instance;
	}
};
template <typename T>
T* Singleton<T>::instance = new T();

#endif