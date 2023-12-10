#include "Jukebox.hpp"

Jukebox::Jukebox()
{
	core::music = std::make_unique<sf::Music>();
}

Jukebox::~Jukebox()
{
	core::music->stop();
}

void Jukebox::next()
{
	if (core::music->openFromFile("resources/Audio/Muisics/Music2.wav"))
	{
		LOG_INFO("Music track 2 loaded sucsesfully");
	}
}

void Jukebox::prev()
{
	if (core::music->openFromFile("resources/Audio/Muisics/Now-We-Ride.wav"))
	{
		LOG_INFO("Music track 1 loaded sucsesfully");
	}
}

void Jukebox::stop()
{
	core::music->stop();
	LOG_INFO("Music stoped");
}

void Jukebox::play()
{
	core::music->play();
	LOG_INFO("Music play");
}

void Jukebox::repeat()
{
	core::music->setLoop(true);
}

void Jukebox::toggle()
{
	core::music->setPlayingOffset(sf::seconds(150.f));
}

void Jukebox::setVolume(float volume)
{
	core::music->setVolume(volume);
	LOG_INFO("Music volume ", volume);
}

const float Jukebox::getVolume() const
{
	return core::music->getVolume();
}
