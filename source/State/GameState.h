#pragma once
#include "../World/WorldMap.h"
#include "../Camera/Camera.h"
#include "StateManager.h"

//------------------------------------------------------------------------------------------------------------------------
// основной стейт игры
class GameState :
    public State
{
private:
	sf::Text province_name;												// название провинции
	sf::Text mouseText;;												// координаты мыши
	sf::Shader shader;													// шейдер
	
	std::stringstream cords;											// координаты мыши вывод

	WorldMap _map;														// карта
	Camera camera;														// камера

//------------------------------------------------------------------------------------------------------------------------
private:
	void initEntities();												// инициализация сущностей
	void renderGUI(sf::RenderTarget& target);							// рендер кнопок
//------------------------------------------------------------------------------------------------------------------------
public:
	GameState(StateData* state_data);
	virtual ~GameState();

	void updateEvents() override;												// обновление событий
	void updateImGui() override;												// обновление IMGUI
	void update(const float& dtime) override;									// обновление
	void render(sf::RenderTarget* target = nullptr) override;					// рендер
};
//------------------------------------------------------------------------------------------------------------------------