#pragma once
#include "Common/Common.h"

struct Frame
{
    sf::IntRect frame_rect;
    sf::Vector2i size;
};

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
    Animator(sf::Sprite& sprite);
    virtual ~Animator() = default;

    /// @brief Initializes the object with a given name.
    /// @param name The name used to initialize the object.
    /// @return bool Returns true if initialization is successful, false otherwise.
    bool init(const std::string& name);

    /// @brief Adds a frame to the animation sequence
    /// @param rect The rectangle defining the frame within the sprite's texture.
    void addFrame(sf::IntRect rect);

    /// @brief Removes a frame with the specified ID.
    /// @param id The ID of the frame to be removed.
    void removeFrame(const int& id);

    /// @brief Removes all frames from all animations.
    /// This method clears all frames from all animations, effectively resetting them.
    void removeAllFrames();

    /// @brief Adds a new animation with the specified name.
    /// @param name The name of the new animation to be added.
    void addAnimation(const std::string& name);

    /// @brief Removes the animation with the specified name.
    /// @param name The name of the animation to be removed.
    void removeAnimation(const std::string& name);

    /// @brief Retrieves the frames of the animation with the specified name.
    /// @param name The name of the animation to retrieve.
    /// @return std::vector<sf::IntRect> A vector containing the frame rectangles of the animation.
    std::vector<sf::IntRect> findAnimation(const std::string& name) const;

    /// @brief Returns a list of animation names.
    /// @return A vector containing the names of all animations.
    std::vector<std::string> getAnimationNames() const;

    void flipFramesHorizontally();

    /// @brief Sets the display time for each frame
    /// @param time The time to display each frame, in seconds.
    void setFrameTime(float time);

    /// @brief Sets the current frame of the animation
    /// @param frame The index of the frame to set as the current frame.
    void setCurrentFrame(int frame);

    /// @brief Updates the animation based on the elapsed time
    /// This method should be called every frame to update the animation's state.
    /// @param deltaTime The elapsed time since the last update call, in seconds.
    void update(float deltaTime);

    /// @brief Starts or resumes the animation.
    /// @param play If true, the animation will start or resume playing.
    void play(bool play);

    /// @brief Pauses the animation.
    void pause();

    /// @brief Checks if the animation is currently playing
    /// @return True if the animation is being played, false otherwise.
    const bool isPlayed() const;

    /// @brief Gets the current frame index
    /// @return The index of the current frame.
    const int getCurrentFrame() const;

    /// @brief Retrieves the index of the next frame in the animation sequence.
    /// @return const int The index of the next frame.
    const int getNextFrame() const;

    /// @brief Retrieves the index of the previous frame in the animation sequence.
    /// @return int The index of the previous frame.
    int getPrevFrame() const;

    /// @brief Retrieves the index of the last frame in the animation sequence.
    /// @return const int The index of the last frame.
    const int getLastFrame();

    /// @brief Retrieves the index of the first frame in the animation sequence.
    /// @return const int The index of the first frame.
    const int getFirstFrame();

    /// @brief Retrieves the size of the current frame.
    /// @return sf::Vector2i A vector containing the width and height of the current frame.
    sf::Vector2i getFrameSize() const;
    
    /// @brief Sets the size of the current frame.
    /// @param size A vector containing the new width and height for the frame.
    void setFrameSize(const sf::Vector2i size);

    ///@brief Gets the current time elapsed for the current frame
    ///@return The time elapsed for the current frame, in seconds.
    const float getCurrentTime() const;

    /// @brief Gets the display time for each frame
    /// @return The display time for each frame, in seconds.
    const float getFrameTime() const;

    /// @brief Gets the frames of the animation
    /// @return A vector of rectangles defining the frames of the animation.
    const std::vector<sf::IntRect>& getFrames() const;

    /// @brief Retrieves the current frame rectangle of the sprite or animation.
    /// @return const sf::IntRect& A constant reference to the current frame rectangle.
    const sf::IntRect& getCurrentFrameRect() const;

    /// @brief Returns a map of animations.
    /// @return A map containing animation names and their corresponding vectors of frames.
    std::map<std::string, std::vector<sf::IntRect>> getAnimations() const;

    /// @brief Saves an animation to a json file.
    /// @param path The path to the file where the animation will be saved.
    /// @param name The file name of the animation to be saved.
    /// @return True if the animation was successfully saved, false otherwise.
    bool saveAnimation(const std::string& path, const std::string& name) const;

    /// @brief Loads all animations from a json file.
    /// @param path The path to the file containing the animations.
    /// @return True if the animations were successfully loaded, false otherwise.
    bool loadAllAnimations(const std::string& path);

private:
    unsigned int             current_frame;       ///< The index of the current frame.
    bool                     played;              ///< Flag indicating whether the animation is playing.
    float                    current_time = 0.0f; ///< The current time elapsed for the current frame.
    float                    frame_time;          ///< The display time for each frame, in seconds.
    sf::Sprite&              sprite;              ///< The sprite being animated.
    sf::Vector2i             frame_size;          ///< The size of frame.
    std::vector<sf::IntRect> frames;              ///< The frames of the animation.
    std::map<std::string, std::vector<sf::IntRect>> animations;
};