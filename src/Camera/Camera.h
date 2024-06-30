#pragma once
#include "../Settings/WindowSettings.h"
#include "../Input/Input.h"

//------------------------------------------------------------------------------------------------------------------------
// класс камера отвечает за передвижение и скролинг камеры.
// необходима для рендера статичных и динамичных элементов, таких как GUI
class Camera
{
public:
	Camera			();
	virtual ~Camera ();

	void    update  (const float& dtime);
	void    move	(const   float& dtime);
	void    zoom	();
	void    scroll	();

private:
	int   max_zoom;
	int   min_zoom;
	float pan_threshold;
	bool  is_panning;

	sf::Vector2f prev_mouse_pos;
};