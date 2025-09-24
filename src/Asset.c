#include "Asset.h"

static const char player[13 * 3 * 7] =
	//stop	0
	"       "
	" <[*]> "
	"       "
	//up	1
	"       "
	" <[*]> "
	"   A    "
	//down	2
	"   V   "
	" <[*]> "
	"      "
	//left	3
	"       "
	" <[*]><"
	"       "
	//right	4
	"       "
	"><[*]> "
	"       "
	//exp1	5
	"       "
	" <(*)> "
	"       "
	//exp2	6	
	"   *   "
	" (*@*) "
	"   *   "
	//exp3	7
	"  *@*  "
	"(*@ @*)"
	"  *@*  "
	//exp4  8
	"  @ @  "
	"*@   @*"
	"  @ @  "	
	//exp5	9
	"  * *  "
	"*     *"
	"  * *  "
	//exp6	10
	"   *   "
	" *   * "
	"   *   "
	//exp7	11
	"       "
	"  * *  "
	"       "
	//exp8	12
	"       "
	"       "
	"       "
	;
static const char enemy[4 * 2 * 5] =
//up	0
	"<~^~>" 
	" Y^Y "

	"<(*)>"
	" Y*Y "

	"(@ @)"
	" *@* "

	"*   *"
	"  *  "
	;
static const char player_bullet[3 * 1 * 1] = "|-*";
static const char enemy_bullet[1 * 1 * 3] = "^Y^";

//heatmap
static const char player_heat[3 * 7] = { 
	0,0,0,0,0,0,0, 
	0,1,1,1,1,1,0,
	0,0,0,0,0,0,0,
};
static const char enemy_heat[2 * 5] = {
	1,1,1,1,1, 
	0,1,1,1,0
};
static const char player_bullet_heat[1 * 1] = {1};
static const char enemy_bullet_heat[1 * 3] = { 1, 1, 1 };

static const Sprite SPRITES[] = {
	{12, 3 ,7, player		},
	{ 4, 2, 5, enemy		},
	{ 3, 1 ,1, player_bullet},
	{ 1, 1, 3, enemy_bullet	},
};

static const Heatmap HEATMAP[] = {
	{3 ,7, player_heat			},
	{2, 5, enemy_heat			},
	{1, 1, player_bullet_heat	},
	{1, 3, enemy_bullet_heat	},
};

const Sprite* get_sprite(SpriteId id) {
	return &SPRITES[id];
}

const Heatmap* get_heatmap(SpriteId id) {
	return &HEATMAP[id];
}
