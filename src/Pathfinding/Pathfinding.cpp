#include "stdafx.h"
#include "Pathfinding.h"

Pathfinding::Pathfinding()
    : start_node(nullptr)
    , end_node(nullptr)
    , current_node(nullptr)
    , tile_size(sf::Vector2f(64, 32))
    , width(50)
    , height(50)
    , is_path_visible{true}
    , is_nodes_visible(false)
    , is_connections_visible(false)
    , is_beginend_visible(false)
    , path_mode (PathMode::Isometric)
{
}

void Pathfinding::initNodes(int width, int height)
{
    this->width  = width;
    this->height = height;
    nodes.clear();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            sf::Vector2f position;

            if (path_mode == PathMode::Isometric)
            {
                float isoX = (x - y) * tile_size.x / 2;
                float isoY = (x + y) * tile_size.y / 2;
                position = { isoX, isoY };
            }
            else
            {
                position = { x * tile_size.x, y * tile_size.y };
            }

            addNode(position);
            connectNeighbors(x, y, width, height, true);
        }
    }
}


void Pathfinding::draw(sf::RenderWindow& window)
{
    sf::Vertex line[2];
    sf::RectangleShape rect(tile_size);

    // Draw lines between neighbors
    if (is_connections_visible) 
    {
        for (const auto& pair : nodes)
        {
            const Node& node = pair.second;
            for (const auto& [neighbor, cost] : node.edge_costs) 
            {
                sf::Color color = (cost > 1.0f) ? sf::Color::Yellow : sf::Color::Cyan; // colors for diagonal
                line[0].position = node.position + sf::Vector2f(16, 16);
                line[1].position = neighbor->position + sf::Vector2f(16, 16);
                line[0].color = color;
                line[1].color = color;

                window.draw(line, 2, sf::Lines);
            }
        }
    }

    // Draw nodes
    if (is_beginend_visible || is_nodes_visible)
    {
        for (const auto& pair : nodes)
        {
            const Node& node = pair.second;
            rect.setPosition(node.position);

            if (is_beginend_visible)
            {
                if (&node == start_node)
                    rect.setFillColor(sf::Color::Green);
                else if (&node == end_node)
                    rect.setFillColor(sf::Color::Red);
                else if (is_nodes_visible && !node.walkable)
                    rect.setFillColor(sf::Color::Red);
                else
                    continue;

                window.draw(rect);
            }
            else if (is_nodes_visible && !node.walkable)
            {
                rect.setFillColor(sf::Color::Red);
                window.draw(rect);
            }
        }
    }

  
    // Draw path
    if (is_path_visible && end_node != nullptr)
    {
        glLineWidth(5.0f);
        Node* p = end_node;
        while (p->parent != nullptr)
        {
            line[0].position = p->position + sf::Vector2f(16, 16);
            line[1].position = p->parent->position + sf::Vector2f(16, 16);
            line[0].color    = sf::Color::Blue;
            line[1].color    = sf::Color::Blue;

            window.draw(line, 2, sf::Lines);
            p = p->parent;
        }
        glLineWidth(1.0f);
    }
}


