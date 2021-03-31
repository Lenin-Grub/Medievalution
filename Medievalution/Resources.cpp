#include "stdafx.h"
#include "Resources.h"
// îò
Resources::Resources()
{
	// textureResources.loadTexture("NAME",	"Icons/----");

	//иконки
	textureResources.loadTexture("DOOR_EXIT",	"Icons/exit-door.png");						// иконка выхода
	textureResources.loadTexture("DOOR_ENTER",	"Icons/entry-door.png");					// иконка входа
	textureResources.loadTexture("DIG_DUG",		"Icons/dig-dug.png");						// копать
	textureResources.loadTexture("MINING",		"Icons/mining.png");						// шахта
	textureResources.loadTexture("THUMB_DOWN",	"Icons/thumb-down.png");					// палец вниз
	textureResources.loadTexture("THUMB_UP",	"Icons/thumb-up.png");						// палец вверх
	textureResources.loadTexture("PAUSE",		"Icons/pause-button.png");					// пауза
	textureResources.loadTexture("PLAY",		"Icons/play-button.png");					// продолжить
	
	// textureResources.loadTexture("NAME",	"Tiles/----");
	//тайлы		 
	textureResources.loadTexture("WATER",		"Tiles/terrain0.png");						// вода
	textureResources.loadTexture("SAND",		"Tiles/terrain1.png");						// песок
	textureResources.loadTexture("DIRT",		"Tiles/terrain2.png");						// грязь
	textureResources.loadTexture("GRASS",		"Tiles/terrain3.png");						// трава
	textureResources.loadTexture("ROCK",		"Tiles/terrain4.png");						// скала

	/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
	
	// soundResources.loadTexture("NAME",	"-----.wav");
	// звуки

	soundResources.loadSound("SOUND_CLICK",		"button_click_sound.wav");					// звук клик
	soundResources.loadSound("SOUND_DESTROY",	"destroy_sound.wav");						// звук разрушения
	soundResources.loadSound("SOUND_DIG",		"dig_sound.wav");							// звук копания
	soundResources.loadSound("SOUND_EXIT",		"exit_sound.wav");							// звук закрытия двери
}				 

Resources::~Resources()
{
}
