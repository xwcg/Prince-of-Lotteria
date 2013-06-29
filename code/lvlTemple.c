#ifndef lvlTemple_c
#define lvlTemple_c

#include "lvlTemple.h"
#include "lvlLottifant.h"

void lvlTempleInit ()
{
	lvlTempleReset();
	lvlTempleStart();
}

void lvlTempleReset ()
{
	media_stop(0);
	g_fhLvlTempleSong = 0;
	
	level_load(NULL);
	freeze_mode = 0;
	sky_active = 0;
}

void lvlTempleMusic ()
{
	g_fhLvlTempleIntro = 0;
	g_fhLvlTempleSong = 0;
	
	g_fhLvlTempleIntro = media_play(LVL_TEMPLE_INTRO, NULL, 100);
	
	while (g_bLvlTempleRunning && g_fhLvlTempleIntro != 0 && media_playing(g_fhLvlTempleIntro))
		wait(1);

	g_fhLvlTempleIntro = 0;

	if (g_bLvlTempleRunning)
	{
		g_fhLvlTempleSong = media_loop(LVL_TEMPLE_MUSIC, NULL, 100);
		
		while (g_bLvlTempleRunning && g_fhLvlTempleSong != 0 && media_playing(g_fhLvlTempleSong))
			wait(1);
			
		g_fhLvlTempleSong = 0;
	}
}

void lvlTempleStart ()
{
	while (total_frames < 1)
		wait(1);
		
	skychange();
	
	g_bLvlTempleRunning = true;
	
	lvlTempleMusic();
	
	gui_show();
	
	fog_color = 0;
	camera.arc = g_lvlTempleCamArc;
	
	level_load_ext(LVL_TEMPLE_WMB);
}

void lvlTempleExit (BOOL bNextLevel)
{
	sky_active = 0;
	g_bLvlTempleRunning = false;
	
	media_stop(0);
	g_fhLvlTempleIntro = 0;
	g_fhLvlTempleSong = 0;
	
	player = NULL;
	
	lvlTempleReset();
	
	level_load(NULL);
	
	gui_hide();
	
	if (bNextLevel)
	{
		achievement("firstlevel");	
		lvlLfInit();
	}
	else
		menu_open();
}

action lvlTpTrigExit ()
{
	while (1)
	{
		if (player != NULL)
			if (vec_dist(player.x, my.x) <= 150 || (key_a && key_c && key_k))
				break;
				
		wait(1);
	}
	
	proc_mode = PROC_GLOBAL;	
	lvlTempleExit(true);
}

#endif /* lvlTemple_c */