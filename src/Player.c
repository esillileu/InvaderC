#include "Object.h"

Player player;

static int choose_bullet_frame(Bullet* bullet) {
	if ((bullet->dx != 0) && (bullet->dy != 0))		return BULLET_D;
	else if (bullet->dy)							return BULLET_Y;
	else											return BULLET_X;
}

static int choose_player_frame(KeyProcess* p_key) {
	if (p_key->dy == 0 && p_key->dx == 0)		return PLAYER_STOP;
	if (abs(p_key->dy) >= abs(p_key->dx))	return (p_key->dy >= 0) ? PLAYER_DOWN : PLAYER_UP;
	else									return (p_key->dx >= 0) ? PLAYER_RIGHT : PLAYER_LEFT;
}

void create_player(void)
{
	Sprite* sprite = get_sprite(PLAYER);
	Sprite* sprite_bullet = get_sprite(PLAYER_BULLET);
	Heatmap* heatmap = get_heatmap(PLAYER);
	Heatmap* heatmap_bullet = get_heatmap(PLAYER_BULLET);

	player.obj = create_obj(COORD_INIT_X-sprite->w, COORD_INIT_Y-sprite->h, sprite, heatmap);
	player.live = 1;

	for (int i = 0; i < PLAYER_BULLET_MAX; i++)
	{
		player.bullet[i].obj = create_obj(COORD_INIT_X, COORD_INIT_Y, sprite_bullet, heatmap_bullet);
		player.bullet[i].shot = FALSE;
		player.bullet[i].dx = 0;
		player.bullet[i].dy = 0;
	}
}

void update_player_position(KeyProcess* p_key)
{
	short coordx = WINDOW_WIDTH - player.obj.sprite->w, coordy = WINDOW_HEIGHT - player.obj.sprite->h;
	player.obj.pos.X += p_key->dx / (TICK_FRAME * PLAYER_VE);
	player.obj.pos.Y += p_key->dy / (TICK_FRAME * PLAYER_VE);
	player.obj.current_frame = choose_player_frame(p_key);

	if (player.obj.pos.X < 0) player.obj.pos.X = 0;
	if (player.obj.pos.Y < 0) player.obj.pos.Y = 0;
	if (player.obj.pos.X > coordx)  player.obj.pos.X = coordx;
	if (player.obj.pos.Y > coordy) player.obj.pos.Y = coordy;

	p_key->dx = 0; p_key->dy = 0;
}

int update_player_bullet(KeyProcess* p_key, int cooltime)
{
	short shot = 0, sucess = 0;
	// check key input
	if (cooltime && (p_key->shotx || p_key->shoty)) shot = 1;
	
	// loop bullet 
	for (int i = 0; i < PLAYER_BULLET_MAX; i++)
	{
		if (player.bullet[i].shot)
		{
			// move 
			player.bullet[i].obj.pos.X += player.bullet[i].dx;
			player.bullet[i].obj.pos.Y += player.bullet[i].dy;
			
			// boundary remove
			if (player.bullet[i].obj.pos.X < 0
				|| player.bullet[i].obj.pos.Y < 0
				|| player.bullet[i].obj.pos.X > WINDOW_WIDTH
				|| player.bullet[i].obj.pos.Y > WINDOW_HEIGHT)
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
			player.bullet[i].dx = p_key->shotx;
			player.bullet[i].dy = p_key->shoty;
			player.bullet[i].obj.current_frame = choose_bullet_frame(&player.bullet[i]);
			shot = 0;
			sucess = 1;
		}
	}
	return sucess;
}