void Pathfinding::handleInput() 
{
    Node* node = getNodeByMousePosition(common::mouse_pos_view);

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

bool compareNodes(Node* a, Node* b)
{
    return a->fCost > b->fCost; // Minimum fCost has the highest priority
}

void Pathfinding::findPath(Node* start, Node* end)
{
    resetNodes();
    start_node = start;
    end_node = end;

    if (!start || !end || start == end) return;

    std::priority_queue<Node*, std::vector<Node*>, decltype(&compareNodes)> openList(compareNodes);
    std::unordered_set<Node*> closedSet;

    start->gCost = 0.0f;
    start->hCost = heuristic(start, end);
    start->fCost = start->gCost + start->hCost;
    openList.push(start);

    while (!openList.empty()) 
    {
        Node* current = openList.top();
        openList.pop();

        if (current == end) 
            break;

        closedSet.insert(current);

        for (Node* neighbor : current->neighbors) 
        {
            if (!neighbor->walkable || closedSet.count(neighbor)) continue;

            float edgeCost       = current->edge_costs[neighbor];
            float tentativeGCost = current->gCost + edgeCost;

            if (tentativeGCost < neighbor->gCost) 
            {
                neighbor->gCost  = tentativeGCost;
                neighbor->hCost  = heuristic(neighbor, end);
                neighbor->fCost  = neighbor->gCost + neighbor->hCost;
                neighbor->parent = current;

                if (!closedSet.count(neighbor)) 
                {
                    openList.push(neighbor);
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

int Pathfinding::heuristic(Node* start, Node* end, HeuristicType type)
{
    int dx = std::abs(start->position.x - end->position.x);
    int dy = std::abs(start->position.y - end->position.y);

    switch (type) 
    {
    case HeuristicType::Manhattan:
        return dx + dy;
    case HeuristicType::Euclidean:
        return static_cast<int>(std::sqrt(dx * dx + dy * dy));
    case HeuristicType::Diagonal:
        return std::max(dx, dy);
    default:
        return dx + dy;
    }
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

void Pathfinding::connect(Node* node1, Node* node2, float cost = 1.0f)
{
    if (node1 && node2 && node1 != node2) 
    {
        node1->neighbors.push_back(node2);
        node1->edge_costs[node2] = cost;

        node2->neighbors.push_back(node1);
        node2->edge_costs[node1] = cost;
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
    if (path_mode == PathMode::Isometric)
    {
        sf::Vector2i gridPos = getMouseGridPosition();

        if (gridPos.x >= 0 && gridPos.y >= 0 && gridPos.x < width && gridPos.y < height)
        {
            float screenX = (gridPos.x - gridPos.y) * (tile_size.x / 2);
            float screenY = (gridPos.x + gridPos.y) * (tile_size.y / 2);

            sf::Vector2f nodePosition(screenX, screenY);          
            return getNode(nodePosition);
        }
    }
    else if (path_mode == PathMode::Regular)
    {
        int x = static_cast<int>(mousePosition.x / tile_size.x);
        int y = static_cast<int>(mousePosition.y / tile_size.y);

        if (x >= 0 && x < width && y >= 0 && y < height)
        {
            sf::Vector2f nodePosition(x * tile_size.x, y * tile_size.y);
            return getNode(nodePosition);
        }
    }

    return nullptr;
}

Node* Pathfinding::getRandomEndNode() const
{
    // Extract nodes into a vector
    std::vector<Node*> nodeVector;
    for (const auto& pair : nodes) 
    {
        nodeVector.push_back(const_cast<Node*>(&pair.second));
    }

    // Return a random node from the vector
    return nodeVector[std::rand() % nodeVector.size()];
}

sf::Vector2i Pathfinding::getMouseGridPosition()
{
    const float tileW = 64, tileH = 32;
    const float halfW = tileW / 2, halfH = tileH / 2;

    float mx = common::mouse_pos_view.x; /* + layer_index        */
    float my = common::mouse_pos_view.y; /* + halfH * layer_index*/

    int tileX = (my / halfH + mx / halfW) / 2;
    int tileY = (my / halfH - mx / halfW) / 2;

    float screenX = (tileX - tileY) * halfW;
    float screenY = (tileX + tileY) * halfH;

    float relX = common::mouse_pos_view.x - screenX;
    float relY = common::mouse_pos_view.y - screenY;

    return common::mouse_pos_grid = { tileX, tileY };
}

void Pathfinding::setPathMode(PathMode mode)
{
    path_mode = mode;
}

void Pathfinding::connectNeighbors(int x, int y, int map_width, int map_height, bool connect_diagonals)
{
    Node* current = getNodeByGridPosition(sf::Vector2i( x, y ));

    if (!current) return;

    const int dx[] = { -1, 0, 1, 0 }; // left, up, right, down
    const int dy[] = { 0, -1, 0, 1 };

    for (int i = 0; i < 4; ++i)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx >= 0 && ny >= 0 && nx < map_width && ny < map_height)
        {
            Node* neighbor = getNodeByGridPosition(sf::Vector2i( nx, ny ));
            if (neighbor)
                connect(current, neighbor, 1.0f);
        }
    }

    if (connect_diagonals)
    {
        const int ddx[] = { -1, 1, -1, 1 };
        const int ddy[] = { -1, -1, 1, 1 };

        for (int i = 0; i < 4; ++i)
        {
            int nx = x + ddx[i];
            int ny = y + ddy[i];

            if (nx >= 0 && ny >= 0 && nx < map_width && ny < map_height)
            {
                Node* diagNeighbor = getNodeByGridPosition(sf::Vector2i( nx, ny ));
                if (diagNeighbor)
                    connect(current, diagNeighbor, std::sqrt(2.0f));
            }
        }
    }
}

Node* Pathfinding::getNodeByGridPosition(sf::Vector2i pos)
{
    for (auto& pair : nodes)
    {
        sf::Vector2i gridPos;

        if (path_mode == PathMode::Isometric)
        {
            float isoX = pair.first.x;
            float isoY = pair.first.y;
            gridPos.x = (isoX / (tile_size.x / 2) + isoY / (tile_size.y / 2)) / 2;
            gridPos.y = (isoY / (tile_size.y / 2) - isoX / (tile_size.x / 2)) / 2;
        }
        else
        {
            gridPos.x = static_cast<int>(pair.first.x / tile_size.x);
            gridPos.y = static_cast<int>(pair.first.y / tile_size.y);
        }

        if (gridPos == pos)
            return &pair.second;
    }

    return nullptr;
}