#pragma once
#include <UI/IDisplay.hpp>

class ObjectDetailsDisplay
	: public UI::IDisplay
{
public:
	ObjectDetailsDisplay() = default;
	virtual ~ObjectDetailsDisplay() = default;

	void draw() override;
	void update(const float& delta_time) override;

private:
    bool bool_value = true;
    float float_value = 0.0f;
    float dtime = 0.0f;
    int current_item = 0;
    int int_value = 0;
    char string_buffer[128] = "Hello, World!";
    const char* ITEMS[3] = { "Item 1", "Item 2", "Item 3" };
};