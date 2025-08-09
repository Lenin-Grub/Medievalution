#include "Pathfinding.h"

#pragma region Pathfinding

Pathfinding::Pathfinding()
    : start_node(nullptr)
    , end_node(nullptr)
    , current_node(nullptr)
    , tile_size(sf::Vector2f(32, 16))
    , width(128)
    , height(128)
    , is_path_visible{false}
    , is_nodes_visible(false)
    , is_connections_visible(false)
    , is_beginend_visible(false)
    , path_mode (PathMode::Isometric)
{
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

            sf::Vector2f delta = neighbor->position - current->position;
            float baseCost = std::sqrt(delta.x * delta.x + delta.y * delta.y);
            float terrainFactor = (current->terrainCost + neighbor->terrainCost) / 2.0f;
            float edgeCost = baseCost * terrainFactor;

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

void Pathfinding::connect(Node* node1, Node* node2, float cost)
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

Node* Pathfinding::getNodeByPosition(const sf::Vector2f& worldPos)
{
    Node* closest = nullptr;
    float minDistSq = std::numeric_limits<float>::max();

    for (auto& [pos, node] : nodes)
    {
        float dx = pos.x - worldPos.x;
        float dy = pos.y - worldPos.y;
        float distSq = dx * dx + dy * dy;

        if (distSq < minDistSq)
        {
            minDistSq = distSq;
            closest = &node;
        }
    }

    // Опционально: ограничить радиус поиска
    const float maxSearchDistSq = 1024.0f; // 32*32
    return (minDistSq <= maxSearchDistSq) ? closest : nullptr;
}

const std::unordered_map<sf::Vector2f, Node, Vector2fHash>& Pathfinding::getNodes() const
{
    return nodes;
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

sf::Vector2i Pathfinding::getMouseGridPosition()
{
    const float halfW = tile_size.x/ 2, halfH = tile_size.y / 2;

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

const PathMode& Pathfinding::getPathMode() const
{
    return path_mode;
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

#pragma endregion

#pragma region RenderPathfinding

void PathfindingRenderer::render(const Pathfinding& pathfinding, sf::RenderWindow& window)
{
    if (pathfinding.is_connections_visible)
        drawConnections(pathfinding, window);

    if (pathfinding.is_nodes_visible || pathfinding.is_beginend_visible)
        drawNodes(pathfinding, window);

    if (pathfinding.is_path_visible && pathfinding.end_node)
        drawPath(pathfinding, window);
}

void PathfindingRenderer::drawConnections(const Pathfinding& pathfinding, sf::RenderWindow& window)
{
    sf::Vertex line[2];
    for (const auto& pair : pathfinding.getNodes())
    {
        const Node& node = pair.second;
        for (const auto& [neighbor, cost] : node.edge_costs)
        {
            sf::Color color = (cost > 1.0f) ? sf::Color::Yellow : sf::Color::Cyan;
            line[0].position = node.position + sf::Vector2f(16, 16);
            line[1].position = neighbor->position + sf::Vector2f(16, 16);
            line[0].color = color;
            line[1].color = color;
            window.draw(line, 2, sf::Lines);
        }
    }
}

void PathfindingRenderer::drawNodes(const Pathfinding& pathfinding, sf::RenderWindow& window)
{
    sf::RectangleShape rect(pathfinding.tile_size);
    for (const auto& pair : pathfinding.getNodes())
    {
        const Node& node = pair.second;
        rect.setPosition(node.position);

        if (pathfinding.is_beginend_visible)
        {
            if (&node == pathfinding.start_node)
            {
                rect.setFillColor(sf::Color::Green);
                window.draw(rect);
            }
        }
        else if (pathfinding.is_nodes_visible && !node.walkable)
        {
            rect.setFillColor(sf::Color::Red);
            window.draw(rect);
        }
    }

    if (pathfinding.is_nodes_visible)
    {
        for (const auto& pair : pathfinding.getNodes())
        {
            const Node& node = pair.second;
            if (node.walkable)
                drawNodeCost(pathfinding, window, node);
        }
    }
}

void PathfindingRenderer::drawPath(const Pathfinding& pathfinding, sf::RenderWindow& window)
{
    glLineWidth(5.0f);
    sf::Vertex line[2];
    Node* p = pathfinding.end_node;
    while (p && p->parent)
    {
        line[0].position = p->position + sf::Vector2f(16, 16);
        line[1].position = p->parent->position + sf::Vector2f(16, 16);
        line[0].color = sf::Color::Blue;
        line[1].color = sf::Color::Blue;
        window.draw(line, 2, sf::Lines);
        p = p->parent;
    }
    glLineWidth(1.0f);
}

void PathfindingRenderer::drawNodeCost(const Pathfinding& pathfinding, sf::RenderWindow& window, const Node& node)
{
    //sf::Text text;
    //text.setFont(font);
    //text.setCharacterSize(14);
    //text.setFillColor(getCostColor(node.terrainCost));
    //std::stringstream ss;
    //ss << std::fixed << std::setprecision(1) << node.terrainCost;
    //text.setString(ss.str());
    //sf::FloatRect bounds = text.getLocalBounds();
    //text.setOrigin(bounds.width / 2, bounds.height / 2);
    //text.setPosition(node.position.x, node.position.y + pathfinding.tile_size.y / 4);
    //window.draw(text);
}

sf::Color PathfindingRenderer::getCostColor(float cost, float maxCost)
{
    float intensity = std::min(1.0f, cost / maxCost);
    int r = static_cast<int>(255 * intensity);
    int g = static_cast<int>(255 * (1 - intensity));
    return sf::Color(r, g, 0);
}

#pragma endregion

#pragma region InputPathfinding

void PathfindingInputSystem::handleInput(Pathfinding& pathfinding)
{
    Node* node = pathfinding.getNodeByMousePosition(common::mouse_pos_view);
    if (node)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            node->walkable = false;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            node->walkable = true;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                node->terrainCost = std::min(10.0f, node->terrainCost + 0.5f);
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                node->terrainCost = std::max(0.0f, node->terrainCost - 0.5f);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
            pathfinding.resetWalkable();
    }
}

#pragma endregion

#pragma region PathfindingGrid

void GridSystem::generateGrid(Pathfinding& pathfinding, int width, int height)
{
    pathfinding.width  = width;
    pathfinding.height = height;
    pathfinding.nodes.clear();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            sf::Vector2f position;
            if (pathfinding.getPathMode()  == PathMode::Isometric)
            {
                float isoX = (x - y) * pathfinding.tile_size.x / 2;
                float isoY = (x + y) * pathfinding.tile_size.y / 2;
                position = { isoX, isoY };
            }
            else
            {
                position = { x * pathfinding.tile_size.x, y * pathfinding.tile_size.y };
            }

            pathfinding.addNode(position);
            connectNeighbors(pathfinding, x, y, width, height, true);
        }
    }
}

void GridSystem::connectNeighbors(Pathfinding& pathfinding, int x, int y, int mapWidth, int mapHeight, bool connectDiagonals)
{
    Node* current = pathfinding.getNodeByGridPosition({ x, y });
    if (!current)
        return;

    const int dx[] = { -1, 0, 1, 0 };
    const int dy[] = { 0, -1, 0, 1 };

    for (int i = 0; i < 4; ++i)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx >= 0 && ny >= 0 && nx < mapWidth && ny < mapHeight)
        {
            Node* neighbor = pathfinding.getNodeByGridPosition({ nx, ny });
            if (neighbor)
                pathfinding.connect(current, neighbor, 1.0f);
        }
    }

    if (connectDiagonals)
    {
        const int ddx[] = { -1, 1, -1, 1 };
        const int ddy[] = { -1, -1, 1, 1 };

        for (int i = 0; i < 4; ++i)
        {
            int nx = x + ddx[i];
            int ny = y + ddy[i];

            if (nx >= 0 && ny >= 0 && nx < mapWidth && ny < mapHeight)
            {
                Node* diagNeighbor = pathfinding.getNodeByGridPosition({ nx, ny });
                if (diagNeighbor)
                    pathfinding.connect(current, diagNeighbor, std::sqrt(2.0f));
            }
        }
    }
}

sf::Vector2i GridSystem::getGridPosition(const Pathfinding& pathfinding, const sf::Vector2f& mousePosition)
{
    const float tileW = pathfinding.tile_size.x;
    const float tileH = pathfinding.tile_size.y;
    const float halfW = tileW / 2, halfH = tileH / 2;

    float mx = mousePosition.x;
    float my = mousePosition.y;

    int tileX = static_cast<int>((my / halfH + mx / halfW) / 2);
    int tileY = static_cast<int>((my / halfH - mx / halfW) / 2);

    return { tileX, tileY };
}

#pragma endregion