#pragma warning(disable : 4996)         
#include "Main.h"

TickManager counter;
DoubleBuffer dbuf;
KeyProcess key = { .loop = -1, .dx = 0, .dy = 0, .shotx = 0, .shoty = 0 };

static int score = 0;

static int play(int score_old);
static int wait_key();

static void draw_all();
static void draw_player_explode();
static void reset_play();

int main()
{
	int loop = 1;

	init_dbuffer(&dbuf, WINDOW_W, WINDOW_H);
	
	// title screen
	draw_fstring_center(&dbuf, WINDOW_H / 2 - 8, "########  ###   ##    ######    #######   ########  ####### ");
	draw_fstring_center(&dbuf, WINDOW_H / 2 - 7, "   ##     ####  ##   ##    ##   ##    ##  ##        ##    ##");
	draw_fstring_center(&dbuf, WINDOW_H / 2 - 6, "   ##     ## ## ##   ########   ##    ##  ########  ####### ");
	draw_fstring_center(&dbuf, WINDOW_H / 2 - 5, "   ##     ##  ####   ##    ##   ##    ##  ##        ##    ##");
	draw_fstring_center(&dbuf, WINDOW_H / 2 - 4, "########  ##   ###   ##    ##   #######   ########  ##    ##");
	draw_fstring_center(&dbuf, WINDOW_H / 2 + 4, "Press Space to START ");
	draw_fstring_center(&dbuf, WINDOW_H / 2 + 6, "Press ESC to EXIT");
	draw_back_buffer(&dbuf);

	loop = wait_key();
	if (loop == 0) { free_dbuffer(&dbuf); return 0; } 

	//intro
	draw_fstring_center(&dbuf, WINDOW_H / 2 - 8, "You are the last pilot standing against the INVADER");
	draw_fstring_center(&dbuf, WINDOW_H / 2 - 6, "Shoot them down before INVADER reach Earth");
	draw_fstring_center(&dbuf, WINDOW_H / 2 + 4, "Press Space to CONTINUE ");
	draw_back_buffer(&dbuf);
	sleep(100);
	while(!wait_key());

	//main loop
	reset_play();
	while (loop)
	{
		create_player();
		create_enemies();
		score = play(score);
		
		// game over
		if (player.live != 1)
		{
			draw_player_explode();
			reset_play();

			// death message
			if (player.live == PLAYER_MISS    ) draw_fstring_center(&dbuf, WINDOW_H / 2 - 8, "You missed the INVADER and it reached Earth");
			if (player.live == PLAYER_SUICIDE ) draw_fstring_center(&dbuf, WINDOW_H / 2 - 8, "You committed suicide out of fear of the INVADER");
			if (player.live == PLAYER_SHOTDOWN) draw_fstring_center(&dbuf, WINDOW_H / 2 - 8, "You were shot down by INVADER");
			if (player.live == PLAYER_CRASH   ) draw_fstring_center(&dbuf, WINDOW_H / 2 - 8, "You crashed into the INVADER");

			draw_fstring_center(&dbuf, WINDOW_H / 2 - 4, "Score: %5d", score);
			
			// info 
			draw_fstring_center(&dbuf, WINDOW_H / 2 + 4, "Press Space to RESTART");
			draw_fstring_center(&dbuf, WINDOW_H / 2 + 6, "Press ESC to EXIT");
			draw_back_buffer(&dbuf);
			
			loop = wait_key();
		}
	}
	
	free_dbuffer(&dbuf);
	return 0;
}

static void draw_all()
{
	draw_2d(&dbuf, &player.obj);

	for (int i = 0; i < PLAYER_B_MAX; i++)
		if (player.bullet[i].shot) draw_2d(&dbuf, &player.bullet[i].obj);


	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		if (enemies[i].live != 0)	draw_2d(&dbuf, &enemies[i].obj);
		if (enemies[i].bullet.shot)	draw_2d(&dbuf, &enemies[i].bullet);
	}
}

static int play(int score_old)
{
	int colide, score = 0;
	while (score < ENEMY_COUNT * SCORE_BASIC )
	{
		//tick & key update 
		tick(&counter);
		update_key(&key);

		//enemy tick
		if (is_enemy_tick(&counter))
		{
			update_enemies_position();
		}
		//player tick
		if (is_player_tick(&counter))
		{
			//update position
			update_player_position(&key);
			if (update_player_bullet(&key, is_shot_tick(&counter)))
				reset_shot(&counter);

			update_enemies_kill_frame();

			// draw
			draw_all();
			draw_fstring_at(&dbuf, WINDOW_W - 11, 0, "score %5d", score + score_old);

			//debug
			draw_fstring_at(&dbuf, 0, WINDOW_H - 1, "debug x:%2d y:%2d dx:%2d dy:%2d shot:%2d %2d ep: %5d ",
				player.obj.pos.X, player.obj.pos.Y,
				key.dx / (TICK_FRAME * PLAYER_VS) / 2, key.dy / (TICK_FRAME * PLAYER_VS),
				key.shotx, key.shoty, 
				counter.enemy_period
			);

			draw_back_buffer(&dbuf);

			//colide
			colide = check_colide_all(&score);
			if (check_bound_out()) { player.live = PLAYER_MISS;  break; }
			if (key.loop==0) { player.live = PLAYER_SUICIDE;  break; }
			if (colide) { player.live = colide;  break; }        
			
			counter.enemy_period = counter.enemy_period > TICK_FRAME ? TICK_FRAME * ENEMY_VE - ENEMY_AC * ((score + score_old) / SCORE_BASIC) : 0;
		}
	}
	key.loop = 1;
	return score + score_old;
}

static void draw_player_explode()
{
	int i = PLAYER_EXP_1;
	while (i <= PLAYER_EXP_END)
	{
		tick(&counter);

		if (is_player_tick(&counter))
		{
			player.obj.current_frame = i;
			draw_2d(&dbuf, &player.obj);
			draw_back_buffer(&dbuf);
			i++;
		}
	}
}

static void reset_play()
{
	init_tick(&counter, TICK_FRAME, TICK_FRAME * ENEMY_VE, TICK_FRAME * PLAYER_B_DELAY);
	score = 0;
	key.loop = 1;
	key.dx = 0;
	key.dy = 0;
	player.live = 1;
}

static int wait_key()
{
	while (1)
	{
		update_key(&key);
		if (key.loop==1) return 1; // reset
		else if (key.loop==0) return 0;//exit
		else continue;
	}
}
