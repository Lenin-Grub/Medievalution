#include "../stdafx.h"
#include "Battleground.h"

bool Battleground::load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
{
    // load the tileset texture
    if (!m_tileset.loadFromFile(tileset))
        return false;

    // resize the vertex array to fit the level size
    vertices.setPrimitiveType(sf::Triangles);
    vertices.resize(width * height * 6);

    // populate the vertex array, with two triangles per tile
    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
            // get the current tile number
            int tileNumber = tiles[i + j * width];

            // find its position in the tileset texture
            int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

            // get a pointer to the triangles' vertices of the current tile
            sf::Vertex* triangles = &vertices[(i + static_cast<size_t>(j) * width) * 6];

            // define the 6 corners of the two triangles
            triangles[0].position  = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            triangles[1].position  = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            triangles[2].position  = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
            triangles[3].position  = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
            triangles[4].position  = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            triangles[5].position  = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);

            // define the 6 matching texture coordinates
            triangles[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            triangles[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            triangles[2].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            triangles[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            triangles[4].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            triangles[5].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
        }

    return true;
}


void Battleground::update(unsigned int x, unsigned int y, int tileNumber)
{
    if (x >= 0 && x < 16 && y >= 0 && y < 16)
    {
        // Find the position of the tile in the tileset texture
        int tu = tileNumber % (m_tileset.getSize().x / 64);
        int tv = tileNumber / (m_tileset.getSize().x / 64);

        // Get a pointer to the triangles' vertices of the current tile
        sf::Vertex* triangles = &vertices[(x + y * 16) * 6];

        // Update the texture coordinates of the triangles
        triangles[0].texCoords = sf::Vector2f(tu * 64, tv * 64);
        triangles[1].texCoords = sf::Vector2f((tu + 1) * 64, tv * 64);
        triangles[2].texCoords = sf::Vector2f(tu * 64, (tv + 1) * 64);
        triangles[3].texCoords = sf::Vector2f(tu * 64, (tv + 1) * 64);
        triangles[4].texCoords = sf::Vector2f((tu + 1) * 64, tv * 64);
        triangles[5].texCoords = sf::Vector2f((tu + 1) * 64, (tv + 1) * 64);
    }
}

void Battleground::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
//    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_tileset;

    // draw the vertex array
    target.draw(vertices, states);
}