#ifndef lvlBoss_c
#define lvlBoss_c

#include "lvlBoss.h"

void lvlBossInit ()
{
	lvlBossReset();
	lvlBossStart();
}

void lvlBossReset ()
{
	snd_stop(g_fhLvlBossSong);
	g_fhLvlBossSong = 0;
	
	level_load(NULL);
	freeze_mode = 0;
	sky_active = 0;
}

void lvlBossStart ()
{
	while (total_frames < 1)
		wait(1);
		
	skychange();
	gui_show();
		
	// start music
	g_fhLvlBossSong = snd_loop(g_musicBoss, 100, 0);
	
	fog_color = 0;
	camera.arc = g_lvlBossCamArc;
	
	level_load_ext(LVL_BOSS_WMB);
}

void lvlBossExit ()
{
	wait(1);
	
	sky_active = 0;

	player = NULL;
	
	lvlBossReset();
	
	wait(1);
	
	achievement("lottifant");
	
	creditsInit();
}

action lvlBossTriggerExit ()
{
	while (1)
	{
		if (player != NULL)
			if (vec_dist(player.x, my.x) <= 250 || (key_a && key_c && key_k))
				break;
		
		wait(1);
	}
	
	proc_mode = PROC_GLOBAL;
	lvlBossExit();
}

#endif /* lvlBoss_c */