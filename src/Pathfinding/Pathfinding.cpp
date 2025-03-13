#include "stdafx.h"
#include "Pathfinding.h"

Pathfinding::Pathfinding()
    : start_node(nullptr)
    , end_node(nullptr)
    , current_node(nullptr)
    , tile_size(sf::Vector2f(32, 32))
    , width(50)
    , height(50)
{
}

void Pathfinding::initNodes(int width, int height)
{
    this->width = width;
    this->height = height;
    nodes.clear();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            sf::Vector2f position(x * tile_size.x, y * tile_size.y);
            addNode(position);

            // Connect nodes to their neighbors
            if (x > 0)
            {
                Node* left = getNode(sf::Vector2f((x - 1) * tile_size.x, y * tile_size.y));
                connect(getNode(position), left);
            }
            if (y > 0)
            {
                Node* up = getNode(sf::Vector2f(x * tile_size.x, (y - 1) * tile_size.y));
                connect(getNode(position), up);
            }
            if (x > 0 && y > 0)
            {
                Node* leftUp = getNode(sf::Vector2f((x - 1) * tile_size.x, (y - 1) * tile_size.y));
                connect(getNode(position), leftUp);
            }
            if (x < width - 1 && y > 0)
            {
                Node* rightUp = getNode(sf::Vector2f((x + 1) * tile_size.x, (y - 1) * tile_size.y));
                connect(getNode(position), rightUp);
            }
        }
    }
}


void Pathfinding::draw(sf::RenderWindow& window) 
{
    sf::Vertex line[] = 
    {
       sf::Vertex(sf::Vector2f(0, 0)),
       sf::Vertex(sf::Vector2f(0, 0))
    };

    sf::RectangleShape rect(tile_size);
    sf::CircleShape    circle(3.0f);
    circle.setFillColor(sf::Color::Black);

    // Draw nodes
    //for (const auto& pair : nodes)
    //{
    //    const Node& node = pair.second;
    //    rect.setPosition(node.position.x, node.position.y);

    //    if (&node == start_node)
    //        rect.setFillColor(sf::Color::Green);
    //    else if (&node == end_node)
    //        rect.setFillColor(sf::Color::Red);
    //    else
    //        rect.setFillColor(sf::Color::Transparent);
    //        window.draw(rect);

        // Draw a black circle if the node has no neighbors
        //if (node.neighbors.empty())
        //{
        //    circle.setPosition(node.position.x + circle.getRadius(), node.position.y + circle.getRadius());
        //        window.draw(circle);
        //}
    //}

    // Draw lines between neighbors
    //for (const auto& pair : nodes)
    //{
    //    const Node& node = pair.second;
    //    for (const Node* neighbor : node.neighbors)
    //    {
    //        line[0].position = sf::Vector2f(node.position.x, node.position.y);
    //        line[1].position = sf::Vector2f(neighbor->position.x, neighbor->position.y);
    //        line[0].color = sf::Color::Cyan;
    //        line[1].color = sf::Color::Cyan;
    //        window.draw(line, 2, sf::Lines);
    //    }
    //}

    // Draw path
    if (end_node != nullptr)
    {
        glLineWidth(5.0f);
        Node* p = end_node;
        while (p->parent != nullptr)
        {
            line[0].position = sf::Vector2f(p->position.x, p->position.y);
            line[1].position = sf::Vector2f(p->parent->position.x, p->parent->position.y);
            line[0].color = sf::Color::Blue;
            line[1].color = sf::Color::Blue;
            window.draw(line, 2, sf::Lines);
            p = p->parent;
        }
        glLineWidth(1.0f);
    }
}

void Pathfinding::handleInput() 
{
    int x = common::mouse_pos_view.x;
    int y = common::mouse_pos_view.y;

    sf::Vector2f position(x, y);
    Node* node = getNode(position);

    if (node) 
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) 
            node->walkable = false;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            node->walkable = true;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
            resetWalkable();
    }
}

