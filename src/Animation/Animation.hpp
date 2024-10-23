#pragma once
#include "Common/Common.h"

/**
 * @class Animator
 * @brief A class for handling sprite animation within an application.
 *
 * The Animator class provides methods to manage frames of animation for a given sprite.
 * It allows for adding frames, setting frame time, updating animations, and controlling playback.
 */
class Animator 
{
public:
    Animator (sf::Sprite& sprite);
    virtual ~Animator () = default;

/// @brief Adds a frame to the animation sequence
/// @param rect The rectangle defining the frame within the sprite's texture.
    void addFrame (sf::IntRect rect);

/// @brief Sets the display time for each frame
/// @param time The time to display each frame, in seconds.
    void setFrameTime (float time);

/// @brief Sets the current frame of the animation
/// @param frame The index of the frame to set as the current frame.
    void setCurrentFrame(int frame);

/// @brief Updates the animation based on the elapsed time
/// This method should be called every frame to update the animation's state.
/// @param deltaTime The elapsed time since the last update call, in seconds.
    void update (float deltaTime);

/// @brief Starts or resumes the animation.
/// @param play If true, the animation will start or resume playing.
    void play (bool play);

/// @brief Pauses the animation.
    void pause ();

/// @brief Checks if the animation is currently playing
/// @return True if the animation is being played, false otherwise.
    const bool isPlayed() const;

/// @brief Gets the current frame index
/// @return The index of the current frame.
    const int getCurrentFrame() const;

///@brief Gets the current time elapsed for the current frame
///@return The time elapsed for the current frame, in seconds.
    const float getCurrentTime() const;

/// @brief Gets the display time for each frame
/// @return The display time for each frame, in seconds.
    const float getFrameTime() const;

/// @brief Gets the frames of the animation
/// @return A vector of rectangles defining the frames of the animation.
    const std::vector<sf::IntRect> getFrames() const;

private:
    int                      current_frame = 0;    ///< The index of the current frame.
    bool                     played        = true; ///< Flag indicating whether the animation is playing.
    float                    current_time  = 0.0f; ///< The current time elapsed for the current frame.
    float                    frame_time;           ///< The display time for each frame, in seconds.
    sf::Sprite&              sprite;               ///< The sprite being animated.
    std::vector<sf::IntRect> frames;               ///< The frames of the animation.
};