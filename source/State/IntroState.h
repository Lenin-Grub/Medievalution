#pragma once
#include "../State/State.h"
#include "../State/MainMenuState.h"

//------------------------------------------------------------------------------------------------------------------------
// основной стейт игры
class IntroState :
	public State
{
private:
	int load = 0;
	sf::Shader shader;
	sf::RectangleShape shape;
	sf::Sprite spr;
	//sf::Texture texture;
private:
	void initEntities();												// инициализация сущностей
	void renderGUI(sf::RenderTarget& target);							// рендер кнопок
//------------------------------------------------------------------------------------------------------------------------
public:
	IntroState(StateData* state_data);
	virtual ~IntroState();

	void updateEvents() override;												// обновление событий
	void updateImGui() override;												// обновление IMGUI
	void update(const float& dtime) override;									// обновление
	void render(sf::RenderTarget* target = nullptr) override;					// рендер
};
//------------------------------------------------------------------------------------------------------------------------