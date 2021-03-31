#include "stdafx.h"
#include "MainMenuState.h"
// îò
void MainMenuState::initView()				// установка камеры вида
{
	S::view.setSize(sf::Vector2f(this->stateData->graphicSettings->resolution.width, this->stateData->graphicSettings->resolution.height));
	S::view.setCenter(sf::Vector2f(this->stateData->graphicSettings->resolution.width / 2.f, this->stateData->graphicSettings->resolution.height / 2.f));
}

void MainMenuState::initVariables()			// инициализация различных вещей
{
	this->fpsText = S::сreateText(v2f(), 16, "FPS: " + to_string(fps_counter), S::fonts._font, sf::Color::Black);
	this->fpsText.setPosition(v2f(this->fpsText.getGlobalBounds().width + 160, this->fpsText.getGlobalBounds().height / 2));
	version_text = S::сreateText(v2f(this->window->getSize().x - 100, this->window->getSize().y - 35), 14, "prototype\n18.03.2021", S::fonts._font, Color(255, 255, 255, 100));
}

void MainMenuState::initGUI()				//инициализация GUI
{
	this->buttons["GAME"]		= new Button((this->window->getSize().x / 2) - 50, (this->window->getSize().y / 2) - 150, 100, 50, L"Игра",12, S::fonts._font);
	this->buttons["SETTINGS"]	= new Button((this->window->getSize().x / 2) - 50, (this->window->getSize().y / 2) - 100, 100, 50, L"Настройки", 12, S::fonts._font);
	this->buttons["EXIT"]		= new Button((this->window->getSize().x / 2) - 50, (this->window->getSize().y / 2) - 50, 100, 50, L"Выход", 12, S::fonts._font);
}

void MainMenuState::updateGUI()				// обвноление GUI
{
	for (auto& it : this->buttons)			// обновляем кнопку
	{
		it.second->update(S::mousePosWindow);
	}
	if (this->buttons["GAME"]->isWidgetPressed()) {	this->states->push(new GameState(this->stateData)); }
	if (this->buttons["SETTINGS"]->isWidgetPressed()) { /*S::audio.sounds.setSounds(0); S::audio.sounds.playSound();*/	this->states->push(new SettingsState(this->stateData));}
	if (this->buttons["EXIT"]->isWidgetPressed()) { /*S::audio.sounds.setSounds(1); S::audio.sounds.playSound();*/		this->endState(); cout << "Debug:: Вы вышли из главного меню" << endl;}
}

void MainMenuState::renderGUI(sf::RenderTarget& target)		//рисуем кнопки
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}

MainMenuState::MainMenuState(StateData* state_data)
	:State(state_data)
{
	this->initView();					// вид
	this->initVariables();				// переменные
	this->initGUI();					// интерфейс
	this->updateEvents();				// разовый вызов обновления событий для корректного отображения кнопок
}

MainMenuState::~MainMenuState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
}

void MainMenuState::updateEvents()
{
	this->updateGUI();
}

void MainMenuState::update(const float& dtime)			//обновляем все
{
	this->updateMousePositions();
	this->updateFPS();
}

void MainMenuState::render(sf::RenderTarget* target)							//рисуем все
{
	if (!target)
		target = this->window.get();
	target->setView(this->window->getDefaultView());
	this->renderGUI(*target);
	target->draw(this->fpsText);												// fps
	target->draw(this->version_text);											// версия
	target->draw(this->mouseCordsText);											// координаты
}