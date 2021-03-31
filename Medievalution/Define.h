#pragma once
#include "stdafx.h"
// îò

#define PI  3.141590					
#define DEG 57.29582
#define RAD 0.017453

class System;
typedef System S;

using std::cout;
using std::endl;
using std::to_string;
using std::cos;
using std::cosf;
using std::sin;
using std::sinf;
using std::atanf;
using std::make_shared;
using std::make_unique;
using std::list;
using std::vector;
using std::string;
using std::unique_ptr;
using std::shared_ptr;
using std::ifstream;
using std::ofstream;
using std::thread;

typedef unsigned int uint;					// размер
typedef sf::Vector2i v2i;					// v2i
typedef sf::Vector2f v2f;					// v2f
typedef sf::Vector2u v2u;					// v2u
typedef sf::Keyboard Key;					// клавиша
typedef sf::RectangleShape Shape;			// фигура
typedef sf::Texture Img;					// изображение
typedef sf::Color Color;					// цвет
typedef sf::Sound Sound;					// звук