void Pathfinding::findPath(Node* start, Node* end) 
{
    resetNodes();
    start_node = start;
    end_node   = end;

    std::priority_queue<Node*, std::vector<Node*>, Node> openList;
    std::vector<Node*> closedList;

    if (start_node == nullptr || end_node == nullptr)
        return;

    if (start_node == end_node)
        return;

    start_node->gCost = 0.0f;
    start_node->hCost = heuristic(start_node, end_node);
    start_node->fCost = start_node->gCost + start_node->hCost;

    openList.push(start_node);

    while (!openList.empty()) 
    {
        Node* current = openList.top();
        openList.pop();

        closedList.push_back(current);

        if (current == end_node)
            return;

        for (Node* neighbor : current->neighbors) 
        {
            if (neighbor->walkable && !neighbor->is_visited) 
            {
                float tentativeGCost = current->gCost + 1;
                if (tentativeGCost < neighbor->gCost) 
                {
                    neighbor-> gCost     = tentativeGCost;
                    neighbor-> hCost     = heuristic(neighbor, end_node);
                    neighbor-> fCost     = neighbor-> gCost + neighbor-> hCost;
                    neighbor-> parent    = current;
                    openList.push(neighbor);
                    neighbor->is_visited = true;
                }
            }
        }
    }
}

void Pathfinding::resetNodes() 
{
    for (auto& pair : nodes) 
    {
        Node& node           = pair.second;
        node.is_visited      = false;
        node.gCost           = INFINITY;
        node.fCost           = INFINITY;
        node.parent          = nullptr;
    }
}

int Pathfinding::heuristic(Node* start, Node* end) 
{
    int manhattanDistance = std::abs(start->position.x - end->position.x) + std::abs(start->position.y - end->position.y);
    int node_penalty = 1; // You can adjust this penalty value as needed
    return manhattanDistance + node_penalty;
}

void Pathfinding::resetWalkable() 
{
    for (auto& pair : nodes) 
    {
        Node& node = pair.second;
        node.walkable = true;
    }
}

void Pathfinding::move(float deltaTime) 
{
    if (end_node == nullptr || end_node->parent == nullptr)
        return;

    if (current_node == nullptr)
        current_node = start_node;

    Node* next_node = current_node->parent;

    if (next_node != nullptr)
        current_node = next_node;
}

void Pathfinding::addNode(const sf::Vector2f& position) 
{
    if (nodes.find(position) != nodes.end()) 
        return;

    Node node;
    node.position   = position;
    node.walkable   = true;
    node.is_visited = false;
    node.parent     = nullptr;
    node.gCost      = 0;
    nodes[position] = node;
}

void Pathfinding::connect(Node* node1, Node* node2) 
{
    if (node1 && node2 && node1 != node2) 
    {
        if (std::find(node1->neighbors.begin(), node1->neighbors.end(), node2) == node1->neighbors.end()) 
            node1->neighbors.push_back(node2);

        if (std::find(node2->neighbors.begin(), node2->neighbors.end(), node1) == node2->neighbors.end()) 
            node2->neighbors.push_back(node1);
    }
}

void Pathfinding::disconnect(Node* node1, Node* node2) 
{
    if (node1 && node2 && node1 != node2) 
    {
        auto it1 = std::find(node1->neighbors.begin(), node1->neighbors.end(), node2);
        if (it1 != node1->neighbors.end())
            node1->neighbors.erase(it1);

        auto it2 = std::find(node2->neighbors.begin(), node2->neighbors.end(), node1);
        if (it2 != node2->neighbors.end())
            node2->neighbors.erase(it2);
    }
}

Node* Pathfinding::getNode(const sf::Vector2f& position) 
{
    auto it = nodes.find(position);
    if (it != nodes.end())
        return &it->second;
    return nullptr;
}

std::vector<Node*> Pathfinding::path() const 
{
    std::vector<Node*> path;
    if (end_node == nullptr || end_node->parent == nullptr)
        return path;

    Node* current = end_node;
    while (current != nullptr) 
    {
        path.push_back(current);
        current = current->parent;
    }

    std::reverse(path.begin(), path.end());
    return path;
}

Node* Pathfinding::getNodeByMousePosition(const sf::Vector2f& mousePosition)
{
    int x = static_cast<int>(mousePosition.x / tile_size.x);
    int y = static_cast<int>(mousePosition.y / tile_size.y);

    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        sf::Vector2f nodePosition(x * tile_size.x, y * tile_size.y);
        return getNode(nodePosition);
    }

    return nullptr;
}