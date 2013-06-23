#ifndef _GAME_C_
#define _GAME_C_

#include "game.h"
#include "player.h"

void game_start()
{
	PANEL* panBlack = pan_create(" ", 100);
	vec_set(panBlack.blue, vector(8,8,8));
	panBlack.size_x = screen_size.x;
	panBlack.size_y = screen_size.y;
	pan_setdigits(panBlack, 0, 5, 5, "Press [Space] to skip", font_create("Arial#24b"), 1, vDummy);
	pan_setcolor(panBlack, 1, 1, vector(255,255,255));
	set(panBlack, SHOW | LIGHT);
	var vMediaHandle = media_play("music\\lotteria_intro_with_music.wav", NULL, 100);
	while(key_esc || key_space || key_enter) wait(1);
	while(media_playing(vMediaHandle) && !key_esc && !key_space && !key_enter) {
		wait(1);
	}
	ptr_remove(panBlack); //fix by firo (cleanup due to restart after game over)
	media_stop(vMediaHandle);
	vMediaHandle = 0;
	
	nPlayerLifes = 3; //fix by firo
	flying_man = 0;//fix by firo
	level_load_ext("level01_v7.wmb"); // by padmalcom
	//level_load_ext("bosslevel.wmb");
	VECTOR* vecPlayerPosition = vector(-432,-80,40);
	wait(3);
	skychange();
	freeze_mode = 0;
	gui_start();
	gui_show();
	vGameMusicHandle = media_play("music\\POL_game_intro.wav", NULL, 70);
	wait(-8);
	vGameMusicHandle = media_loop("music\\POL_game_looped.wav", NULL, 50);
}

void game_restart(void) {
	//level_load(level_name);
	level_load_ext(level_name);
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