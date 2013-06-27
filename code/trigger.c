#ifndef _TRIGGER_C_
#define _TRIGGER_C_

//#include "sky.h"

#include "trigger.h"
#include "credits.h"
#include "lvlBoss.h"

action trigger_wall ()
{
	wait(1);
	
	while (1)
	{
		if (player != NULL)
		{
			if (vec_dist(player.x, my.x) <= 150 || (key_a && key_c && key_k))
			{
				wait(1);
				achievement("firstlevel");
				// level_load("bosslevel.wmb");
				//level_load_ext("bosslevel.wmb"); // by padmalcom
				
				media_stop(vGameMusicHandle);
				vGameMusicHandle = 0;
				
				//level_load_ext("bosslevel.wmb"); // by padmalcom
				
				lvlLfInit();
				break;
			}
		}
		wait(1);
	}
}

action trigger_credits ()
{
	wait(1);
	
	while (1)
	{
		if (player != NULL)
		{
			if (vec_dist(player.x, my.x) <= 250 || (key_a && key_c && key_k))
			{
				break;
				
				/*
				wait(1);
				achievement("lottifant");
				wait(-2);
				media_stop(vGameMusicHandle);
				gui_hide();
				sky_active = 0; //fix by firo
				
				creditsInit();
				
				return;
				*/
			}
		}
		
		wait(1);
	}
	
	lvlBossExit();
}

action trigger_visible() {
	VECTOR vecTemp;
	while(1)
	{
		vec_set(vecTemp, my.x);
		if (vec_to_screen(vecTemp, camera) != NULL) {
			switch(my.skill1)
			{
			case 0:
				snd_play(snd_narrator_start, 100, 0);
				break;
			case 1:
				snd_play(snd_narrator_sky, 100, 0);
				break;
			case 2:
				snd_play(snd_narrator_enemy, 100, 0);
				break;
			case 3:
				snd_play(snd_narrator_lottifant, 100, 0);
				break;
			}
			break;
		}
		wait(1);
	}
}

#endif /* _TRIGGER_C_ */