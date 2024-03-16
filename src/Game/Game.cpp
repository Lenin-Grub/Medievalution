#include "../stdafx.h"
#include "Game.h"

Game::Game()
{
	core::dtime = 0.00f;
}

Game::~Game() 
{
}

void Game::run()
{
	initGraphicSettings();
	initWindow();
	initIcon();	
	initFonts();
	initJukebox();

	std::shared_ptr<IntroState>		intro_state		= std::make_shared<IntroState>		(state_manager.state_data, state_manager);
	std::shared_ptr<MainMenuState>	main_menu_state = std::make_shared<MainMenuState>	(state_manager.state_data, state_manager);

	state_manager.add(intro_state);
	state_manager.add(main_menu_state);

	state_manager.switchTo(States::State_Intro);

	while (window->isOpen())
	{
		update();
		render();
	}
	ImGui::SFML::Shutdown();
}

bool Game::initGraphicSettings() noexcept
{
	if (WindowSettings::getInstance().loadFromFIle("config/settings.json"))
	{
		return true;
	}
	LOG_ERROR("Settings.json not found!");

	return false;
}

bool Game::initWindow() noexcept
{
	if (WindowSettings::getInstance().fullscrean)
		window = std::make_unique<sf::RenderWindow>(
		 WindowSettings::getInstance().resolution,
		 WindowSettings::getInstance().title, sf::Style::Fullscreen,
		 WindowSettings::getInstance().context_settings);
	else
		window = std::make_unique<sf::RenderWindow>(
		 WindowSettings::getInstance().resolution,
		 WindowSettings::getInstance().title, sf::Style::Close,
		 WindowSettings::getInstance().context_settings);

	window->setFramerateLimit		(WindowSettings::getInstance().fps_limit);
	window->setVerticalSyncEnabled	(WindowSettings::getInstance().vertycal_sync);
	ImGui::SFML::Init(*window);

	return true;
}

bool Game::initIcon() noexcept
{
	sf::Image i;
	if (i.loadFromFile("resources/Icons/icon.png"))
	{
		this->window->setIcon(i.getSize().x, i.getSize().y, i.getPixelsPtr());
		return true;
	}
	LOG_ERROR("File <<icon.png>> not loadded");
	return false;
}

void Game::initFonts() noexcept
{
	//TODO перенсти в отдельное место
	//установка дефолтного шрифта в ImGui
	ImFontConfig config;
	config.MergeMode = true;
	config.PixelSnapH = true;
	config.GlyphMinAdvanceX = 6.0f;
	config.OversampleH = 3;
	config.OversampleV = 3;
	config.GlyphExtraSpacing.x = 10.0f;
	config.GlyphExtraSpacing.y = 10.0f;

	static const ImWchar icon_ranges[] = { ICON_MIN, ICON_MAX, 0 };

	ImGuiIO& io = ImGui::GetIO();
	ImFont* font1 = io.Fonts->AddFontFromFileTTF("resources/Fonts/OpenSans-Semibold.ttf", 20.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	ImFont* font2 = io.Fonts->AddFontFromFileTTF("resources/Fonts/OpenFontIcons.ttf", 20.f, &config, icon_ranges);
	io.Fonts->Build();

	ImGui::SFML::UpdateFontTexture();
	io.FontDefault = font1;
}

bool Game::initJukebox() noexcept
{
	state_manager.state_data.jukebox.requestAll();
	state_manager.state_data.jukebox.setVolume(WindowSettings::getInstance().music_volume);
	state_manager.state_data.jukebox.play();
	return false;
}

void Game::updateSFMLevents()
{
	while (window->pollEvent(core::sfml_event))
	{


		if (core::sfml_event.type == sf::Event::Closed)
		{
			window->close();
		}
		if (!state_manager.states.empty())
		{
			ImGui::SFML::ProcessEvent(*window, core::sfml_event);
			state_manager.processEvent();
		}
	}
}

void Game::update()
{
	core::dtime = this->clock.restart().asMilliseconds();

	updateSFMLevents();
	ImGui::SFML::Update(*window, core::clock.restart());

	if (true)
	{
		state_manager.update(core::dtime);
		state_manager.updateImGui();
	}
	else
	{
		window->close();
	}
}

void Game::render()
{
	window->clear(sf::Color(63, 72, 204));
	state_manager.draw(*window);
	ImGui::SFML::Render(*window);
	window->display();
}