#include <acknex.h>
#include <default.c>
#include <ackphysx.h>

#define PRAGMA_PATH "code"
#define PRAGMA_PATH "music"

#define PRAGMA_LEVEL "level01_v7.wmp"
#define PRAGMA_LEVEL "bosslevel.wmp"

//- dev cheats ---------------------------------------------------------------------------

	//#define SKIP_INTRO

	// activate SKIP_TO and a target to redefine start level
	//#define SKIP_TO
	//#define SKIP_TO_LOTTIFANTLEVEL
	//#define SKIP_TO_BOSSLEVEL
	//#define SKIP_TO_LAVASTAGE
	//#define SKIP_TO_CREDITS

	//#define SKIP_CREDITS
	
	//#define BOSS_DIE_1FINGER
	
	//#define PL_DEBUG
	
//----------------------------------------------------------------------------------------

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
#include "postprocessing.h"

#include "lvlTemple.h"
#include "lvlLottifant.h"
#include "lvlBoss.h"
#include "lvlLavastage.h"

void main ()
{
	warn_level = 0;
	
	video_window(NULL, NULL, 0, "Prince of Lotteria - Zorro Edition v0.2");
	
	fps_max = 60;
	video_mode = 10;
	mouse_pointer = 0;
	
	d3d_antialias = 0;
	mip_shaded = 2;
	mip_flat = 2;
	
	preload_mode = 2+4;
	
	random_seed(0);
	
	initPostprocessing();
	
	menu_open();
	
	game_hotkeys();
}