#pragma once

#include <stdio.h>
#include <windows.h>
#include "Config.h"

//alias
typedef COORD Pos;

//types
typedef struct _DOUBLEBUFFER DoubleBuffer;
typedef struct _TICKCOUNTER TickCounter;
typedef struct _KEYPROCESS KeyProcess;

//dbuf func
void init_dbuffer(DoubleBuffer* p_dbuf, short width, short height);
void free_dbuffer(DoubleBuffer* p_dbuf);
void draw_back_buffer(DoubleBuffer* p_dbuf);
void draw_fstring_at(DoubleBuffer* p_dbuf, short x, short y, const char* fmt, ...);
void draw_fstring_center(DoubleBuffer* p_dbuf, short y, const char* fmt, ...);
void draw_2d(DoubleBuffer* p_dbuf,
	char* data, short W, short H, short frame, short draw_x, short draw_y,
	short start_x, short start_y, short end_x, short end_y
);

//tick func
void init_tick(TickCounter* p_counter, short player_period, short enemy_period, short shot_tick);
void tick(TickCounter* p_counter);
int is_player_tick(TickCounter* p_counter);
int is_enemy_tick(TickCounter* p_counter);
int is_shot_tick(TickCounter* p_counter);
int reset_tick(TickCounter* p_counter, ULONGLONG* tick);

//kps func
void init_kps(KeyProcess* p_kps);
void update_kps(KeyProcess* p_kps);
int wait_kps(KeyProcess* p_kps);

//etc
void sleep_ms(int ms);

//types
struct _DOUBLEBUFFER {
	HANDLE hbuf[2];
	int idx;
	COORD size;
};

struct _TICKCOUNTER {
	short		player_period;
	short		enemy_period;
	short		shot_period;
	ULONGLONG   tick;
	ULONGLONG   player_tick;
	ULONGLONG   enemy_tick;
	ULONGLONG	shot_tick;
};

struct _KEYPROCESS {
	int dx, dy;
	short loop, shotx, shoty;
};

enum _LOOP_STATE {
	LOOP_CONTINUE = -1, 
	LOOP_END,
	LOOP_RESTART,
};