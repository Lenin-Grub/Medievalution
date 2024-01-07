#pragma once
#include "../Settings/WindowSettings.h"

//------------------------------------------------------------------------------------------------------------------------
// класс камера отвечает за передвижение и скролинг камеры.
// необходима для рендера статичных и динамичных элементов, таких как GUI
class Camera
{
public:
	Camera();
	virtual ~Camera();

	void update(const float& dtime);
	void move(const float& dtime);
	void zoom();

private:
	int max_zoom = 1500;
	int min_zoom = 90;
};