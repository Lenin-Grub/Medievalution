#include "../stdafx.h"
#include "../World/WorldMap.h"

WorldMap::WorldMap()
{
	loadMapData();
	loadProvincesMap();
	loadShader();
	initProvinceData();
	setUniformes();
	shape.setRadius(5);
	shape.setFillColor(sf::Color::Red);
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

void WorldMap::setUniformes()
{
	shader.setUniform("map_texture", sf::Shader::CurrentTexture);
	shader.setUniform("transperency", transperency);
	shader.setUniform("select_color", sf::Glsl::Vec4(select_color));
	shader.setUniform("width",	(float)map_texture.getSize().x);
	shader.setUniform("height", (float)map_texture.getSize().y);
}

void WorldMap::loadShader()
{
	//to do @ загружать карту через загрузчик
	if (!shader.loadFromFile("shaders/map_vert.vert", "shaders/map_color_change.frag"))
		LOG_ERROR("Shader not found");

	map_texture.loadFromImage(this->map_image);
	s_province_map.setTexture(map_texture);
	s_texture_map.setTexture(s_texture);
}

void WorldMap::initCentreOfProvinces()
{
	for (std::vector<Province>::iterator it = provinces.begin(); it != provinces.end(); ++it) 
	{
		sf::Vector2f center = { findProvinceCenter((*it).color) };
		(*it).centre = center;
	}
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
	auto res = std::find_if(provinces.begin(), provinces.end(), [this](Province p) {return p.color == current_color; });

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
	auto res = std::find_if(provinces.begin(), provinces.end(), [this](Province p) {return p.color == current_color; });

	if (res == provinces.end())
	{
		LOG_WARN("Province name not found");
		return "not_found";
	}
	else
		return res->name;
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
	target.draw(s_texture_map);
	target.draw(s_province_map, &shader);
	target.draw(shape);
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
	 return	current_color = map_image.getPixel(core::mouse_pos_view.x, core::mouse_pos_view.y);
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

			if (pixels[index + 3] != 0 &&
				pixels[index]		== provinceColor.r &&
				pixels[index + 1]	== provinceColor.g &&
				pixels[index + 2]	== provinceColor.b &&
				pixels[index + 3]	== provinceColor.a)
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
		return sf::Vector2f(-1.0f, -1.0f);
	}
}

sf::Vector2f WorldMap::getProvinceCenter(const std::string& provinceName) const
{
	for (const auto& province : provinces)
	{
		if (province.name == provinceName)
		{
			return province.centre;
		}
	}
}

sf::Vector2f WorldMap::getProvinceCenter(const sf::Color& color) const
{
	for (const auto& province : provinces) 
	{
		if (province.color == color) 
		{
			return province.centre;
		}
	}
	// Вернуть недопустимый центр (-1, -1) в случае отсутствия провинции
	return sf::Vector2f(-1.f, -1.f);
}