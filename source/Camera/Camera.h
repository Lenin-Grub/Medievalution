﻿#pragma once
#include "../Settings/WindowSettings.h"

//------------------------------------------------------------------------------------------------------------------------
// класс камера отвечает за передвижение и скролинг камеры.
// необходима для рендера статичных и динамичных елементов, таких как GUI
class Camera
{
public:
	void initView();												// инициализация камеры
	void updateView(const float& dtime);							// обновление камеры

	void move(const float& dtime);									// движение камеры
	void zoom();													// прокрутка камеры(приближение\удаление)
private:
	//WindowSettings settings;
	int max_zoom = 1500;
	int min_zoom = 100;
};
//------------------------------------------------------------------------------------------------------------------------