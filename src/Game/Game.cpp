#include "../stdafx.h"
#include "Game.h"

Game::Game()
{
	core::dtime = 0.00f;
}

void Game::run()
{
	initGraphicSettings();
	initWindow();
	initIcon();	
	initFonts();
	initJukebox();

	state_machine.run(StateMachine::build<IntroState>(state_machine.data, state_machine, *window, true));

	while (state_machine.running())
	{
		if (state_machine.resturting())
		{
			restartApplication();
		}

		state_machine.nextState();
		state_machine.update();
		state_machine.draw();
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
	if (WindowSettings::getInstance().fullscreen)
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
	window->setVerticalSyncEnabled	(WindowSettings::getInstance().vertical_sync);
	ImGui::SFML::Init(*window);

	return true;
}

bool Game::initIcon() noexcept
{
	sf::Image icon;
	if (icon.loadFromFile("resources/Icons/icon.png"))
	{
		this->window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
		return true;
	}
	LOG_ERROR("File <<icon.png>> not loadded");
	return false;
}

bool Game::initFonts() noexcept
{
	ImFontConfig config;
	config.MergeMode           = true;
	config.PixelSnapH          = true;
	config.GlyphMinAdvanceX    = 6.0f;
	config.OversampleH         = 3;
	config.OversampleV         = 3;
	config.GlyphExtraSpacing.x = 10.0f;
	config.GlyphExtraSpacing.y = 10.0f;

	static const ImWchar icon_ranges[] = { ICON_MIN, ICON_MAX, 0 };

	ImGuiIO& io = ImGui::GetIO();
	ImFont* font1 = io.Fonts->AddFontFromFileTTF("resources/Fonts/OpenSans-Semibold.ttf", 20.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
	ImFont* font2 = io.Fonts->AddFontFromFileTTF("resources/Fonts/OpenFontIcons.ttf", 20.f, &config, icon_ranges);
	if(io.Fonts->Build())

	{
		ImGui::SFML::UpdateFontTexture();
		io.FontDefault = font1;
	}

	return false;
}

bool Game::initJukebox() noexcept
{
	state_machine.data.jukebox.requestAll();
	state_machine.data.jukebox.play();
	state_machine.data.jukebox.setVolume(WindowSettings::getInstance().music_volume);
	return false;
}

void Game::restartApplication()
{
	state_machine.restart(false);
	window->close();
	ImGui::SFML::Shutdown();

	initGraphicSettings();
	initWindow();
	initIcon();
	initFonts();
	initJukebox();

	state_machine.run(StateMachine::build<IntroState>(state_machine.data, state_machine, *window, true));
	LOG_INFO("Application\t Restarted");
}