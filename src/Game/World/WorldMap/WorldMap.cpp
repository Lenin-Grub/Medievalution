#include "WorldMap.h"

WorldMap::WorldMap()
    : transparency(0)
    , load_progress(0)
    , height(0)
    , width(0)
    , is_selected(false)
    , selected_province_color(sf::Color::White)
    , default_color(sf::Color::White)
{
    // Do nothing
}

WorldMap::~WorldMap()
{
    file.close();
}

bool WorldMap::init()
{
    if (!loadMapData())      return false;
    if (!loadProvincesMap()) return false;
    if (!loadShaders())      return false;
    if (!isInitProvinces())  return false;

    s_province_map.setTexture(map_texture);
    s_texture_map.setTexture(s_texture);
    map_size = color_map_texture.getSize();

    setQuads();
    setUniforms();

    return true;
}

bool  WorldMap::loadMapData()
{
    try
    {
        file.open("resources/Map/Provinces.csv");
        return true;
    }
    catch (const std::ifstream::failure& ex)
    {
        LOG_ERROR("File <<Provinces.csv>> not found");
        LOG_ERROR(ex.what());
        return false;
    }
}

bool WorldMap::loadProvincesMap()
{
    try
    {
        map_image         = ResourceLoader::instance().getImage("Provinces.png");
        s_texture         = ResourceLoader::instance().getTexture("Greyscale.jpg");
        atlas_texture     = ResourceLoader::instance().getTexture("tiles.jpg");
        color_map_texture = ResourceLoader::instance().getTexture("Colormap.png");

        return true;
    }
    catch (const std::ifstream::failure& ex)
    {
        LOG_ERROR("Province map not found");
        LOG_ERROR(ex.what());
        return false;
    }
}

