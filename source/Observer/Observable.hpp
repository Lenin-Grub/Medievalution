#include "../Core/"

class Observable
{
public:
	Observable() = default;
	virtual ~Observable() = default;
	// добавить наблюдателя
	virtual void addObserver()		= 0;
	// удалить наблюдателя
	virtual void removeObserver()	= 0;
	// оповестить подписчиков
	virtual void notifyObservers()	= 0;

private:

};