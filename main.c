#include <acknex.h>
#include <default.c>
#include <ackphysx.h>

#define PRAGMA_PATH "code"
#define PRAGMA_PATH "music"

#define PRAGMA_LEVEL "level01_v7.wmp"
#define PRAGMA_LEVEL "bosslevel.wmp"

#include "credits.h"
#include "achievements.h"
#include "game.h"
#include "platform.h"
#include "traps.h"
#include "items.h"
#include "player.h"
#include "menu.h"
#include "gui.h"
#include "PoL_enemy.h"
#include "zorroMesh.h"
#include "level_effects.c"
#include "credits.h"
#include "trigger.h"
#include "geist.c"

#include "lvlTemple.h"
#include "lvlLottifant.h"
#include "lvlBoss.h"

void main ()
{
	physX_open();
	
	fps_max = 60;
	video_mode = 10;
	mouse_pointer = 0;
	
	d3d_antialias = 9;
	mip_shaded = 2;
	mip_flat = 2;
	
	preload_mode = 2+4;
	
	freeze_mode = 1;
	
	random_seed(0);
	
	menu_open();
	// <<intro>>
	//game_start(); // level 1
	//lvlLfInit(); // level 2 - Lottifant
	// << boss level >>
	// <<musical>>
	//creditsInit();
}