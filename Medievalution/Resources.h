#pragma once
#include "Textures.h"
#include "Audio.h"

class Resources									// буферный класс для удобной загрузки сюда ресурсов
{
public:
	Textures textureResources;					// текстуры
	Sounds	 soundResources;					// звки

	Resources();
	virtual ~Resources();
};

