#include "Object.h"

#define SIGN(x) (((x) > 0) - ((x) < 0))

Player player;

static int choose_bullet_frame(Bullet* bullet) {
	if ((bullet->dx != 0) && (bullet->dy != 0))		return BULLET_D;
	else if (bullet->dy)							return BULLET_Y;
	else											return BULLET_X;
}

static int choose_player_frame(KeyProcess* p_kps) {
	if (p_kps->dy == 0 && p_kps->dx == 0)		return PLAYER_STOP;
	if (abs(p_kps->dy) >= abs(p_kps->dx))	return (p_kps->dy >= 0) ? PLAYER_DOWN : PLAYER_UP;
	else									return (p_kps->dx >= 0) ? PLAYER_RIGHT : PLAYER_LEFT;
}

void create_player(void)
{
	const Sprite* sprite = get_sprite(PLAYER);
	const Sprite* sprite_bullet = get_sprite(PLAYER_BULLET);
	const Heatmap* heatmap = get_heatmap(PLAYER);
	const Heatmap* heatmap_bullet = get_heatmap(PLAYER_BULLET);

	player.obj = create_obj(COORD_INIT_X-sprite->w, COORD_INIT_Y-sprite->h, sprite, heatmap);
	player.live = 1;

	for (int i = 0; i < PLAYER_B_MAX; i++)
	{
		player.bullet[i].obj = create_obj(COORD_INIT_X, COORD_INIT_Y, sprite_bullet, heatmap_bullet);
		player.bullet[i].shot = FALSE;
		player.bullet[i].dx = 0;
		player.bullet[i].dy = 0;
	}
}

void update_player_position(KeyProcess* p_kps)
{
	short coordx = WINDOW_W - player.obj.sprite->w, coordy = WINDOW_H - player.obj.sprite->h;
	player.obj.pos.X += p_kps->dx * PLAYER_VX / (TICK_FRAME * PLAYER_VS);
	player.obj.pos.Y += p_kps->dy * PLAYER_VY / (TICK_FRAME * PLAYER_VS);
	player.obj.current_frame = choose_player_frame(p_kps);

	if (player.obj.pos.X < 0) player.obj.pos.X = 0;
	if (player.obj.pos.Y < 0) player.obj.pos.Y = 0;
	if (player.obj.pos.X > coordx)  player.obj.pos.X = coordx;
	if (player.obj.pos.Y > coordy) player.obj.pos.Y = coordy;
}

int update_player_bullet(KeyProcess* p_kps, int cooltime)
{
	short shot = 0, sucess = 0;
	// check key input
	if (cooltime && (p_kps->shotx || p_kps->shoty)) shot = 1;
	
	// loop bullet 
	for (int i = 0; i < PLAYER_B_MAX; i++)
	{
		if (player.bullet[i].shot)
		{
			// move 
			player.bullet[i].obj.pos.X += player.bullet[i].dx * PLAYER_B_VX;
			player.bullet[i].obj.pos.Y += player.bullet[i].dy * PLAYER_B_VY;
			
			// boundary remove
			if (player.bullet[i].obj.pos.X < 0
				|| player.bullet[i].obj.pos.Y < 0
				|| player.bullet[i].obj.pos.X > WINDOW_W
				|| player.bullet[i].obj.pos.Y > WINDOW_H)
			{
				player.bullet[i].shot = FALSE;
			}
		}

		// shot
		else if (shot)
		{
			player.bullet[i].obj.pos.X = player.obj.pos.X + player.obj.sprite->w / 2 - player.bullet[i].obj.sprite->w /2;
			player.bullet[i].obj.pos.Y = player.obj.pos.Y + player.obj.sprite->h / 2;
			player.bullet[i].shot = TRUE;
			player.bullet[i].dx = p_kps->shotx;
			player.bullet[i].dy = p_kps->shoty;
			player.bullet[i].obj.current_frame = choose_bullet_frame(&player.bullet[i]);
			shot = 0;
			sucess = 1;
		}
	}
	return sucess;
}

void draw_player_explode(DoubleBuffer* p_dbuf, TickCounter* p_counter )
{
	int i = PLAYER_EXP_1;
	while (i <= PLAYER_EXP_END)
	{
		tick(p_counter);

		if (is_player_tick(p_counter))
		{
			player.obj.current_frame = i;
			draw_obj(p_dbuf, &player.obj);
			draw_back_buffer(p_dbuf);
			i++;
		}
	}
}

void draw_player_to_init(DoubleBuffer* p_dbuf, TickCounter* p_counter)
{
	Pos pos = { COORD_INIT_X - player.obj.sprite->w, COORD_INIT_Y - player.obj.sprite->h };
	
	short dx = pos.X - player.obj.pos.X, dy = pos.X - player.obj.pos.X;
	short min = (dx < dy) ? dx : dy;
	dx = (min ? dx / min : dx);
	dy = (min ? dy / min : dy);

	while (pos.X != player.obj.pos.X || pos.Y != player.obj.pos.Y)
	{
		tick(p_counter);

		if (is_player_tick(p_counter))
		{
			player.obj.pos.X += SIGN(pos.X - player.obj.pos.X) * dx;
			player.obj.pos.Y += SIGN(pos.Y - player.obj.pos.Y) * dy;

			draw_obj(p_dbuf, &player.obj);
			draw_back_buffer(p_dbuf);
		}
	}
}
