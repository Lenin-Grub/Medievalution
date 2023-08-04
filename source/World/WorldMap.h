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
	sf::Color getColorUnderCursor();

	int	findProvinceByColor(sf::Color color);

private:

	void loadProvincesMap();		
	void loadMapData();				
	void initProvinceData();		
	bool isMouseOnMap();			

private:

	Province province;

	sf::Color	pixelColor;											
	sf::Color	currentColor;										
	sf::Image	map_image;											
	sf::Texture map_texture;										
	sf::Texture s_texture;											
	sf::Sprite	s_province_map;										
	sf::Sprite	s_texture_map;										
	sf::Shader  shader;												

	std::ifstream file;												
	std::string id, r, g, b, name, comment;			

public:
	float transperency = 0.0f;
	std::vector<Province> provinces;
};