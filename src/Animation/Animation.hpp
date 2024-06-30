#pragma once
#include "../Core/Core.h"

class Animator 
{
public:
	 Animator                       (sf::Sprite& sprite);
	 virtual ~Animator              ();

	 void                           addFrame (sf::IntRect rect);
	 void                           setFrameTime (float time);
	 void                           setCurrentFrame(int frame);
	 void                           update (float deltaTime);
	 void                           play (bool play);
	 void                           pause ();
	 const bool                     isPlayed       () const;
	 const int                      getCurrentFrame() const;
	 const float                    getCurrentTime () const;
	 const float                    getFrameTime   () const;
	 const std::vector<sf::IntRect> getFrames      () const;

private:

	bool  played;
	int   current_frame;
	float current_time;
	float frame_time;

	sf::Sprite&              sprite;
	std::vector<sf::IntRect> frames;
};