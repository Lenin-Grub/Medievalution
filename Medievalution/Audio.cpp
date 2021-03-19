#include "stdafx.h"
#include "Audio.h"

sf::SoundBuffer& Sounds::useSound(const std::string& sound)
{
	return this->sounds.at(sound);
}

//sf::Sound& Sounds::playSound(const std::string& sd)
//{
//	sf::Sound sound;
//	sound.setBuffer(useSound(sd));
//	sound.play();
//	return sound;
//}

void Sounds::loadSound(const std::string& name, const std::string& file)
{
	sf::SoundBuffer soundBuffer;
	soundBuffer.loadFromFile("Resourses/Audio/Sounds/" + file);
	this->sounds[name] = soundBuffer;

	return;
}

Sounds::Sounds(){}
Sounds::~Sounds(){}