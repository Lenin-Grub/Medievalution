#pragma once
#include "Common/Common.h"
#include "Camera/Camera.h"

class StateMachine;

struct StateData
{
    Camera camera;
    Jukebox jukebox = Jukebox("resources/Audio/Musics/");
};

/**
 * @class State
 * @brief Base class for individual states within the application.
 *
 * The State class provides a common interface for different states to implement.
 * It includes methods for updating, drawing, and handling various state-specific functionalities.
 */

class State
{
public:
    State(StateData &data, StateMachine &machine, sf::RenderWindow &window, bool replace = true);
    virtual ~State()                = default;
    State(const State &)            = delete;
    State &operator=(const State &) = delete;

    /// @brief Initializes the state.
    /// This method must be implemented in derived classes to initialize the state.
    virtual void init() = 0;

    /// @brief Called when the state is activated.
    /// This method must be implemented in derived classes to perform necessary actions when the state is activated.
    virtual void onActivate() = 0;

    /// @brief Called when the state is deactivated.
    /// This method must be implemented in derived classes to perform necessary actions when the state is deactivated.
    virtual void onDeactivate() = 0;

    /// @brief Updates the state's events.
    /// This method must be implemented in derived classes to update the SFML events.
    virtual void updateEvents() = 0;

    /// @brief Updates the state's ImGui interface.
    /// This method must be implemented in derived classes to update the state's ImGui interface.
    virtual void updateImGui() = 0;

    /// @brief Updates the state with respect to time.
    /// This method must be implemented in derived classes to update the state with respect to time.
    /// @param dtime Time elapsed since the last update.
    virtual void update(const float &dtime) = 0;

    /// @brief Draws the state on the target object.
    /// This method must be implemented in derived classes to draw the state on the target object.
    /// @param target Target object for drawing.
    virtual void draw(sf::RenderTarget *target = nullptr) = 0;

    /// @brief Returns a pointer to the next state.
    /// @return Unique pointer to the next state.
    std::unique_ptr<State> next();

    /// @brief Checks if the current state replaces another state.
    /// @return True if the current state replaces another state, false otherwise.
    bool isReplacing() const;

    /// @brief Updates the mouse positions for the state.
    /// @param view View object for updating mouse positions.
    void updateMousePositions(sf::View *view = nullptr);

    sf::RenderWindow &window;

protected:
    bool is_replace;
    StateData &data;
    StateMachine &state_machine;
    std::unique_ptr<State> next_state;
};
