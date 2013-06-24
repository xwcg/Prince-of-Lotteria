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
	media_stop(0);
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
		
	// start music
	{
		if (g_fhLvlBossSong != 0)
			media_stop(g_fhLvlBossSong);
		
		g_fhLvlBossSong = media_loop(LVL_BOSS_MUSIC, NULL, 100);
	}
	
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

#endif /* lvlBoss_c */