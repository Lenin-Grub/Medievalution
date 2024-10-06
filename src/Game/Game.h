#pragma once
#include "../State/IntroState.hpp"
#include "../State/StateMachine.hpp"
#include "../Settings/WindowSettings.h"

/**
 * @class Game
 * @brief A class for managing the main game loop and initialization processes.
 *
 * The Game class handles the initialization of various graphical settings, fonts, window creation,
 * and the main game loop. It also manages the application state through an instance of the StateMachine class.
 */
class Game
{
public:
    Game();
    virtual ~Game() = default;

    /// @brief Starts the main game loop
    /// The run method contains the main loop that keeps the game running
    /// until the window is closed.
    void run();

private:
    /// @brief Initializes the graphical settings for the game
    /// This method sets up various graphical settings necessary for the game
    /// @return True if the graphical settings were successfully initialized; false otherwise.
    bool initGraphicSettings() noexcept;

    /// @brief Initializes the game window
    /// This method creates and initializes the main window for the game
    /// @return True if the window was successfully initialized; false otherwise.
    bool initWindow() noexcept;

    /// @brief Initializes the application icon
    /// This method sets the icon for the application window
    /// @return True if the icon was successfully initialized; false otherwise.
    bool initIcon() noexcept;
    
    /// @brief Load and set cursor
    void initCursor();

    /// @brief Initializes the fonts used in the game
    /// This method loads and initializes the fonts used throughout the game
    /// @return True if the fonts were successfully initialized; false otherwise.
    bool initFonts() noexcept;

    /// @brief Initializes the jukebox for the game
    /// This method initializes the audio system for the game, setting up music and sound effects
    /// @return True if the jukebox was successfully initialized; false otherwise.
    bool initJukebox() noexcept;

    /// @brief Restarts the application
    /// This method handles the process of restarting the application.
    void restartApplication();

private:
    std::shared_ptr<sf::RenderWindow> window; ///< A shared pointer to the main render window for the game.
    StateMachine state_machine;               ///< Manages the state of the game (e.g., menus, game play).
    sf::Cursor cursor;                        ///< Mouse cursor.
    sf::Image im_cursor;                      ///< Image to the mouse cursor.
};