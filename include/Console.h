#pragma once

#include <stdio.h>
#include <windows.h>
#include "Object.h"

typedef struct _TICKMANAGER TickManager;
typedef struct _DOUBLEBUFFER DoubleBuffer;

void init_tick(TickManager* p_counter, short player_period, short enemy_period, short shot_tick);
void tick(TickManager* p_counter);
int is_player_tick(TickManager* p_counter);
int is_enemy_tick(TickManager* p_counter);
int is_shot_tick(TickManager* p_counter);
int reset_shot(TickManager* p_counter);

void init_dbuffer(DoubleBuffer* p_dbuf, short width, short height);
void free_dbuffer(DoubleBuffer* p_dbuf);

void clear_back_buffer(DoubleBuffer* p_dbuf);
void draw_back_buffer(DoubleBuffer* p_dbuf);
HANDLE get_back_buffer(DoubleBuffer* p_dbuf);

void draw_2d(DoubleBuffer* p_dbuf, Object2D* obj);
void draw_fstring_at(DoubleBuffer* p_dbuf, short x, short y, const char* fmt, ...);
void draw_fstring_center(DoubleBuffer* p_dbuf, short y, const char* fmt, ...);

void update_key(KeyProcess* p_kps);
void sleep(int ms);

struct _DOUBLEBUFFER {
	HANDLE hbuf[2];
	int idx;
	COORD size;
};

struct _TICKMANAGER {
	short		player_period;
	short		enemy_period;
	short		shot_period;
	ULONGLONG   tick;
	ULONGLONG   player_tick;
	ULONGLONG   enemy_tick;
	ULONGLONG	shot_tick;
};