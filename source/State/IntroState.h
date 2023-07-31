#pragma once
#include "../State/State.h"
#include "../State/MainMenuState.h"
#include "../Observer/Observable.hpp"

//------------------------------------------------------------------------------------------------------------------------
// �������� ����� ����
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
	void initEntities();												// ������������� ���������
	void renderGUI(sf::RenderTarget& target);							// ������ ������

//------------------------------------------------------------------------------------------------------------------------
public:
	IntroState(StateData* state_data);
	virtual ~IntroState();

	void updateEvents() override;												// ���������� �������
	void updateImGui() override;												// ���������� IMGUI
	void update(const float& dtime) override;									// ����������
	void render(sf::RenderTarget* target = nullptr) override;					// ������

	void addObserver(Observer& observer)	override;
	void removeObserver(Observer& observer)	override;
	void notifyObservers()					override;
};
//------------------------------------------------------------------------------------------------------------------------