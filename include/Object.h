#pragma once

#include <windows.h>
#include "Config.h"
#include "Asset.h"

typedef struct _KEYPROCESS KeyProcess;
typedef struct _OBJECT2D Object2D;
typedef struct _BULLET Bullet;
typedef struct _ENEMY Enemy;
typedef struct _PLAYER Player;

void init_obj(Object2D* obj, short x, short y, Sprite* sprite, Heatmap* heatmap);
int check_colide(Object2D* obj1, Object2D* obj2);
Object2D create_obj(short x, short y, Sprite* sprite, Heatmap* heatmap);

void create_enemies(void);
void update_enemies_position(void);
void update_enemies_kill_frame();
int check_bound_out(void);

void create_player(void);
void update_player_position(KeyProcess* p_key);
int update_player_bullet(KeyProcess* p_key, int cooltime);

struct _KEYPROCESS {
	int dx, dy;
	short loop, shotx, shoty;
};

struct _OBJECT2D {
	short current_frame;

	COORD pos;
	Sprite* sprite;
	Heatmap* heatmap;
}; 

struct _BULLET {
	Object2D obj;

	short shot, dx, dy;
};

struct _ENEMY {
	Object2D obj;
	Bullet bullet;

	short live;
};

struct _PLAYER {
	Object2D obj;
	Bullet bullet[PLAYER_BULLET_MAX]; 

	short live;
};

extern Enemy enemies[ENEMY_COUNT];
extern Player player;

enum _PlayerStat {
	PLAYER_CRASH=-3,
	PLAYER_SHOTDOWN,
	PLAYER_SUICIDE,
	PLAYER_MISS 
};