#pragma once
#include "../Settings/WindowSettings.h"
#include "../Input/Input.h"

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
	virtual ~Camera();

/// @brief Updates the camera's state based on the elapsed time.
/// This method should be called every frame to update the camera's
/// position and handle any other necessary updates.
/// @param dtime The elapsed time since the last update call, in seconds.
	void update(const float& dtime);

/// @brief Moves the camera based on the elapsed time.
/// This method handles the movement of the camera, typically based
/// on user input or other factors.
/// @param dtime The elapsed time since the last update call, in seconds.
	void move(const float& dtime);

///@brief Zooms the camera in or out
///This method handles the zoom functionality of the camera.The
/// zoom level is typically adjusted based on user input.
	void zoom() const;

/// @brief Scrolls the camera view.
/// This method handles the scrolling of the camera view, typically
/// based on user input such as mouse wheel scrolling.
	void scroll();

private:
	int max_zoom        = 1500;         ///< The maximum zoom level for the camera.
	int min_zoom        = 90;           ///< The minimum zoom level for the camera.
	float pan_threshold = 5.0f;         ///< The threshold for initiating a pan action.
	bool is_panning     = false;        ///< Indicates whether the camera is currently panning.
	sf::Vector2f prev_mouse_pos;        ///< Stores the previous mouse position for panning calculations.
};