#pragma once

class Observer
{
public:
	virtual ~Observer() = default;

	virtual void updateObserver() = 0;
private:

};