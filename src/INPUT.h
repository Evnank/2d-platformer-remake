#pragma once

#include "GLOBALS.h"

#include <SFML/Graphics.hpp>

struct INPUT{
	sf::Vector2f mouse_true_coords;
	sf::Vector2f mouse_window_coords;
	bool Mouse1,Mouse2;

	bool SPACE;

	bool F,R,M;

	bool ESCAPE,LSHIFT,TAB,ENTER,PageUp;

	bool W,A,S,D;

	bool left,right,up,down;

	int mouse_wheel_movement=0;
	
	bool F1,F2,F9,F11;

	bool player1_left;
	bool player1_right;
	bool player1_jump;

	bool player2_left;
	bool player2_right;
	bool player2_jump;

	void read(sf::RenderWindow& window);
};