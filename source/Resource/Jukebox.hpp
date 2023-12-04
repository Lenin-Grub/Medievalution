#pragma once
#include "../stdafx.h"
#include "Files.hpp"

class Jukebox
{
public:
	Jukebox();
	~Jukebox();

	void next();
	void prev();
	void stop();
	void play();
	void repeat();
	void toggle();

	void setVolume();
	void getVolume();

	sf::Music music;
private:

};