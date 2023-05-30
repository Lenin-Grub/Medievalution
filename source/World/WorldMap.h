#pragma once
#include "../Core/Core.h"
#include "../Resource/ResourceManager.hpp"

//------------------------------------------------------------------------------------------------------------------------
class WorldMap 
	:public sf::Drawable
{
//------------------------------------------------------------------------------------------------------------------------
public:
	WorldMap();
	virtual ~WorldMap();
//------------------------------------------------------------------------------------------------------------------------
public:
//------------------------------------------------------------------------------------------------------------------------
	void init();
	void update();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	const int			getProvinceID()		const;						
	const std::string	getProvinceName()	const;						

	template <typename T>
	const T				findProvinceByColor(sf::Color color) const;			
//------------------------------------------------------------------------------------------------------------------------
private:
	void loadProvincesMap();		
	void loadMapData();				
	void initProvinceData();		
	void pickColor();	// возвращает цвет под курсором и запоминает его при нажатии лкм,
	void fillColor();	// выполняет заливку области на назначенный цвет
	bool isMouseOnMap();			
	sf::Color getColorUnderCursor();
//------------------------------------------------------------------------------------------------------------------------
private:
	sf::Color	pixelColor;											
	sf::Color	currentColor;										
	sf::Image	map_image;											
	sf::Texture map_texture;										
	sf::Texture s_texture;											
	sf::Sprite	s_province_map;										
	sf::Sprite	s_texture_map;										
	sf::Shader  shader;												

	std::ifstream file;												
	std::string ID, R, G, B, COMMENT, COMMENT_2;					
//------------------------------------------------------------------------------------------------------------------------
public:
	float transperency = 0.0f;
//------------------------------------------------------------------------------------------------------------------------
	struct ProvinceData
	{
		int id;
		std::string name;
		std::string comment;
		sf::Color _color;
	};
//------------------------------------------------------------------------------------------------------------------------
	ProvinceData province;				
	std::vector<ProvinceData> provinces;
};
//------------------------------------------------------------------------------------------------------------------------

// Very bad, bad code !!! What if provinces container will be empty?
template<typename T>
inline const T WorldMap::findProvinceByColor(sf::Color color) const
{
	if (color == provinces.data()->_color)
	{
		return T();
	}
	return -1;
}
