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

	shader.setUniform("owner_color", sf::Glsl::Vec4(owner_color));
	shader.setUniform("select_color", sf::Glsl::Vec4(select_color));
	shader.setUniform("width", (float)map_texture.getSize().x);
	shader.setUniform("height", (float)map_texture.getSize().y);
}

WorldMap::~WorldMap()
{
	file.close();
}

void WorldMap::loadMapData()
{
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
	if (!map_image.loadFromFile("resources/Map/Provinces.png"))
		LOG_ERROR("resources/Map/Provinces.png\t not found!");

	if (!s_texture.loadFromFile("resources/Map/Colormap2.jpg"))
	{
		LOG_WARN("Colormap2.jpg not found!");
	}
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

	s_province_map.setTexture(map_texture);

	if (!shader.loadFromFile("shaders/map_vert.vert", "shaders/map_color_change.frag"))
	{
		LOG_WARN("Shaders not found!");
	}

	shader.setUniform("map_texture", sf::Shader::CurrentTexture);



	s_texture_map.setTexture(s_texture);
}

/*
	todo @ при высоком фпс лагает
	скорее всего за 1 кадр делает поиск по всем 10000+ провинций
	и каждый кадр сверяет текущий цвет с текстурой
	нужно добавить проверку
	если провинция найдена и позиция мыши не изменилась, искать не нужно
*/

const int WorldMap::getProvinceID() const
{
	auto res = std::find_if(provinces.begin(), provinces.end(), [this](Province p) {return p.color == currentColor; });

	if (res == provinces.end())
	{
		LOG_WARN("Province id not found");
		return 0;
	}
	else
	{
		return res->id;
	}
}

/*
	todo @ при высоком фпс лагает
	скорее всего за 1 кадр делает поиск по всем 10000+ провинций
	и каждый кадр сверяет текущий цвет с текстурой
	нужно добавить проверку
	если провинция найдена и позиция мыши не изменилась, искать не нужно
*/

const std::string WorldMap::getProvinceName() const
{
	auto res = std::find_if(provinces.begin(), provinces.end(), [this](Province p) {return p.color == currentColor; });

	if (res == provinces.end())
	{
		LOG_WARN("Province name not found");
		return "not_found";
	}
	else
		return res->name;
}

bool WorldMap::isMouseOnMap()
{
	if (core::mousePosView.x >= 0 && 
		core::mousePosView.y >= 0 && 
		core::mousePosView.x <= map_image.getSize().x && 
		core::mousePosView.y <= map_image.getSize().y)
		return true;
	else
		return false;
}

void WorldMap::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(s_texture_map);
	target.draw(s_province_map, &shader);
}

/*
	todo @ при высоком фпс лагает
	скорее всего за 1 кадр делает поиск по всем 10000+ провинций
	и каждый кадр сверяет текущий цвет с текстурой
	нужно добавить проверку
	если провинция найдена и позиция мыши не изменилась, искать не нужно
*/

sf::Color WorldMap::getColor()
{
	if (isMouseOnMap())
	{
	 return	currentColor = map_image.getPixel(core::mousePosView.x, core::mousePosView.y);
	}
	else
		return sf::Color::White;
}

int WorldMap::findProvinceID(sf::Color color)
{
	if (color == getColor())
	{
		return getProvinceID();
	}
	else
		return 0;
}
