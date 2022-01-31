#ifndef OBSERVER_H
#define OBSERVER_H

template <typename T>
class Observer {
public:
	virtual void Notification(T data) = 0;
};
#endif