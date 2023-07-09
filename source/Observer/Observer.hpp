#include "../Core/Core.h"

class Observer
{
public:
	virtual ~Observer() = default;

	virtual void update() = 0;
private:

};