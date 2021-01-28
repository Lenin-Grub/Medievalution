#include "stdafx.h"
#include "GameState.h"

void GameState::initView()				// установка камеры вида
{
	S::view.setSize(sf::Vector2f(this->stateData->graphicSettings->resolution.width, this->stateData->graphicSettings->resolution.height));
	S::view.setCenter(sf::Vector2f(this->stateData->graphicSettings->resolution.width / 2.f, this->stateData->graphicSettings->resolution.height / 2.f));
}

void GameState::initVariables()			// инициализация различных вещей
{
	this->fpsText = S::сreateText(v2f(), 16, "FPS: " + to_string(fps_counter), S::fonts._font, sf::Color::Black);
	this->fpsText.setPosition(v2f(this->fpsText.getGlobalBounds().width + 160, this->fpsText.getGlobalBounds().height / 2));
	version_text = S::сreateText(v2f(this->window->getSize().x - 100, this->window->getSize().y - 35), 14, "prototype\n11.01.2021", S::fonts._font, Color(255, 255, 255, 100));

	this->map = new Map(S::gridSize, S::mapSize, S::mapSize, 1);
}

void GameState::initGUI()				//инициализация GUI
{
	this->buttons["EXIT"] = (new Button(this->window->getSize().x - 60, 10, 50, 50, S::textures.GUIcons[0]));
	this->buttons["BUILD"] = new Button(this->window->getSize().x - 60, this->window->getSize().y - 110, 50, 50, S::textures.GUIcons[1]);
	this->buttons["DESTROY"] = new Button(this->window->getSize().x - 60, this->window->getSize().y - 60, 50, 50, S::textures.GUIcons[2]);
}

void GameState::updateGUI()				// обвноление GUI
{
	for (auto& it : this->buttons)			// обновляем кнопку
	{
		it.second->update(this->mousePosWindow);
	}
	if (this->buttons["EXIT"]->isWidgetPressed()) { S::audio.sounds.setSounds(1); S::audio.sounds.playSound(); this->endState(); cout << "Debug:: Вы вышли из игры" << endl; }
	if (this->buttons["BUILD"]->isWidgetPressed()) { S::audio.sounds.setSounds(2); S::audio.sounds.playSound();}
	if (this->buttons["DESTROY"]->isWidgetPressed()) { S::audio.sounds.setSounds(3); S::audio.sounds.playSound(); }
	if (isMouseReleased(sf::Mouse::Left))
	{
		//S::audio.sounds.setSounds(2); S::audio.sounds.playSound();
		//this->map->addTile(this->mousePosGrid.x, this->mousePosGrid.y,0,3);
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		//this->map->removeTile(this->mousePosGrid.x, this->mousePosGrid.y,0);
	}
}

void GameState::renderGUI(sf::RenderTarget& target)		//рисуем кнопки
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}

void GameState::updateView(const float& dtime)
{
	//движение камеры WASD
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {this->view.move(-10 * dtime * 0.1, 0); }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {this->view.move(10 * dtime * 0.1, 0); }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {this->view.move(0, -10 * dtime * 0.1); }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {this->view.move(0, 10 * dtime * 0.1); }
	//движение камеры мышкой у края экрана
	if (State::mousePosWindow.x < 3) { this->view.move(-10 * dtime * 0.1, 0); }
	if (State::mousePosWindow.x > this->window->getSize().x - 3) { this->view.move(10 * dtime * 0.1, 0); }
	if (State::mousePosWindow.y > this->window->getSize().y - 3) { this->view.move(0, 10 * dtime * 0.1); }
	if (State::mousePosWindow.y < 3) { this->view.move(0, -10 * dtime * 0.1); }
}

GameState::GameState(StateData* state_data)
	:State(state_data)
{
	cout << "Debug:: Вы зашли в меню настроек" << endl;
	this->initView();					// вид
	this->initVariables();				// переменные
	this->initGUI();					// интерфейс
}

GameState::~GameState()
{
	delete this->map;
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
}

void GameState::updateEvents()
{
	this->updateGUI();
}

void GameState::update(const float& dtime)			//обновляем все
{
	this->updateMousePositions();
	this->updateView(dtime);
}

void GameState::render(sf::RenderTarget* target)							//рисуем все
{
	if (!target)
		target = this->window;
	///////////
	target->setView(S::view);
	//рисуем динамические объекты тут (игрок и тд)
	this->map->render(*target);
	//sf::Text mouseText;
	//mouseText.setPosition(this->mousePosView.x + 20, this->mousePosView.y);
	//mouseText.setFont(S::fonts._font);
	//mouseText.setCharacterSize(10);
	//std::stringstream cords;
	//cords << "mPos\t" << this->mousePosView.x << " " << this->mousePosView.y
	//	<< "\nwPos\t" << this->mousePosWindow.x << " " << this->mousePosWindow.y
	//	<< "\nsPos\t" << this->mousePosScreen.x << " " << this->mousePosScreen.y
	//	<< "\ngPos\t" << this->mousePosGrid.x << " " << this->mousePosGrid.y;
	//mouseText.setString(cords.str());
	//target->draw(mouseText);
	///////////
	target->setView(this->window->getDefaultView());
	//рисуем статические объекты тут (gui и тд)
	this->renderGUI(*target);
	target->draw(this->fpsText);												// fps
	target->draw(this->version_text);											// версия
	target->draw(this->mouseCordsText);											// координаты
	target->setView(S::view);													// возвращаем камеру вида
}