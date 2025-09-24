#pragma once

typedef struct _Sprite Sprite;
typedef struct _Heatmap Heatmap;
typedef enum _SpriteID SpriteId;

const Sprite* get_sprite(SpriteId id);
const Heatmap* get_heatmap(SpriteId id);

struct _Sprite {
	short frame, h, w ;
	const char* data;
};

struct _Heatmap {
	short h, w;
	const char* data;
};

enum _SpriteId {
	PLAYER,
	ENEMY,  
	PLAYER_BULLET,
	ENEMY_BULLET,
};

enum _PlayerSptiteId {
	PLAYER_STOP,
	PLAYER_UP,
	PLAYER_DOWN,
	PLAYER_LEFT,
	PLAYER_RIGHT,
	PLAYER_EXP_1,
	PLAYER_EXP_2,
	PLAYER_EXP_3,
	PLAYER_EXP_4,
	PLAYER_EXP_5,
	PLAYER_EXP_6,
	PLAYER_EXP_7,
	PLAYER_EXP_END,
};

enum _PlayerBulletId {
	BULLET_Y, 
	BULLET_X, 
	BULLET_D
};

