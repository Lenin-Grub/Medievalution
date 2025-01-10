#include "../stdafx.h"
#include "../World/WorldMap.h"

WorldMap::WorldMap()
: transparency (0)
, load_progress(0)
, height       (0)
, width        (0)
{
// Do nothing
}

WorldMap::~WorldMap()
{
    file.close();
}

bool WorldMap::init()
{
    loadMapData();
    loadProvincesMap();
    loadShader();
    isInitProvinces();
    setUniforms();
    shape.setRadius(5);
    shape.setFillColor(sf::Color::Red);

    return false;
}

void WorldMap::loadMapData()
{
    try
    {
        file.open("resources/Map/Provinces.csv");
    }
    catch (const std::ifstream::failure & ex)
    {
        LOG_ERROR("File <<Provinces.csv>> not found");
        LOG_ERROR(ex.what());
    }
}

void WorldMap::loadProvincesMap()
{
    map_image = ResourceLoader::instance().getImage("Provinces.png");
    s_texture = ResourceLoader::instance().getTexture("Colormap2.jpg");
}

bool WorldMap::isInitProvinces() 
{
    std::string str;
    provinces.reserve(9000);
    if(!file.is_open())
        return false;

    while (std::getline(file, str))
    {
        std::stringstream ss(str);

        std::getline(ss, id,     ';');
        std::getline(ss, r,	     ';');
        std::getline(ss, g,	     ';');
        std::getline(ss, b,	     ';');
        std::getline(ss, name,   ';');
        std::getline(ss, comment,';');

        province.id      = std::stoi(id);
        province.name    = name;
        province.comment = comment;

        province.color.r = std::stoi(r);
        province.color.g = std::stoi(g);
        province.color.b = std::stoi(b);

        provinces.emplace(province.color, province);
        ++load_progress;
    }
    file.close();
    if (!provinces.empty())
        return true;
    else
        return false;
}

void WorldMap::setUniforms()
{
    shader.setUniform ("map_texture", sf::Shader::CurrentTexture);
    shader.setUniform ("transparency", transparency);
    shader.setUniform ("select_color", sf::Glsl::Vec4(select_color));
    shader.setUniform ("width",	(float)map_texture.getSize().x);
    shader.setUniform ("height", (float)map_texture.getSize().y);
}

void WorldMap::loadShader()
{
    if (!shader.loadFromFile("shaders/map_vert.vert", "shaders/map_color_change.frag"))
        LOG_ERROR("Shader not found");

    map_texture.loadFromImage(this->map_image);
    s_province_map.setTexture(map_texture);
    s_texture_map.setTexture (s_texture);
}

const sf::Color WorldMap::getColor()
{
    if (isMouseOnMap())
    {
        return	current_color = map_image.getPixel(common::mouse_pos_view.x, common::mouse_pos_view.y);
    }
    else
        return sf::Color::White;
}

int WorldMap::getProvinceID(const sf::Color& color) const
{
    if (!isMouseOnMap())
        return 0;
    if (color == map_image.getPixel(common::mouse_pos_view.x, common::mouse_pos_view.y))
        return provinces.find(color)->second.id;
    else
        return 0;
}

const std::string WorldMap::getProvinceName(const sf::Color& color) const
{
    if (!isMouseOnMap())
        return "not_found";
    if (color == map_image.getPixel(common::mouse_pos_view.x, common::mouse_pos_view.y))
        return provinces.find(color)->second.name;
    else
        return "not_found";
}

bool WorldMap::isMouseOnMap() const
{
    if (common::mouse_pos_view.x >= 0 && 
        common::mouse_pos_view.y >= 0 && 
        common::mouse_pos_view.x <= map_image.getSize().x && 
        common::mouse_pos_view.y <= map_image.getSize().y)
        return true;
    else
        return false;
}

void WorldMap::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    target.draw (s_texture_map);
    target.draw (s_province_map, &shader);
    target.draw (shape);
}

sf::Vector2f WorldMap::findProvinceCenter(sf::Color provinceColor) const
{
    const sf::Uint8* pixels = map_image.getPixelsPtr();
    sf::Vector2u mapSize    = map_image.getSize();

    float pixelCount = 0.0f;
    float sumX       = 0.0f;
    float sumY       = 0.0f;

    for (unsigned int i = 0; i < mapSize.y; ++i) 
    {
        unsigned int rowStart = i * mapSize.x * 4;
        for (unsigned int j = 0; j < mapSize.x; ++j) 
        {
            unsigned int index = rowStart + j * 4;

            if (pixels[index + 3] != 0 &&
                pixels[index]     == provinceColor.r &&
                pixels[index + 1] == provinceColor.g &&
                pixels[index + 2] == provinceColor.b &&
                pixels[index + 3] == provinceColor.a)
            {
                sumX += j;
                sumY += i;
                pixelCount++;
            }
        }
    }

    if (pixelCount > 0.0f) 
    {
        float centerX = sumX / pixelCount;
        float centerY = sumY / pixelCount;
        return sf::Vector2f(centerX, centerY);
    }
    else 
    {
        return sf::Vector2f(0, 0);
    }
}