#pragma once

#include "Console.h"
#include "Asset.h"

typedef struct _OBJECT2D Object2D;
typedef struct _BULLET Bullet;
typedef struct _ENEMY Enemy;
typedef struct _PLAYER Player;

// global variables
extern Enemy enemies[ENEMY_COUNT];
extern Player player;

// Object.c
Object2D create_obj(short x, short y, Sprite* sprite, Heatmap* heatmap);
void init_obj(Object2D* obj, short x, short y, Sprite* sprite, Heatmap* heatmap);
int check_colide(Object2D* obj1, Object2D* obj2);
int check_colide_all(int* score);
void draw_obj(DoubleBuffer* p_dbuf, Object2D* obj);

// Enemy.c
void create_enemies(void);
void update_enemies_position(void);
void update_enemies_kill_frame();
int check_bound_out(void);

// Player.c
void create_player(void);
void update_player_position(KeyProcess* p_kps);
int update_player_bullet(KeyProcess* p_kps, int cooltime);
void draw_player_explode(DoubleBuffer* p_dbuf, TickCounter* p_counter);

// types
struct _OBJECT2D {
	short current_frame;

	Pos pos;
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
	Bullet bullet[PLAYER_B_MAX]; 

	short live;
};

// alias
enum _PlayerStat {
	PLAYER_CRASH = -3,
	PLAYER_SHOTDOWN,
	PLAYER_SUICIDE,
	PLAYER_MISS, 
	PLAYER_LIVE = 1
};