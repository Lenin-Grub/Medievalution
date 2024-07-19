#pragma once
#include "../Core/Core.h"

struct Node
{
    bool               walkable = false;    // ����� �� ������
    bool               is_visited = false;  // �������� ��
    float              gCost = 0;           // ���������� �� ���������� ���� �� �������� ����  ����������� ��������� ���� �� ��������� ����� �� ����� n.
    float              hCost = 0;           // ��������� ���������� �� �������� ���� �� ��������� ���� ������������� ������ ��������� ���� �� ����� n �� ���� (������ ��� ������ ���������, ��������, � �������������� ��������� ����������).
    float              fCost = 0;           // g + h ����� ��������� ����
    Node* parent = nullptr;
    sf::Vector2f       position;
    std::vector<Node*> neighbors;

    bool operator()(Node* a, Node* b)
    {
        return a->fCost > b->fCost;
    }
};