#pragma once

class Observer
{
public:
    Observer() {};
    virtual ~Observer() {};

    virtual void updateObserver() = 0;

private:
};