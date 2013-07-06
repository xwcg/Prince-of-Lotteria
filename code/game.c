#ifndef _GAME_C_
#define _GAME_C_

#include "game.h"
#include "player.h"
#include "lvlIntro.h"
#include "lvlTemple.h"
#include "lvlLottifant.h"

void game_start()
{
	#ifndef SKIP_INTRO
	
		PANEL* panBlack = pan_create(" ", 100);
		vec_set(panBlack.blue, vector(8,8,8));
		panBlack.size_x = screen_size.x;
		panBlack.size_y = screen_size.y;
		pan_setdigits(panBlack, 0, 5, 5, "Press [Space] to skip", font_create("Arial#24b"), 1, vDummy);
		pan_setcolor(panBlack, 1, 1, vector(255,255,255));
		set(panBlack, SHOW | LIGHT);
		
		var vMediaHandle = snd_play(g_musicIntro, 100, 0);
		
		while (key_esc || key_space || key_enter)
			wait(1);
		
		while (snd_playing(vMediaHandle) && !key_esc && !key_space && !key_enter)
			wait(1);
		
		ptr_remove(panBlack);
		
		snd_stop(vMediaHandle);
		vMediaHandle = 0;
	
	#endif
	
	nPlayerLifes = 3;
	flying_man = 0;
	gui_start();
	
	// standard start level = temple
	#ifndef SKIP_TO
		lvlTempleInit();
	#else
		
		#ifdef SKIP_TO_LOTTIFANTLEVEL
			lvlLfInit();
		#endif
	
		#ifdef SKIP_TO_BOSSLEVEL
			lvlBossInit();
		#endif
		
		#ifdef SKIP_TO_LAVASTAGE
			lvlLavastageInit();
		#endif
		
		#ifdef SKIP_TO_CREDITS
			creditsInit();
		#endif
		
	#endif
}

void game_restart ()
{
	level_load(level_name);
}

void on_level_event(var _percent) {
	if (panLoadBar != NULL) {
		panLoadBar.size_x = (screen_size.x / 100) * _percent;
	}
}

void on_level_loaded_event() {
	if (panLoad != NULL) reset(panLoad, SHOW);
	if (panLoadBar != NULL) reset(panLoadBar, SHOW);
}

void level_load_ext(STRING* _lvl) {
	if (_lvl != NULL) {
		if (panLoad == NULL) {
			on_level = on_level_event;
			on_level_load = on_level_loaded_event;
			panLoad = pan_create("", 100);
			vec_set(panLoad.blue, vector(8,8,8));
			pan_setdigits(panLoad, 0, 10, 10, "Loading...", font_create("Arial#30b"), 1, vDummy2);
			pan_setcolor(panLoad, 1, 1, vector(255,255,255));
			
			panLoadBar = pan_create("", 101);
			vec_set(panLoadBar.blue, vector(0,0,255));
		}
		
		panLoad.size_x = screen_size.x;
		panLoad.size_y = screen_size.y;
		panLoadBar.size_x = 0;
		panLoadBar.size_y = 40;
		panLoadBar.pos_x = 0;
		panLoadBar.pos_y = screen_size.y - 100;		
		set(panLoad, LIGHT | SHOW);
		set(panLoadBar, LIGHT | SHOW);
		
		wait(1);
		level_load(_lvl);
	}
}

#endif // #ifndef _GAME_C_