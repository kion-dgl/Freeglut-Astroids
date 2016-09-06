/*
    
	Gamepad Utilities
   
	Copyright (C) 2016 Benjamin Collins
    
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License version 3
    as published by the Free Software Foundation.
    
	This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.
    
	You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdbool.h>

/**
 * Define Gamepad Masks
 **/

#define GAMEPAD_BUTTON_A_MASK 		0x01
#define GAMEPAD_BUTTON_B_MASK 		0x02
#define GAMEPAD_BUTTON_X_MASK 		0x04
#define GAMEPAD_BUTTON_Y_MASK 		0x08
#define GAMEPAD_BUTTON_L_MASK 		0x10
#define GAMEPAD_BUTTON_R_MASK 		0x20
#define GAMEPAD_BUTTON_SELECT_MASK 	0x40
#define GAMEPAD_BUTTON_START_MASK 	0x80

/**
 * Define Directional Buttons
 **/

bool GAMEPAD_LEFT 				= false;
bool GAMEPAD_RIGHT 				= false;
bool GAMEPAD_UP 				= false;
bool GAMEPAD_DOWN 				= false;

/**
 * Define Pressed Gamepad Buttons
 **/

bool GAMEPAD_BUTTON_A 			= false;
bool GAMEPAD_BUTTON_B 			= false;
bool GAMEPAD_BUTTON_X 			= false;
bool GAMEPAD_BUTTON_Y 			= false;
bool GAMEPAD_BUTTON_L 			= false;
bool GAMEPAD_BUTTON_R 			= false;
bool GAMEPAD_BUTTON_SELECT 		= false;
bool GAMEPAD_BUTTON_START 		= false;

/**
 * Define Gamepad Callback function
 **/

void gamepad_callback( unsigned int btn_mask, int x, int y, int z ) {

	// Directional Buttons
	
	GAMEPAD_LEFT 	= (x < 0) ? true : false;
	GAMEPAD_RIGHT 	= (x > 0) ? true : false;
	GAMEPAD_UP 		= (y < 0) ? true : false;
	GAMEPAD_DOWN 	= (y > 0) ? true : false;
	
	// Gamepad Buttons

	GAMEPAD_BUTTON_A = btn_mask & GAMEPAD_BUTTON_A_MASK ? true : false;
	GAMEPAD_BUTTON_B = btn_mask & GAMEPAD_BUTTON_B_MASK ? true : false;
	GAMEPAD_BUTTON_X = btn_mask & GAMEPAD_BUTTON_X_MASK ? true : false;
	GAMEPAD_BUTTON_Y = btn_mask & GAMEPAD_BUTTON_Y_MASK ? true : false;
	GAMEPAD_BUTTON_L = btn_mask & GAMEPAD_BUTTON_L_MASK ? true : false;
	GAMEPAD_BUTTON_R = btn_mask & GAMEPAD_BUTTON_R_MASK ? true : false;
	GAMEPAD_BUTTON_SELECT = btn_mask & GAMEPAD_BUTTON_SELECT_MASK ? true : false;
	GAMEPAD_BUTTON_START = btn_mask & GAMEPAD_BUTTON_START_MASK ? true : false;

}

