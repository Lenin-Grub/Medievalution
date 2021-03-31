#include "stdafx.h"
#include "GameState.h"
// îò
void GameState::initView()				// установка камеры вида
{
	S::view.setSize(sf::Vector2f(this->stateData->graphicSettings->resolution.width, this->stateData->graphicSettings->resolution.height));
	S::view.setCenter(sf::Vector2f(this->stateData->graphicSettings->resolution.width / 2.f, this->stateData->graphicSettings->resolution.height / 2.f));
}

void GameState::initVariables()			// инициализация различных вещей
{
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	this->fpsText = S::сreateText(v2f(), 16, "FPS: " + to_string(fps_counter), S::fonts._font, sf::Color::Black);
	this->fpsText.setPosition(v2f(this->fpsText.getGlobalBounds().width + 160, this->fpsText.getGlobalBounds().height / 2));
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	map.reset(new Map(S::gridSize, S::mapSize, S::mapSize));
	map->initMap();
}

void GameState::initEntities()
{
}

void GameState::initGUI()				//инициализация GUI
{
	this->buttons["EXIT"] =		new Button(this->window->getSize().x - 60, 10, 50, 50,								S::res.textureResources.useTexture("DOOR_EXIT"));
	this->buttons["BUILD"] =	new Button(this->window->getSize().x - 60, this->window->getSize().y - 110, 50, 50, S::res.textureResources.useTexture("DIG_DUG"));
	this->buttons["DESTROY"] =	new Button(this->window->getSize().x - 60, this->window->getSize().y - 60, 50, 50,	S::res.textureResources.useTexture("MINING"));
}

void GameState::updateGUI()				// обвноление GUI
{
	for (auto& it : this->buttons)		// обновляем кнопку
	{
		it.second->update(S::mousePosWindow);
	}
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	if (this->buttons["EXIT"]->isWidgetPressed()) {
		this->endState(); cout << "Debug:: Вы вышли из игры" << endl;
	}
	if (this->buttons["BUILD"]->isWidgetPressed()) {
	}
	if (this->buttons["DESTROY"]->isWidgetPressed()) { 
	}
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	if (S::input.isKeyReleased(Key::Num1))
		type = 1;
	else if (S::input.isKeyReleased(Key::Num2))
		type = 2;
	else if (S::input.isKeyReleased(Key::Num3))
		type = 3;
	else if (S::input.isKeyReleased(Key::Num4))
		type = 4;
	else if (S::input.isKeyReleased(Key::Num5))
		type = 5;
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))							//добавляем и разрушаем тайлы на карте
	{
		map->addTile(S::mousePosGrid.x, S::mousePosGrid.y,0,type);
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		map->removeTile(S::mousePosGrid.x,S::mousePosGrid.y,0);
	}
}

void GameState::renderGUI(sf::RenderTarget& target)								//рисуем кнопки
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
	target.draw(this->fpsText);													// fps

	mouseText.setPosition(S::mousePosWindow.x + 20, S::mousePosWindow.y);
	mouseText.setFont(S::fonts._font);
	mouseText.setCharacterSize(10);
	mouseText.setFillColor(sf::Color::Black);
	std::stringstream cords;
	cords << "mPos\t" << S::mousePosView.x << " " << S::mousePosView.y
		<< "\nwPos\t" << S::mousePosWindow.x << " " << S::mousePosWindow.y
		<< "\nsPos\t" << S::mousePosScreen.x << " " << S::mousePosScreen.y
		<< "\ngPos\t" << S::mousePosGrid.x << " " << S::mousePosGrid.y;
	mouseText.setString(cords.str());
}

void GameState::updateView(const float& dtime)
{
	//движение камеры WASD
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {S::view.move(-10 * dtime * 0.1f, 0); }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {S::view.move(10 * dtime * 0.1f, 0); }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {S::view.move(0, -10 * dtime * 0.1f); }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {S::view.move(0, 10 * dtime * 0.1f); }
	//движение камеры мышкой у края экрана
	if (S::sfmlEvent.type != sf::Event::MouseLeft) // если курсор не покинул пределы окна
	{
		if (S::mousePosWindow.x < 3) { S::view.move(-10 * dtime * 0.1f, 0); }
		if (S::mousePosWindow.x > this->window->getSize().x - 3) { S::view.move(10 * dtime * 0.1f, 0); }
		if (S::mousePosWindow.y > this->window->getSize().y - 3) { S::view.move(0, 10 * dtime * 0.1f); }
		if (S::mousePosWindow.y < 3) { S::view.move(0, -10 * dtime * 0.1f); }
	}
}

GameState::GameState(StateData* state_data)
	:State(state_data)
{
	cout << "Debug:: Вы зашли в меню настроек" << endl;
	this->initView();					// вид
	this->initVariables();				// переменные
	this->initEntities();				// сущности
	this->initGUI();					// интерфейс
	this->updateEvents();				// разовый вызов обновления событий для корректного отображения кнопок
}

GameState::~GameState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
}

void GameState::updateEvents()
{
	this->updateGUI();
	//зум камеры
	if (S::sfmlEvent.type == sf::Event::MouseWheelScrolled)
	{
		if (S::sfmlEvent.mouseWheelScroll.delta > 0)
		{
			if(S::view.getSize().x <=5000||S::view.getSize().y<=5000)
			S::view.zoom(1.1);
		}
		else if (S::sfmlEvent.mouseWheelScroll.delta < 0)
		{
			if (S::view.getSize().x >= 300 || S::view.getSize().y >= 300)
			S::view.zoom(0.9);
		}
	}
}

void GameState::update(const float& dtime)									//обновляем все
{
	this->updateMousePositions();
	this->updateFPS();
	this->updateView(dtime);
}

void GameState::render(sf::RenderTarget* target)							//рисуем все
{
	if (!target)
		target = this->window.get();
	//target->setView(S::view);
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//рисуем динамечсеие объекты
	map->render(*target);
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//рисуем статические объекты после установки дефолтной камеры (gui и тд)
	target->setView(this->window->getDefaultView());
	this->renderGUI(*target);
	target->draw(mouseText);
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//возвращаем камеру вида игры
	target->setView(S::view);
}