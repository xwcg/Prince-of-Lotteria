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
	snd_stop(g_fhLvlTempleSong);
	g_fhLvlTempleSong = 0;
	
	level_load(NULL);
	freeze_mode = 0;
	sky_active = 0;
}

void lvlTempleMusic ()
{
	g_fhLvlTempleIntro = 0;
	g_fhLvlTempleSong = 0;
	
	g_fhLvlTempleIntro = snd_play(g_musicTempleIntro, 100, 0);
	
	while (g_bLvlTempleRunning && g_fhLvlTempleIntro != 0 && snd_playing(g_fhLvlTempleIntro))
		wait(1);

	g_fhLvlTempleIntro = 0;

	if (g_bLvlTempleRunning)
	{
		g_fhLvlTempleSong = snd_loop(g_musicTempleLoop, 100, 0);
		
		while (g_bLvlTempleRunning && g_fhLvlTempleSong != 0 && snd_playing(g_fhLvlTempleSong))
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
	
	// set camera + walk glide
	setPlayerCamera(g_lvlTempleCamDist, g_lvlTempleCamRaiseZ, g_lvlTempleCamArc);
	setPlayerWalkGlide(true);
	
	level_load_ext(LVL_TEMPLE_WMB);
}

void lvlTempleExit (BOOL bNextLevel)
{
	sky_active = 0;
	g_bLvlTempleRunning = false;
	
	snd_stop(g_fhLvlTempleIntro);
	snd_stop(g_fhLvlTempleSong);
	
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
	
	snd_play(g_sndDoor, 100, 0);
	lvlTempleExit(true);
}

action acTpBat ()
{
	set(my, PASSABLE);
	
	var reactTime = 4 * 16;
	var reactDist = 300 + random(300);
	
	while (1)
	{
		reactTime -= time_step;
		ent_animate(my, "idle", (4 * total_ticks) % 100, ANM_CYCLE);
		
		if (player != NULL && reactTime < 0)
		{
			VECTOR myPos, playerPos;
			
			vec_set(&myPos, my->x);
			vec_set(&playerPos, player->x);
			
			myPos.y = playerPos.y = 0;
			
			if (vec_dist(&myPos, &playerPos) < reactDist)
				break;
		}
		
		wait(1);
	}
	
	var sh = snd_play(g_sndBatFly, 100, 0);
	snd_tune(sh, 0, 90+random(20), 0);
	
	set(my, ZNEAR);
	
	var flyUp = 1 + random(1);
	var flyGravity = 2 + random(2);
	var flyFriction = 0.5;
	var flyForward = 2 + random(2);
	
	var flyPan = 1 * (1 - random(2));
	
	var startAnim = 0;
	var startSpeed = 10 + random(5);
	
	while (startAnim < 100)
	{
		ent_animate(my, "start", minv(startAnim, 100), 0);
		startAnim += startSpeed * time_step;
		
		my->pan += flyPan * time_step;
		my->roll -= flyPan * time_step;
		
		accelerate(my->skill3, -flyGravity, flyFriction);
		my->z += my->skill3;
		
		wait(1);
	}
	
	var flapSpeed = 12+random(5);
	
	while (my->y > camera->y && !(my->eflags & CLIPPED))
	{
		ent_animate(my, "fly", (flapSpeed * total_ticks) % 100, ANM_CYCLE);
		
		my->pan += flyPan * time_step;
		my->roll -= flyPan * time_step;
		
		accelerate(my->skill1, flyForward, flyFriction);
		accelerate(my->skill3, flyUp, flyFriction);
		
		VECTOR dir;
		vec_set(&dir, my->skill1);
		vec_rotate(&dir, my->pan);
		
		vec_add(my->x, &dir);
		
		wait(1);
	}
	
	ptr_remove(my);
}

#endif /* lvlTemple_c */