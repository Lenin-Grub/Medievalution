#pragma once
#include "../State/State.h"
#include "../State/MainMenuState.h"
#include "../Observer/Observable.hpp"

//------------------------------------------------------------------------------------------------------------------------
// основной стейт игры
class IntroState :
	public State,
	public Observable
{
private:
	int load = 0;

	sf::Shader shader;
	sf::RectangleShape shape;
	sf::Sprite spr;

	std::list<Observer*> observer_list;
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

	void addObserver(Observer& observer)	override;
	void removeObserver(Observer& observer)	override;
	void notifyObservers()					override;
};
//------------------------------------------------------------------------------------------------------------------------