#include "stdafx.h"
#include "SettingsState.h"

void SettingsState::initView()				// установка камеры вида
{
	S::view.setSize(sf::Vector2f(this->stateData->graphicSettings->resolution.width, this->stateData->graphicSettings->resolution.height));
	S::view.setCenter(sf::Vector2f(this->stateData->graphicSettings->resolution.width / 2.f, this->stateData->graphicSettings->resolution.height / 2.f));
}

void SettingsState::initVariables()			// инициализация различных вещей
{
	//fps
	this->fpsText = S::сreateText(v2f(), 16, "FPS: " + to_string(fps_counter), S::fonts._font, sf::Color::Black);
	this->fpsText.setPosition(v2f(this->fpsText.getGlobalBounds().width + 160, this->fpsText.getGlobalBounds().height / 2));
	//версия
	version_text = S::сreateText(v2f(this->window->getSize().x - 100, this->window->getSize().y - 35), 14, "prototype\n18.03.2021", S::fonts._font, Color(255, 255, 255, 100));
}

void SettingsState::initGUI()				//инициализация GUI
{
	this->buttons["APPLAY"] = new Button(this->window->getSize().x - 110, this->window->getSize().y - 110, 100, 50, L"Принять", 12, S::fonts._font);
	this->buttons["EXIT"] = new Button(this->window->getSize().x - 110, this->window->getSize().y- 60, 100, 50, L"Выход", 12, S::fonts._font);

	this->checkboxs["test"] = new CheckBox(this->window->getSize().x/2,this->window->getSize().y/2,50,50, S::res.textureResources.useTexture("THUMB_DOWN"));
}

void SettingsState::updateGUI()				// обвноление GUI
{
	for (auto& it : this->buttons)			// обновляем кнопку
	{
		it.second->update(S::mousePosWindow);
	}
	if (this->buttons["APPLAY"]->isWidgetPressed()) { /*S::audio.sounds.setSounds(0); S::audio.sounds.playSound();*/ cout<<"Debug:: Настройки изменены"<<endl; }
	if (this->buttons["EXIT"]->isWidgetPressed()) { /*S::audio.sounds.setSounds(1); S::audio.sounds.playSound();*/ this->endState(); cout << "Debug:: Вы вышли из меню настроек" << endl; }
	this->checkboxs["test"]->update(S::mousePosWindow);
}

void SettingsState::renderGUI(sf::RenderTarget& target)		//рисуем кнопки
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
	this->checkboxs["test"]->render(target);
}

SettingsState::SettingsState(StateData* state_data)
	:State(state_data)
{
	cout << "Debug:: Вы зашли в меню настроек" << endl;
	this->initView();					// вид
	this->initVariables();				// переменные
	this->initGUI();					// интерфейс
	this->updateEvents();				// разовый вызов обновления событий для корректного отображения кнопок
}

SettingsState::~SettingsState()
{

}

void SettingsState::updateEvents()
{
	this->updateGUI();
}

void SettingsState::update(const float& dtime)									//обновляем все
{
	this->updateMousePositions();
}

void SettingsState::render(sf::RenderTarget* target)							//рисуем все
{
	if (!target)
		target = this->window.get();

	target->setView(S::view);													// установлена камера вида
	this->renderGUI(*target);
	target->draw(this->fpsText);												// fps
	target->draw(this->version_text);											// версия

	target->draw(this->mouseCordsText);											// координаты
}