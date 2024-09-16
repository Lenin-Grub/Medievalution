#pragma once
#include "../Core/Core.h"

struct Node
{
    bool               walkable = false;    // можно ли пройти
    bool               is_visited = false;  // посетили ли
    float              gCost = 0;           // Расстояние от начального узла до текущего узла  Фактическая стоимость пути от начальной точки до точки n.
    float              hCost = 0;           // Примерное расстояние от текущего узла до конечного узла эвристическая оценка стоимости пути от точки n до цели (обычно это прямая дистанция, например, с использованием евклидова расстояния).
    float              fCost = 0;           // g + h Общая стоимость пути
    Node* parent = nullptr;
    sf::Vector2f       position;
    std::vector<Node*> neighbors;

    bool operator()(Node* a, Node* b)
    {
        return a->fCost > b->fCost;
    }
};