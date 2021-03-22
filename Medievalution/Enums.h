#pragma once

enum widget_states				// состояния виджетов
{
	WIDGET_IDLE = 0,			// кнопка без действия	"бездельник"
	WIDGET_ACTIVE,				// нажатая кнопка		"нажато"
	WIDGET_HOVER,				// мышь над кнопкой		"готово к нажатию"
	WIDGET_DISABLED,			// мышь не активна		"не доступна для нажатия"
};

enum tile_type					// тип тайла
{
	TILE_NONE	= 0,				// пустой тайл
	TILE_WATER	= 1,					
	TILE_SAND	= 2,					
	TILE_GROUND	= 3,				
	TILE_GRASS	= 4,					
	TILE_ROCK	= 5,					
};

enum connection : unsigned int	// соседи тайлов
{
	center		= 0,
	north		= 2,
	south		= 4,
	west		= 6,
	east		= 8,
	north_west	= 10,
	north_east	= 12,
	sout_west	= 14,
	sout_east	= 16
};