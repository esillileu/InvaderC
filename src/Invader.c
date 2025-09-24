#include "Object.h"

TickCounter counter;
DoubleBuffer dbuf;
KeyProcess kps;

static int score = 0;

static int play(int score_old);
static void reset_play(void);
static void draw_all(void);

int main(void)
{
	int stage = 0;

	init_dbuffer(&dbuf, WINDOW_W, WINDOW_H);
	init_kps(&kps);
	
	// title screen
	draw_fstring_center(&dbuf, -8, "########  ###   ##    ######    #######   ########  ####### ");
	draw_fstring_center(&dbuf, -7, "   ##     ####  ##   ##    ##   ##    ##  ##        ##    ##");
	draw_fstring_center(&dbuf, -6, "   ##     ## ## ##   ########   ##    ##  ########  ####### ");
	draw_fstring_center(&dbuf, -5, "   ##     ##  ####   ##    ##   ##    ##  ##        ##    ##");
	draw_fstring_center(&dbuf, -4, "########  ##   ###   ##    ##   #######   ########  ##    ##");
	draw_fstring_center(&dbuf,  4, "Press Space to START");
	draw_fstring_center(&dbuf,  6, "Press ESC to EXIT");
	draw_back_buffer(&dbuf);

	if (wait_kps(&kps) == LOOP_END) { free_dbuffer(&dbuf); return 0; }

	//intro
	draw_fstring_center(&dbuf, -8, "You are the last pilot standing against the INVADER");
	draw_fstring_center(&dbuf, -6, "Shoot them down before INVADER reach Earth");
	draw_fstring_center(&dbuf,  4, "Press Space to CONTINUE");
	draw_back_buffer(&dbuf);
	sleep_ms(200);

	while(!wait_kps(&kps));

	//main loop
	reset_play();
	while (1)
	{
		stage++;
		//reset objs
		create_player();
		create_enemies();

		score = play(score);
		
		// game over
		if (player.live != PLAYER_LIVE)
		{
			draw_player_explode(&dbuf, &counter);

			// death message
			if (player.live == PLAYER_MISS)		draw_fstring_center(&dbuf, -8, "You missed the INVADER and it reached Earth");
			if (player.live == PLAYER_SUICIDE)	draw_fstring_center(&dbuf, -8, "You committed suicide out of fear of the INVADER");
			if (player.live == PLAYER_SHOTDOWN)	draw_fstring_center(&dbuf, -8, "You were shot down by INVADER");
			if (player.live == PLAYER_CRASH)	draw_fstring_center(&dbuf, -8, "You crashed into the INVADER");

			draw_fstring_center(&dbuf, -4, "Score: %5d", score);

			// info 
			draw_fstring_center(&dbuf, 4, "Press Space to RESTART");
			draw_fstring_center(&dbuf, 6, "Press ESC to EXIT");
			draw_back_buffer(&dbuf);

			if (!wait_kps(&kps)) break;
			reset_play();
		}
		else draw_player_to_init(&dbuf, &counter);
	}
	
	free_dbuffer(&dbuf);
	return 0;
}

static int play(int score_old)
{
	int colide, score = 0;
	while (score < ENEMY_COUNT * SCORE_BASIC )
	{
		//tick & kps update 
		tick(&counter);
		update_kps(&kps);

		if (is_enemy_tick(&counter))
		{
			update_enemies_position();
		}

		if (is_player_tick(&counter))
		{
			update_player_position(&kps);

			//bullet shot & cooltime
			if (update_player_bullet(&kps, is_shot_tick(&counter)))
				reset_tick(&counter, &counter.shot_tick);

			update_enemies_kill_frame();

			// draw
			draw_all();
			draw_fstring_at(&dbuf, WINDOW_W - 11, 0, "score %5d", score + score_old);

			//debug
			draw_fstring_at(&dbuf, 0, WINDOW_H - 1, "debug x:%2d y:%2d dx:%2d dy:%2d shot:%2d %2d score: %5d ",
				player.obj.pos.X, player.obj.pos.Y,
				kps.dx / (TICK_FRAME * PLAYER_VS) / 2, kps.dy / (TICK_FRAME * PLAYER_VS),
				kps.shotx, kps.shoty, 
				score
			);

			draw_back_buffer(&dbuf);

			//colide 
			colide = check_colide_all(&score);
			counter.enemy_period = counter.enemy_period > TICK_FRAME ? TICK_FRAME * ENEMY_VE - ENEMY_AC * ((score + score_old) / SCORE_BASIC) : 0;

			//gameover condition
			if (check_bound_out()	){ player.live = PLAYER_MISS;		break; }
			if (kps.loop == LOOP_END){ player.live = PLAYER_SUICIDE;	break; }
			if (colide				){ player.live = colide;			break; }        
		}
	}
	return score + score_old;
}

static void reset_play(void)
{
	//reset tick
	init_tick(&counter, TICK_FRAME, TICK_FRAME * ENEMY_VE, TICK_FRAME * PLAYER_B_DELAY);

	//reset kps
	kps.loop = -1, kps.dx = 0, kps.dy = 0;
	player.live = 1;
	score = 0;
}

static void draw_all(void)
{
	draw_obj(&dbuf, &player.obj);

	for (int i = 0; i < PLAYER_B_MAX; i++)
		if (player.bullet[i].shot) draw_obj(&dbuf, &player.bullet[i].obj);


	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		if (enemies[i].live != 0)	draw_obj(&dbuf, &enemies[i].obj);
		if (enemies[i].bullet.shot)	draw_obj(&dbuf, &enemies[i].bullet);
	}
}