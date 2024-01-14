#pragma once
#include "../Settings/WindowSettings.h"
#include "../Input/Input.h"

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
	void scroll();

private:
	int max_zoom = 1500;
	int min_zoom = 90;
	sf::Vector2f prevMousePos;
	bool isPanning = false;
	float panThreshold = 5.0f;
	//Input& input;
};