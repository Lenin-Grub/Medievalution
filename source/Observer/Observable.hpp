#include "../Core/"

class Observable
{
public:
	Observable() = default;
	virtual ~Observable() = default;
	// �������� �����������
	virtual void addObserver()		= 0;
	// ������� �����������
	virtual void removeObserver()	= 0;
	// ���������� �����������
	virtual void notifyObservers()	= 0;

private:

};