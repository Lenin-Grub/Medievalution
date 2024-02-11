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
	shader.setUniform("width", (float)map_texture.getSize().x);
	shader.setUniform("height", (float)map_texture.getSize().y);
}

void WorldMap::loadShader()
{
	//to do @ ��������� ����� ����� ���������
	if (!shader.loadFromFile("shaders/map_vert.vert", "shaders/map_color_change.frag"))
		LOG_ERROR("Shader not found");

	map_texture.loadFromImage(this->map_image);
	s_province_map.setTexture(map_texture);
	s_texture_map.setTexture(s_texture);
}

/*
	todo @ ��� ������� ��� ������
	������ ����� �� 1 ���� ������ ����� �� ���� 10000+ ���������
	� ������ ���� ������� ������� ���� � ���������
	����� �������� ��������
	���� ��������� ������� � ������� ���� �� ����������, ������ �� �����
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
	todo @ ��� ������� ��� ������
	������ ����� �� 1 ���� ������ ����� �� ���� 10000+ ���������
	� ������ ���� ������� ������� ���� � ���������
	����� �������� ��������
	���� ��������� ������� � ������� ���� �� ����������, ������ �� �����
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

bool WorldMap::isMouseOnMap()
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
	todo @ ��� ������� ��� ������
	������ ����� �� 1 ���� ������ ����� �� ���� 10000+ ���������
	� ������ ���� ������� ������� ���� � ���������
	����� �������� ��������
	���� ��������� ������� � ������� ���� �� ����������, ������ �� �����
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

sf::Vector2f WorldMap::findProvinceCenter(sf::Color provinceColor)
{
	// �������� �� ���� �������� ����������� � ������� ������� ���������� ��� �������� ����� ���������
	int pixelCount = 0;
	int sumX = 0;
	int sumY = 0;

	for (int y = 0; y < map_image.getSize().y; ++y)
	{
		for (int x = 0; x < map_image.getSize().x; ++x)
		{
			if (map_image.getPixel(x, y) == provinceColor)
			{
				sumX += x;
				sumY += y;
				pixelCount++;
			}
		}
	}

	// ��������� ������� ���������� �������� ����� ��������� ��� ����������� ������ ���������
	float centerX = static_cast<float>(sumX) / pixelCount;
	float centerY = static_cast<float>(sumY) / pixelCount;

	return sf::Vector2f(centerX, centerY);
}