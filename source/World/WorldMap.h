#pragma once
#include "../Core/Core.h"
#include "../Resource/ResourceManager.hpp"
#include "Province.h"

class WorldMap 
	:public sf::Drawable
{
public:

	WorldMap();
	virtual ~WorldMap();

	void init();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	const int			getProvinceID()		const;
	const std::string	getProvinceName()	const;

	sf::Color getColor();
	sf::Image	map_image;

	int	findProvinceID(sf::Color color);

private:

	void loadProvincesMap();		
	void loadMapData();				
	void initProvinceData();		
	bool isMouseOnMap();			

private:

	Province province;

	sf::Color	currentColor;										
										
	sf::Texture map_texture;										
	sf::Texture s_texture;											
	sf::Sprite	s_province_map;										
	sf::Sprite	s_texture_map;			

	std::ifstream file;												
	std::string id, r, g, b, name, comment;	

	float height;
	float width;

public:
	sf::Shader  shader;												
	float transperency = 0.0f;
	sf::Color owner_color;
	sf::Color select_color;
	std::vector<Province> provinces;
};