bool WorldMap::isInitProvinces()
{
    std::string str;
    provinces.reserve(9000);
    if (!file.is_open())
        return false;

    auto provinceCenters = findAllProvinceCenters();

    while (std::getline(file, str))
    {
        std::stringstream ss(str);

        std::getline(ss, id,      ';');
        std::getline(ss, r,       ';');
        std::getline(ss, g,       ';');
        std::getline(ss, b,       ';');
        std::getline(ss, name,    ';');
        std::getline(ss, comment, ';');

        province.id      = std::stoi(id);
        province.name    = name;
        province.comment = comment;

        province.color.r = std::stoi(r);
        province.color.g = std::stoi(g);
        province.color.b = std::stoi(b);

        auto it = provinceCenters.find(province.color);
        if (it != provinceCenters.end())
            province.centre = it->second;

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
    shader_texture.setUniform("atlas", atlas_texture);
    shader_texture.setUniform("colormap", color_map_texture);
    shader_texture.setUniform("tile_size", sf::Vector2f(256, 256));
    shader_texture.setUniform("atlas_size", sf::Vector2f(2048, 2048));

    shader_border.setUniform("map_texture", sf::Shader::CurrentTexture);
    shader_border.setUniform("transparency", transparency);
    shader_border.setUniform("select_color", sf::Glsl::Vec4(select_color));
    shader_border.setUniform("width", (float)map_texture.getSize().x);
    shader_border.setUniform("height", (float)map_texture.getSize().y);
}

void WorldMap::setQuads()
{
    quad = sf::VertexArray(sf::Quads, 4);
    quad[0].position = sf::Vector2f(0, 0);
    quad[1].position = sf::Vector2f(map_size.x, 0);
    quad[2].position = sf::Vector2f(map_size.x, map_size.y);
    quad[3].position = sf::Vector2f(0, map_size.y);

    quad[0].texCoords = sf::Vector2f(0, 0);
    quad[1].texCoords = sf::Vector2f(1, 0);
    quad[2].texCoords = sf::Vector2f(1, 1);
    quad[3].texCoords = sf::Vector2f(0, 1);
}

bool WorldMap::loadShaders()
{
    if (!shader_border.loadFromFile("shaders/map_vert.vert", "shaders/map_color_change.frag"))
    {
        LOG_ERROR("Shader border not found!");
        return false;
    }

    if (!map_texture.loadFromImage(this->map_image))
    {
        LOG_ERROR("Map texture not loaded");
        return false;
    }

    if (!shader_texture.loadFromFile("shaders/map_texture.frag", sf::Shader::Fragment))
    {
        LOG_ERROR("Shader texture not found!");
        return false;
    }

    return true;
}

const sf::Color WorldMap::getColor()
{
    if (isMouseOnMap())
    {
        current_color = map_image.getPixel(common::mouse_pos_view.x, common::mouse_pos_view.y);
        return current_color;
    }
    else
        return default_color;
}

const sf::Color WorldMap::getColor(sf::Vector2f pos)
{
    if (isMouseOnMap())
    {
        current_color = map_image.getPixel(pos.x, pos.y);
        return current_color;
    }
    else
        return default_color;
}

int WorldMap::getProvinceID(const sf::Color& color) const
{
    if (!isMouseOnMap())
        return 0;

    sf::Vector2f pos = common::mouse_pos_view;
    if (color != map_image.getPixel(pos.x, pos.y))
        return 0;

    auto it = provinces.find(color);
    if (it == provinces.end())
        return 0;

    return it->second.id;
}

int WorldMap::getProvinceID(const sf::Color& color, sf::Vector2f pos) const
{
    if (color != map_image.getPixel(pos.x, pos.y))
        return 0;

    auto it = provinces.find(color);
    if (it == provinces.end())
        return 0;

    return it->second.id;
}

const std::string WorldMap::getProvinceName(const sf::Color& color) const
{
    if (!isMouseOnMap())
        return "Province not found";

    if (color != map_image.getPixel(common::mouse_pos_view.x, common::mouse_pos_view.y))
        return "Province not found";

    auto it = provinces.find(color);
    if (it == provinces.end())
        return "Province not found";

    return it->second.name;
}

const std::string WorldMap::getProvinceName(const sf::Color& color, sf::Vector2f pos) const
{
    if (!isMouseOnMap())
        return "Province not found";

    if (color != map_image.getPixel(pos.x, pos.y))
        return "Province not found";

    auto it = provinces.find(color);
    if (it == provinces.end())
        return "Province not found";

    return it->second.name;
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

void WorldMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //target.draw(s_texture_map);
    target.draw(quad, &shader_texture);
    target.draw(s_province_map, &shader_border);
}

sf::Vector2f WorldMap::findProvinceCenter(sf::Color provinceColor) const
{
    const sf::Uint8* pixels = map_image.getPixelsPtr();
    sf::Vector2u mapSize = map_image.getSize();

    float pixelCount = 0.0f;
    float sumX = 0.0f;
    float sumY = 0.0f;

    for (unsigned int i = 0; i < mapSize.y; ++i)
    {
        unsigned int rowStart = i * mapSize.x * 4;
        for (unsigned int j = 0; j < mapSize.x; ++j)
        {
            unsigned int index = rowStart + j * 4;

            if (pixels[index + 3] != 0               &&
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

std::unordered_map<sf::Color, sf::Vector2f, WorldMap::ColorHash> WorldMap::findAllProvinceCenters() const
{
    std::unordered_map<sf::Color, std::pair<float, float>, ColorHash> provinceSum;
    std::unordered_map<sf::Color, float, ColorHash> provincePixelCount;

    const sf::Uint8* pixels = map_image.getPixelsPtr();
    sf::Vector2u mapSize    = map_image.getSize();

    for (unsigned int i = 0; i < mapSize.y; ++i)
    {
        unsigned int rowStart = i * mapSize.x * 4;
        for (unsigned int j = 0; j < mapSize.x; ++j)
        {
            unsigned int index = rowStart + j * 4;
            sf::Color provinceColor(pixels[index], pixels[index + 1], pixels[index + 2], pixels[index + 3]);

            if (pixels[index + 3] != 0) // Check if the pixel is not transparent
            {
                provinceSum[provinceColor].first  += j;
                provinceSum[provinceColor].second += i;
                provincePixelCount[provinceColor]++;
            }
        }
    }

    std::unordered_map<sf::Color, sf::Vector2f, ColorHash> provinceCenters;
    for (const auto& entry : provinceSum)
    {
        const sf::Color& provinceColor = entry.first;
        float pixelCount = provincePixelCount[provinceColor];
        if (pixelCount > 0.0f)
        {
            float centerX = entry.second.first  / pixelCount;
            float centerY = entry.second.second / pixelCount;
            provinceCenters[provinceColor] = sf::Vector2f(centerX, centerY);
        }
    }

    return provinceCenters;
}