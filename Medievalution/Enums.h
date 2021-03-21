#pragma once

enum widget_states				// состояния виджетов
{
	WIDGET_IDLE = 0,			// кнопка без действия	"бездельник"
	WIDGET_ACTIVE,				// нажатая кнопка		"нажато"
	WIDGET_HOVER,				// мышь над кнопкой		"готово к нажатию"
	WIDGET_DISABLED,			// мышь не активна		"не доступна для нажатия"
};

enum TILE_TYPE			// тип тайла
{
	TILE_NONE = 0,		// ничего
	TILE_WATER,			// вода
	TILE_SAND,			// песок
	TILE_GROUND,		// земля
	TILE_GRASS,			// трава
	TILE_ROCK,			// cкала
};