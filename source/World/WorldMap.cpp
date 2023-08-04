#include "../stdafx.h"
#include "../World/WorldMap.h"

WorldMap::WorldMap()
{
	loadMapData();
	loadProvincesMap();
	initProvinceData();
	if (!shader.loadFromFile("shaders/map_vert.vert", "shaders/map_color_change.frag"))
		LOG_ERROR("Shader not found");
	shader.setUniform("map_texture", sf::Shader::CurrentTexture);
	shader.setUniform("transperency", transperency);
}

WorldMap::~WorldMap()
{
	file.close();
}

void WorldMap::loadMapData()
{
	//file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	try
	{
		file.open("resources/Map/Provinces.csv");
	}
	catch (const std::ifstream::failure & ex)
	{
		LOG_ERROR("File <<Proivinces.csv>> not found");
		LOG_ERROR(ex.what());
	}
}

void WorldMap::loadProvincesMap()
{
	if (!map_image.loadFromFile("resources/Map/Provinces.bmp"))
		LOG_ERROR("resources/Map/Provinces.png\t not found!");
}

void WorldMap::initProvinceData()
{
	std::string str;
	while (std::getline(file, str))
	{
		std::stringstream ss(str);

		std::getline(ss, id,		';');
		std::getline(ss, r,			';');
		std::getline(ss, g,			';');
		std::getline(ss, b,			';');
		std::getline(ss, name,		';');
		std::getline(ss, comment,	';');

		province.id = std::stoi(id);
		province.name = name;
		province.comment = comment;

		province.color.r = std::stoi(r);
		province.color.g = std::stoi(g);
		province.color.b = std::stoi(b);

		provinces.push_back(province);
	}
}

void WorldMap::init()
{
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

void WorldMap::update()
{
	getColorUnderCursor();
	shader.setUniform("transperency", transperency);
}

const int WorldMap::getProvinceID() const
{
	auto res = std::find_if(provinces.begin(), provinces.end(), [this] (Province p)
		{
			return p.color == currentColor;
		});
	return res->id;
}

const std::string WorldMap::getProvinceName() const
{
	auto res = std::find_if(provinces.begin(), provinces.end(), [this](Province p)
		{
			return p.color == currentColor;
		});
	return res->name;
}

bool WorldMap::isMouseOnMap()
{
	if (core::mousePosView.x >= 0 && core::mousePosView.y >= 0 && core::mousePosView.x <= map_image.getSize().x && core::mousePosView.y <= map_image.getSize().y)
		return true;
	else
		return false;
}

void WorldMap::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(s_texture_map);
	target.draw(s_province_map, &shader);
}

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

int WorldMap::findProvinceByColor(sf::Color color)
{
	if (color == getColorUnderCursor())
	{
		return getProvinceID();
	}
	else
		return 0;
}
