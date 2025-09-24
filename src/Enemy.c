#include "Object.h"

Enemy enemies[ENEMY_COUNT];
Object2D bound;
short direction = -1;

void create_enemies(void)
{
	Sprite* sprite = get_sprite(ENEMY);
	Sprite* sprite_bullet = get_sprite(ENEMY_BULLET);
	Heatmap* heatmap = get_heatmap(ENEMY);
	Heatmap* heatmap_bullet = get_heatmap(ENEMY_BULLET);

	short w = sprite->w, h = sprite->h, index = 0;
	short margin = (WINDOW_W - (w + ENEMY_GAP) * ENEMY_ROW) / 2;
	short out = 0;
		
	for (int i = 0; i < ENEMY_COUNT / ENEMY_ROW + 1; i++)
	{
		for (int j = 0; j < ENEMY_ROW; j++)
		{
			short index = i * ENEMY_ROW + j;
			if (index == ENEMY_COUNT)
			{
				out = 1;
				break;
			}

			enemies[index].obj = create_obj(
				margin - ENEMY_GAP + ((w + ENEMY_GAP) * j),
				i * (h + ENEMY_GAP / 2),
				sprite, heatmap
			);
			enemies[index].bullet.obj = create_obj(
				COORD_INIT_X, COORD_INIT_Y,
				sprite_bullet, heatmap_bullet
			);

			enemies[index].live = 1;
			enemies[index].bullet.shot = FALSE;
			enemies[index].bullet.dx = 0;
			enemies[index].bullet.dy = 0;
		}
		if (out) break;
	}
	bound = create_obj(enemies[index].obj.pos.X, enemies[index].obj.pos.Y, 0, 0);
}

void update_enemies_position(void)
{
	short w = enemies[0].obj.sprite->w, h = enemies[0].obj.sprite->h;
	short add_y = 0, count = 0;
	
	// shot 
	short check_shot = rand() % ENEMY_COUNT;

	//direction
	if (bound.pos.X <= 0) { direction = 1; add_y = 1; }
	if (bound.pos.X >= WINDOW_W - (w + ENEMY_GAP) * ENEMY_ROW + ENEMY_GAP) { direction = -1; add_y = 1; }

	bound.pos.X += direction;
	bound.pos.Y += add_y;

	//update
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		//move enemy
		if (enemies[i].live)
		{
			enemies[i].obj.pos.X += direction;
			enemies[i].obj.pos.Y += add_y;
		}

		//move bullet
		if (enemies[i].bullet.shot)
		{
			count++;
			enemies[i].bullet.obj.pos.Y += enemies[i].bullet.dy;
			enemies[i].bullet.obj.pos.X += enemies[i].bullet.dx;
		}

		//reset bullet
		if (enemies[i].bullet.shot && enemies[i].bullet.obj.pos.Y > WINDOW_H - 1) 
		{ 
			enemies[i].bullet.shot = FALSE; 
			enemies[i].bullet.obj.pos.Y = 0; 
			enemies[i].bullet.dx = 0;
			enemies[i].bullet.dy = 0;

		}
	}
	
	if (count < ENEMY_COUNT/ENEMY_B_SCALE && (enemies[check_shot].live==1) && !enemies[check_shot].bullet.shot)
	{
		enemies[check_shot].bullet.shot = TRUE;
		enemies[check_shot].bullet.dx = 0;
		enemies[check_shot].bullet.dy = 1;
		enemies[check_shot].bullet.obj.pos.Y = enemies[check_shot].obj.pos.Y + 1;
		enemies[check_shot].bullet.obj.pos.X = enemies[check_shot].obj.pos.X + enemies[check_shot].obj.sprite->w / 2;
	}
}

void update_enemies_kill_frame(void)
{
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		if (enemies[i].live == -1)
		{
			if (enemies[i].obj.current_frame < enemies[i].obj.sprite->frame-1)
				enemies[i].obj.current_frame += 1;
			else
			{
				enemies[i].obj.current_frame = 0;
				enemies[i].live = 0;
			}
		}
	}
}

int check_enemy_bound_out(void) {
	if (bound.pos.Y > WINDOW_H) return 1;
	else return 0;
}