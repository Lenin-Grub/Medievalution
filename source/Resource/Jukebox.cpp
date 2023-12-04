#include "Jukebox.hpp"

Jukebox::Jukebox()
{
	core::music = std::make_unique<sf::Music>();

	if (core::music->openFromFile("resources/Audio/Muisics/Now-We-Ride.wav"))
	{
		LOG_INFO("Music track loaded sucsesfully");
	}

}

Jukebox::~Jukebox()
{
	music.stop();

}

void Jukebox::next()
{
	music.stop();
	music.getStatus() == sf::SoundSource::Status::Stopped;
	if (music.getStatus() == sf::SoundSource::Status::Stopped)
	{
		music.openFromFile("resources/Audio/Muisics/Music2.wav");
		music.play();
	}
}

void Jukebox::play()
{
	core::music->play();
}

void Jukebox::repeat()
{
	core::music->setLoop(true);
}

void Jukebox::toggle()
{
	music.setPlayingOffset(sf::seconds(150.f));
}
