#pragma once
#include <UI/IDisplay.hpp>
#include <imgui.h>
#include <cmath>

#define M_PI 3.14159265358979323846

class CompassWidget {
public:
    CompassWidget(float size = 150.0f, int num_directions = 8);

    // Рисует виджет и возвращает true, если значение изменилось
    bool draw(const char* label);

    // Получить/установить текущее направление (в градусах, 0 = Север)
    float getDirection() const { return current_direction; }
    void setDirection(float direction);

    // Получить направление в виде индекса (0-7 для 8 направлений)
    int getDirectionIndex() const;

    // Установить направление по индексу
    void setDirectionIndex(int index);

private:
    float size;
    int num_directions;
    float current_direction; // В градусах (0-360)
    bool is_dragging;

    // Размеры элементов
    float radius;
    float inner_radius;

    // Направления
    static const char* direction_names[8];
    static const char* direction_names_short[8];

    // Вспомогательные функции
    ImVec2 screenToCircle(const ImVec2& screen_pos, const ImVec2& center) const;
    float vectorToAngle(const ImVec2& vec) const;
    ImVec2 angleToVector(float angle) const;
    float snapToNearestDirection(float angle) const;
    void drawCompass(ImDrawList* draw_list, const ImVec2& center);
};

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
    CompassWidget compas;
};
