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
    if (!map_image.loadFromFile("resources/Map/Provinces.png"))
        LOG_ERROR("resources/Map/Provinces.png\t not found!");

    if (!s_texture.loadFromFile("resources/Map/Colormap2.jpg"))
    {
        LOG_ERROR("Colormap2.jpg not found!");
    }
}

bool WorldMap::isInitProvinces() 
{
    std::string str;
    provinces.reserve(9000);
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
        return	current_color = map_image.getPixel(core::mouse_pos_view.x, core::mouse_pos_view.y);
    }
    else
        return sf::Color::White;
}

int WorldMap::getProvinceID(const sf::Color& color) const
{
    if (!isMouseOnMap())
        return 0;
    if (color == map_image.getPixel(core::mouse_pos_view.x, core::mouse_pos_view.y))
        return provinces.find(color)->second.id;
    else
        return 0;
}

const std::string WorldMap::getProvinceName(const sf::Color& color) const
{
    if (!isMouseOnMap())
        return "not_found";
    if (color == map_image.getPixel(core::mouse_pos_view.x, core::mouse_pos_view.y))
        return provinces.find(color)->second.name;
    else
        return "not_found";
}

bool WorldMap::isMouseOnMap() const
{
    if (core::mouse_pos_view.x >= 0 && 
        core::mouse_pos_view.y >= 0 && 
        core::mouse_pos_view.x <= map_image.getSize().x && 
        core::mouse_pos_view.y <= map_image.getSize().y)
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

#pragma region Fixe it
//void WorldMap::findAllProvinceCenters()
//{
//	const sf::Uint8* pixels = map_image.getPixelsPtr();
//	sf::Vector2u mapSize = map_image.getSize();
//
//	std::vector<float> pixelCounts(provinces.size(), 0.0f);
//	std::vector<float> sumXs(provinces.size(), 0.0f);
//	std::vector<float> sumYs(provinces.size(), 0.0f);
//
//	std::vector<bool> visited(mapSize.x * mapSize.y, false);
//
//	auto addPixelToProvince = [&](size_t k, int j, int i) {
//		sumXs[k] += j;
//		sumYs[k] += i;
//		pixelCounts[k]++;
//		};
//
//	for (unsigned int i = 0; i < mapSize.y; ++i) {
//		unsigned int rowStart = i * mapSize.x * 4;
//		for (unsigned int j = 0; j < mapSize.x; ++j) {
//			unsigned int index = rowStart + j * 4;
//
//			if (visited[i * mapSize.x + j]) {
//				continue;
//			}
//
//			bool isProvinceFound = false;
//			sf::Color currentColor(pixels[index], pixels[index + 1], pixels[index + 2], pixels[index + 3]);
//
//			for (size_t k = 0; k < provinces.size(); ++k) {
//				if (currentColor == provinces[k].color) {
//					addPixelToProvince(k, j, i);
//					isProvinceFound = true;
//					visited[i * mapSize.x + j] = true;
//				}
//			}
//
//			if (isProvinceFound) {
//				std::queue<size_t> toVisit;
//				toVisit.push(i * mapSize.x + j);
//
//				while (!toVisit.empty()) {
//					size_t currentIndex = toVisit.front();
//					toVisit.pop();
//
//					int x = currentIndex % mapSize.x;
//					int y = currentIndex / mapSize.x;
//
//					for (int k = -1; k <= 1; ++k) {
//						for (int l = -1; l <= 1; ++l) {
//							if (k == 0 && l == 0) {
//								continue;
//							}
//
//							size_t neighborIndex = currentIndex + k * mapSize.x + l;
//
//							if (neighborIndex < 0 || neighborIndex >= mapSize.x * mapSize.y || visited[neighborIndex]) {
//								continue;
//							}
//
//							sf::Color neighborColor(pixels[neighborIndex * 4], pixels[neighborIndex * 4 + 1], pixels[neighborIndex * 4 + 2], pixels[neighborIndex * 4 + 3]);
//							if (neighborColor == currentColor) {
//								addPixelToProvince(k, x, y);
//								toVisit.push(neighborIndex);
//								visited[neighborIndex] = true;
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//
//	for (size_t i = 0; i < provinces.size(); ++i)
//	{
//		if (pixelCounts[i] > 0.0f)
//		{
//			float centerX = sumXs[i] / pixelCounts[i];
//			float centerY = sumYs[i] / pixelCounts[i];
//			provinces.at(i).centre = sf::Vector2f(centerX, centerY);
//			LOG_INFO("{0} - {1}", (int)provinces.at(i).centre.x, (int)provinces.at(i).centre.y);
//		}
//	}
//}
#pragma endregion