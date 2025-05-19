#pragma once
#include "Settings/WindowSettings.h"
#include "Input/Input.h"

/**
 * @class Camera
 * @brief A class for handling camera functionality within an application.
 *
 * The Camera class provides methods for updating the camera's position,
 * handling movement, zooming, and scrolling based on user input.
 */
class Camera
{
public:
    Camera();
    virtual ~Camera() = default;

    /// @brief Sets the default view.
    void setDefaulatView() noexcept;

    /// @brief Updates the camera's state based on the elapsed time.
    /// This method should be called every frame to update the camera's
    /// position and handle any other necessary updates.
    /// @param dtime The elapsed time since the last update call, in seconds.
    void update(const float& dtime) noexcept;

    /// @brief Moves the camera based on the elapsed time.
    /// This method handles the movement of the camera, typically based
    /// on user input or other factors.
    /// @param dtime The elapsed time since the last update call, in seconds.
    void move(const float& dtime) noexcept;

    ///@brief Zooms the camera in or out
    ///This method handles the zoom functionality of the camera.The
    /// zoom level is typically adjusted based on user input.
    void zoom() noexcept;

    /// @brief Scrolls the camera view.
    /// This method handles the scrolling of the camera view, typically
    /// based on user input such as mouse wheel scrolling.
    void scroll() noexcept;

    /// @brief Focuses the camera on a specific target point.
    void focusOn() noexcept;

    /// @brief Enables or disables automatic focusing of the camera.
    /// @param enable True to enable auto - focus, false to disable.
    /// @return The new state of the auto - focus flag.
    bool enableFocus(const bool enable) noexcept;

    /// @brief Stops the current camera focus animation or action.
    void stopFocus() noexcept;

    /// @brief Checks if the camera is currently moving to a focus point.
    /// @return True if the camera is moving to a focus point, false otherwise.
    const bool isMoveToFocus() const noexcept;

    /// @brief Sets the camera to focus on a given world position.
    /// @param target The target position in world coordinates.
    /// @return The new target focus position.
    sf::Vector2f setFocusOn(sf::Vector2f target) noexcept;

    /// @brief Resets the camera to its initial state.
    void reset() noexcept;

private:
    int          max_zoom;          ///< The maximum zoom level for the camera.
    int          min_zoom;          ///< The minimum zoom level for the camera.
    
    float        pan_threshold;     ///< The threshold for initiating a pan action.
    float        arrival_threshold; ///< Distance threshold for reaching the focus point.
    
    bool         is_panning;        ///< Indicates whether the camera is currently panning.
    bool         is_moved_to_focus; ///< Tracks whether the camera has reached the focus point.
    bool         auto_focusing;     ///< Indicates whether auto-focusing is enabled.
    
    sf::Vector2f prev_mouse_pos;    ///< Stores the previous mouse position for panning calculations.
    sf::Vector2f target_position;   ///< Target position the camera is moving towards.
    sf::Vector2f target_focus;      ///< Current focus point in world coordinates.
};