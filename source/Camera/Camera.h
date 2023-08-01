#pragma once
#include "../Settings/WindowSettings.h"

//------------------------------------------------------------------------------------------------------------------------
// класс камера отвечает за передвижение и скролинг камеры.
// необходима для рендера статичных и динамичных елементов, таких как GUI
class Camera
{
public:
	void initView();					
	void updateView(const float& dtime);

	void move(const float& dtime);		
	void zoom();						
private:
	int max_zoom = 1500;
	int min_zoom = 100;
};