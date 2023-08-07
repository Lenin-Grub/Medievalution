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
	void update();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	const int			getProvinceID()		const;
	const std::string	getProvinceName()	const;

	//Return sf::Color on mouse position
	sf::Color getColor();

	int	findProvinceByColor(sf::Color color);

private:

	void loadProvincesMap();		
	void loadMapData();				
	void initProvinceData();		
	bool isMouseOnMap();			

private:

	Province province;

	sf::Color	currentColor;										
	sf::Image	map_image;											
	sf::Texture map_texture;										
	sf::Texture s_texture;											
	sf::Sprite	s_province_map;										
	sf::Sprite	s_texture_map;										

	std::ifstream file;												
	std::string id, r, g, b, name, comment;			

public:
	//Use in fragment shader to hide main colors
	sf::Shader  shader;												
	float transperency = 0.0f;
	sf::Color owner_color;
	std::vector<Province> provinces;
};