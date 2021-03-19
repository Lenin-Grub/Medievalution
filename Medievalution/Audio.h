#pragma once
#include "Define.h"


class Sounds
{
public:
	Sounds();
	virtual ~Sounds();

	sf::SoundBuffer& useSound(const std::string& sound);
	//sf::Sound& playSound(const std::string& sd);

	void loadSound(const std::string& name, const std::string& file);
private:
	std::map<std::string, sf::SoundBuffer> sounds;
};