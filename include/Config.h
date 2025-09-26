#pragma once

// WINDOW - size by character
#define WINDOW_W		64
#define	WINDOW_H		32
#define	WINDOW_FONTSIZE	12

// PLAY CONFIG
#define TICK_FRAME		16	// tick per frame
#define SCORE_BASIC		100	// multiplier: killcount to score

// INIT COORD
#define COORD_INIT_X	WINDOW_W / 2 
#define COORD_INIT_Y	WINDOW_H / 2

// PLAYER
#define PLAYER_VX		2	// multiplier: player x move by char- higher = faster	# 2 x char == 1 y char
#define PLAYER_VY		1	// multiplier: player y move by char- higher = faster
#define PLAYER_VS		384	// normalizer: player velocity		- higher = slower

// PLAYER BULLET
#define PLAYER_B_VX		2	
#define PLAYER_B_VY		1
#define PLAYER_B_MAX	2	
#define PLAYER_B_DELAY	32

// ENEMY
#define ENEMY_COUNT		32	// total enemy count
#define ENEMY_ROW		8	// enemy per row - if too much, grapic crash
#define ENEMY_GAP		2

#define ENEMY_VE		16	// multiplier: enemy tick			- higher = slower   
#define ENEMY_AC		2	// multiplier: enemy acelleration	- higher = faster increase 

// ENEMY BULLET
#define ENEMY_B_SCALE	1	// normalizer: enemy bullet count	- higher = lower count

// KEY
#define KEY_MOVE_LEFT	'A'
#define KEY_MOVE_RIGHT	'D'
#define KEY_MOVE_UP		'W'
#define KEY_MOVE_DOWN 	'S'

#define KEY_SHOT_LEFT	VK_LEFT
#define KEY_SHOT_RIGHT	VK_RIGHT
#define KEY_SHOT_UP		VK_UP
#define KEY_SHOT_DOWN	VK_DOWN

#define KEY_EXIT		VK_ESCAPE
#define KEY_RESTART		VK_SPACE