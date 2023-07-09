#include "../stdafx.h"
#include "../World/WorldMap.h"

//------------------------------------------------------------------------------------------------------------------------
WorldMap::WorldMap()
{
	loadMapData();
	loadProvincesMap();
	initProvinceData();
	if (!shader.loadFromFile("shaders/map_vert.vert", "shaders/map_color_change.frag"))
		LOG_WARN("SHADER NOT FOUND");
	shader.setUniform("map_texture", sf::Shader::CurrentTexture);
	shader.setUniform("transperency", transperency);

}

WorldMap::~WorldMap()
{
	file.close();
}

//------------------------------------------------------------------------------------------------------------------------
void WorldMap::loadMapData()
{
	file.open("resources/Map/Provinces.csv");
}

//------------------------------------------------------------------------------------------------------------------------
void WorldMap::loadProvincesMap()
{
	if (!map_image.loadFromFile("resources/Map/Provinces.bmp"))
		LOG_WARN("resources/Map/Provinces.png\t NOT FOUND!");
}

//------------------------------------------------------------------------------------------------------------------------
void WorldMap::initProvinceData()
{
	std::string str;
	while (std::getline(file, str))
	{
		std::stringstream ss(str);

		std::getline(ss, ID,		';');
		std::getline(ss, R,			';');
		std::getline(ss, G,			';');
		std::getline(ss, B,			';');
		std::getline(ss, COMMENT,	';');
		std::getline(ss, COMMENT_2, ';');

		province.id = std::stoi(ID);
		province.name = COMMENT;
		province.comment = COMMENT_2;

		province._color.r = std::stoi(R);
		province._color.g = std::stoi(G);
		province._color.b = std::stoi(B);

		provinces.push_back(province);
	}
}

//------------------------------------------------------------------------------------------------------------------------
void WorldMap::init()
{
	sf::Color ocean;
	ocean = sf::Color(255, 255, 255,0);
	for (size_t x = 0;x < 20; x++)
	{
		for (size_t y = 0; y < 20; y++)
		{
			map_image.setPixel(x, y, ocean);

		}
	}
	map_image.createMaskFromColor(sf::Color::White);
	map_texture.loadFromImage(this->map_image);
	s_province_map.setTexture(map_texture, true);
	if (!shader.loadFromFile("shaders/map_vert.vert", "shaders/map_color_change.frag"))
	{
		LOG_WARN("Shaders not found!");
	}
	shader.setUniform("map_texture", sf::Shader::CurrentTexture);
	s_texture.loadFromFile("resources/Map/Colormap.jpg");
	s_texture_map.setTexture(s_texture);
}

//------------------------------------------------------------------------------------------------------------------------
void WorldMap::update()
{
	getColorUnderCursor();
	shader.setUniform("transperency", transperency);
}


//------------------------------------------------------------------------------------------------------------------------
const int WorldMap::getProvinceID() const
{
	auto res = std::find_if(provinces.begin(), provinces.end(), [this] (ProvinceData p)
		{
			return p._color == currentColor;
		});
	return res->id;
}

//------------------------------------------------------------------------------------------------------------------------
const std::string WorldMap::getProvinceName() const
{
	auto res = std::find_if(provinces.begin(), provinces.end(), [this](ProvinceData p)
		{
			return p._color == currentColor;
		});
	return res->name;
}

//------------------------------------------------------------------------------------------------------------------------
bool WorldMap::isMouseOnMap()
{
	if (core::mousePosView.x >= 0 && core::mousePosView.y >= 0 && core::mousePosView.x <= map_image.getSize().x && core::mousePosView.y <= map_image.getSize().y)
		return true;
	else
		return false;
}

//------------------------------------------------------------------------------------------------------------------------
void WorldMap::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(s_texture_map);
	target.draw(s_province_map, &shader);
}

//------------------------------------------------------------------------------------------------------------------------
sf::Color WorldMap::getColorUnderCursor()
{
	if (isMouseOnMap())
	{
		pixelColor = map_image.getPixel(core::mousePosView.x, core::mousePosView.y);
		currentColor.r = pixelColor.r,
		currentColor.g = pixelColor.g,
		currentColor.b = pixelColor.b;
	}
	return currentColor;
}
//------------------------------------------------------------------------------------------------------------------------