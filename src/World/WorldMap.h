#pragma once
#include "../Core/Core.h"
#include "../Resource/ResourceManager.hpp"
#include "Province.h"

class WorldMap 
	:public sf::Drawable
{
public:
	WorldMap ();
	virtual		 ~WorldMap ();
	bool		 init();
	virtual      void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	const        int getProvinceID () const;
	const        std::string getProvinceName() const;
	int          findProvinceID (sf::Color color);
	sf::Color    getColor ();
	sf::Vector2f findProvinceCenter (sf::Color provinceColor) const;
	sf::Vector2f getProvinceCenter  (const std::string& provinceName) const;
	sf::Vector2f getProvinceCenter  (const sf::Color& color) const;
	bool         initProvinceData   ();

public:
	int                   load_progress;
	float                 transparency;
	sf::Shader            shader;
	sf::Color             select_color;
	sf::Image             map_image;
	std::vector<Province> provinces;
	sf::CircleShape       shape;

private:
	void loadProvincesMap();
	void loadMapData     ();
	void setUniforms     ();
	void loadShader      ();
	bool isMouseOnMap    () const;

private:
	Province      province;
	sf::Color     current_color;
	sf::Texture   map_texture;
	sf::Texture   s_texture;
	sf::Sprite    s_province_map;
	sf::Sprite    s_texture_map;

	std::ifstream file;
	std::string   id, r, g, b, name, comment;

	float         height;
	float         width;
};