#ifndef lvlBoss_c
#define lvlBoss_c

#include "lvlBoss.h"
#include "lvlLavastage.h"
#include "postprocessing.h"
#include "narrator.h"

void lvlBossInit ()
{
	lvlBossReset();
	lvlBossStart();
}

void lvlBossReset ()
{
	snd_stop(g_fhLvlBossSong);
	
	g_fhLvlBossSong = 0;
	g_fhLvlBossSongVol = 100;
	
	g_entBossBottle = NULL;
	g_lvlBossGhostVisible = false;
	
	level_load(NULL);
	sky_active = 0;
	
	resetPpSwirl();
}

void lvlBossStart ()
{
	while (total_frames < 1)
		wait(1);
		
	skychange();
	setHdr(LVL_BOSS_HDR_STRENGTH, LVL_BOSS_HDR_THRESHOLD, LVL_BOSS_HDR_EXPOSURE);
	
	gui_show();
		
	// start music
	g_fhLvlBossSong = snd_loop(g_musicBoss, g_fhLvlBossSongVol, 0);
	
	fog_color = 0;

	// set camera + walk glide
	setPlayerCamera(g_lvlBossCamDist, g_lvlBossCamRaiseZ, g_lvlBossCamArc);
	setPlayerWalkGlide(true);
	
	level_load_ext(LVL_BOSS_WMB);
}

void lvlBossExit (BOOL bNextLevel)
{
	sky_active = 0;
	
	snd_stop(g_fhLvlBossSong);
	g_fhLvlBossSong = 0;
	
	player = NULL;
	
	lvlBossReset();
	
	gui_hide();
	
	if (bNextLevel)
	{
		achievement("tower");
		lvlLavastageInit();
	}
	else
		menu_open();
}

function bounce_event()
{
	if (event_type == EVENT_PUSH && you == player && player != NULL && my->skill1 == 0)
		my->skill1 = 1;
}

action acBossBottle ()
{
	g_entBossBottle = my;
	
	while (!(key_a && key_c && key_k))
		wait(1);
		
	lvlBossExit(true);
}

void zuzu ()
{
	var fRadius = 0;
	var fAngle = 0;
	var fBlend = 0;
	
	var t = 0;
	var fAngleOld = 0;
	
	snd_play(g_sndSwirl, 100, 0);
	
	while (1)
	{
		var s = (t < 64);
		
		accelerate(&fRadius, s * 0.15, 0.1);
		accelerate(&fAngle, s * 0.3, 0.1);
		accelerate(&fBlend, s * 0.07, 0.1);
		
		if (fAngle < fAngleOld)
			break;
		
		setPpSwirl(fRadius, fAngle, 0.5, 0.5, fBlend);
		
		t += time_step;
		fAngleOld = fAngle;
		
		wait(1);
	}
	
	g_lvlLavastageSwirlRadius = fRadius;
	g_lvlLavastageSwirlAngle = fAngle;
	g_lvlLavastageSwirlBlend = fBlend;
	g_lvlLavastageKeepSwirl = true;
	
	lvlBossExit(true);
}

action acBossSpeech ()
{
	set(my, INVISIBLE);
	
	my->push = -50;
	my->emask |= ENABLE_PUSH;
	
	my->event = bounce_event;
	
	while (my->skill1 == 0)
		wait(1);
		
	flying_man = 0;	
	
	zorroMeshOptions(player, true, true, false);
	setPlayerControls(false);
	
	while (player->PL_IS_JUMPING || player->PL_IS_ATTACKING)
		wait(1);
		
	var h = narrate(g_narrateBottleFound);
	setPlayerPan(g_entBossBottle);
	
	while (snd_playing(h))
	{
		g_fhLvlBossSongVol = maxv(2, g_fhLvlBossSongVol - 5 * time_step);
		snd_tune(g_fhLvlBossSong, g_fhLvlBossSongVol, 0, 0);		
		
		wait(1);
	}
		
	g_lvlBossGhostVisible = true;
}

void bossSparkle_ev (PARTICLE* p)
{
    p->alpha -= p->skill_a * time_step;
    
	if (p->alpha <= 0)
		p->lifespan = 0;
}

void bossSparkle_p (PARTICLE* p)
{
   VECTOR vTemp;

   vec_set(&vTemp, vector(random(1)-0.5,random(1)-0.5,random(1)-0.5));
   vec_normalize(&vTemp, random(5));
   
   vec_add(p->vel_x, vTemp);

   set(p, MOVE | BRIGHT | TRANSLUCENT);

   p->alpha = 25 + random(75);
   p->size = 256 + random(256);
   p->bmap = g_bmapSparkle;
   
   p->skill_a = 1 + random(3);

   p->event = bossSparkle_ev;
}

action acBossWarghost ()
{
	my->material = g_mtlBossGhost;
	
	set(my, INVISIBLE);
	
	var startPan = my->pan;
	
	BOOL bSparklesEmitted = false;
	BOOL bToTheNextDimension = false;
	var hNarrate = 0;
	
	while (1)
	{
		if (g_lvlBossGhostVisible)
		{
			g_fhLvlBossSongVol = maxv(2, g_fhLvlBossSongVol - 5 * time_step);
			snd_tune(g_fhLvlBossSong, g_fhLvlBossSongVol, 0, 0);			
			
			if (!bSparklesEmitted)
			{
				snd_play(g_sndSparkle, 100, 0);
				
				reset(my, INVISIBLE);
				
				int i, numVertices = ent_status(my, 0);
				
				for (i = 0; i < numVertices; i++)
				{
					if (i % 32 == 0)
					{
						VECTOR pos;
						vec_for_vertex(&pos, my, i);
						
						effect(bossSparkle_p, 1, &pos, nullvector);
					}
				}
				
				hNarrate = narrate(g_narrateGenieSpeechAfterRelease);
				
				bSparklesEmitted = true;
			}
			
			ent_animate(me, "attack", my->skill[0], ANM_CYCLE);
			
			my->skill[0] += time_step * 3;
			my.pan = startPan + 25 * sinv(total_ticks * 5);
			
			if (hNarrate != 0 && !snd_playing(hNarrate))
			{
				if (!bToTheNextDimension)
				{
					bToTheNextDimension = true;
					zuzu();
				}
			}
		}
		
		wait(1);
	}
}

action acBossTrail ()
{
	set(my, TRANSLUCENT);
	my->alpha = 0;
	
	var morph_speed = 4;
	
	COLOR colours;
	vec_set(&colours, vector(random(360), random(360), random(360)));
	
	my->material = g_mtlBossTrail;
	
	while (1)
	{
		if (g_lvlBossGhostVisible)
			my->alpha = clamp(my->alpha + 5 * time_step, 0, 50);
		
		int i;
		for (i = 0; i < 3; i++)
		{
			(&(colours.blue))[i] = ((&(colours.blue))[i] + time_step * morph_speed) % 360;
			(&(my->material->ambient_blue))[i] = abs(sinv((&(colours.blue))[i]) * 255);
		}
		
		wait(1);
	}
}

#endif /* lvlBoss_c */