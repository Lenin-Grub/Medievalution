#pragma once
#include "Define.h"

//class Musics
//{
//};

class Sounds
{
private:
	//загружаем звуки
	sf::SoundBuffer loadSound(const std::string file)
	{
		sf::SoundBuffer buffer;
		buffer.loadFromFile("Resourses/Audio/Sounds/" + file);
		return buffer;
	}
	sf::Sound sounds;
public:

	vector <sf::SoundBuffer> soundBufferGUI;							// GUI звки
	Sounds()
	{
		soundBufferGUI.push_back(loadSound("button_click_sound.wav"));	//0
		soundBufferGUI.push_back(loadSound("exit_sound.wav"));			//1
		soundBufferGUI.push_back(loadSound("dig_sound.wav"));			//2
		soundBufferGUI.push_back(loadSound("destroy_sound.wav"));		//3
	}
	virtual ~Sounds() {};

	sf::Sound setSounds(int i)
	{
		sounds.setBuffer(soundBufferGUI[i]);
		return sounds;
	}
	
	sf::Sound playSound()
	{
		sounds.play();
		return sounds;
	}
};

class Audio : Sounds
{
public:
	//Musics musics;
	Sounds sounds;
	Audio()	
	{
	};
	~Audio() {};
};
