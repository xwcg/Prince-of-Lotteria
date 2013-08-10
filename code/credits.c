#ifndef _CREDITS_C_
#define _CREDITS_C_

#include "credits.h"

#include "ini.h"
#include <strio.c>
#include "menu.h"
#include "zorroMesh.h"
#include "lyrics.h"
#include "gui.h"

void creditsInit ()
{
	#ifndef SKIP_CREDITS
	
		creditsReset();
		creditsStart();
		
	#else
		creditsExit();
	#endif
}

void creditsReset ()
{
	sky_active = 0;
	
	g_bCrFirstLotti = false;
	
	gui_hide();
	
	creditsFinished = 0;
	currentRow = 0;
	
	resetPpSwirl();
	
	camera->clip_far = 30000;
	
	vec_set(&g_vecCreditsCamShake, nullvector);
	
	snd_stopall(4);
	g_fhCreditsSong = 0;
	
	reset(g_txtCreditsSpace, SHOW);
	
	reset(creditsHead1, SHOW);
	reset(creditsHead2, SHOW);
	reset(creditsBody1, SHOW);
	reset(creditsBody2, SHOW);
	
	g_bCreditsAllExplode = false;
	
	on_space = NULL;
	
	level_load(NULL);
}

void creditsExit ()
{
	creditsReset();
	menu_open();
}

void creditsFog ()
{
	camera.fog_start = 128;
	camera.fog_end = 4000;
	
	fog_color = 1;
	vec_set(&d3d_fogcolor1.blue, vector(162, 174, 123));
	
	VECTOR fogColor;
	vec_set(&fogColor, vector(d3d_fogcolor1.blue, d3d_fogcolor1.green, d3d_fogcolor1.red));
	
	while (!creditsFinished)
	{
		vec_lerp(d3d_fogcolor1.blue, &fogColor, &sky_color, 0.5);
		wait(1);
	}
}

void creditsStart ()
{
	reset(camera, SHOW);
	
	wait(1);
	
	skychange(0.1);
	setHdr(LVL_CREDITS_HDR_STRENGTH, LVL_CREDITS_HDR_THRESHOLD, LVL_CREDITS_HDR_EXPOSURE);
	
	detail_size = 64;	
	level_load_ext("credits.wmb");

	creditsFog();
	creditsText();
	
	camera.arc = 65;
	
	wait(1);
	
	set(camera, SHOW);
	
	credits_populate();
	
	while (key_a && key_c && key_k)
		wait(1);
	
	while (!creditsFinished)
	{
		if (key_a && key_c && key_k)
			break;
		
		wait(1);
	}	
	
	g_bCreditsAllExplode = true;
}

void creditsText ()
{
	static char strIni [256];
	sprintf(strIni, "%s\\credits.ini", _chr(work_dir));
	
	// start music and lyrics
	{
		snd_stop(g_fhCreditsSong);
		g_fhCreditsSong = snd_play(g_musicCredits, 100, 0);
		
		lyricsStart(NULL, g_txtCreditsLyrics, g_fhCreditsSong);
	}
	
	int pos = 0;
	int count = ini_read_int(strIni, "Credits", "max", "0");
	var duration = ini_read_int(strIni, "Credits", "duration", "15");
	
	var itemDuration = duration / (var)count;
	
	creditsHead1.flags |= SHOW;
	creditsBody1.flags |= SHOW;
	creditsHead2.flags |= SHOW;
	creditsBody2.flags |= SHOW;
	
	creditsHead2.pos_y = screen_size.y - g_lyricsBarHeight - 150;
	creditsBody2.pos_y = screen_size.y - g_lyricsBarHeight - 100;
		
	int size1 = 300;
	int size2 = 600;
	
	var x = 0;
	var len = 0;
	
	while (snd_playing(g_fhCreditsSong) != 0 && !g_bCreditsAllExplode)
	{
		static char section [256];
		sprintf(section, "Credit%d", pos + 1);

		ini_read(creditsText1Head, strIni, section, "Head1", " ");
		ini_read(creditsText1Body, strIni, section, "Body1", " ");
		ini_read(creditsText2Head, strIni, section, "Head2", " ");
		ini_read(creditsText2Body, strIni, section, "Body2", " ");
		
		str_replaceall(creditsText1Body, "##", "\n");
		str_replaceall(creditsText2Body, "##", "\n");
		
		creditsHead1.pos_x = -size1;
		creditsBody1.pos_x = -size1;
		
		creditsHead2.pos_x = screen_size.x + size2;
		creditsBody2.pos_x = screen_size.x + size2;
		
		var fadetime = 0.22 * itemDuration;
		
		var fade = 0;
		var fadespeed = 16 / fadetime;
		
		for (fade = 0; fade < 100; fade += fadespeed * time_step)
		{
			creditsHead1.pos_x = -size1 + 1.5 * size1 * (1 - pow(1 - 0.01 * fade, 2));
			creditsBody1.pos_x = -size1 + 1.7 * size1 * (1 - pow(1 - 0.01 * fade, 2));
			
			if (g_bCreditsAllExplode)
				break;
			
			wait(1);
		}
		
		for (fade = 0; fade < 100; fade += fadespeed * time_step)
		{
			creditsHead2.pos_x = screen_size.x + size2 - 1.2 * size2 * (1 - pow(1 - 0.01 * fade, 2));
			creditsBody2.pos_x = screen_size.x + size2 - 1.3 * size2 * (1 - pow(1 - 0.01 * fade, 2));
			
			if (g_bCreditsAllExplode)
				break;
			
			wait(1);
		}
		
		var t_id = 0.6 * itemDuration * 16;
		
		while (t_id > 0 && !g_bCreditsAllExplode)
		{
			t_id -= time_step;
			wait(1);
		}
		
		var scrollspeed = 100;
		
		for (fade = 0; fade < 100; fade += fadespeed * time_step)
		{
			creditsHead1.pos_x += scrollspeed * time_step;
			
			if (fade > 20)
				creditsBody1.pos_x += scrollspeed * time_step;
				
			if (g_bCreditsAllExplode)
				break;
				
			wait(1);
		}
		
		for (fade = 0; fade < 100; fade += fadespeed * time_step)
		{
			creditsHead2.pos_x -= scrollspeed * time_step;
			
			if (fade > 20)
				creditsBody2.pos_x -= scrollspeed * time_step;
				
			if (g_bCreditsAllExplode)
				break;
				
			wait(1);
		}
		
		pos += 1;
		
		wait(1);
	} 
	
	snd_stopall(4);
	
	creditsHead1.flags &= ~SHOW;
	creditsBody1.flags &= ~SHOW;
	creditsHead2.flags &= ~SHOW;
	creditsBody2.flags &= ~SHOW;
	
	g_bCreditsAllExplode = true;
	
	creditsFinished = true;
	
	var et = 5 * 16;
	
	while (et > 0)
	{
		camera->tilt += time_step * 0.2;
		camera->arc += time_step * 0.2;
		
		et -= time_step;
		wait(1);
	}
	
	// show space text
	{
		set(g_txtCreditsSpace, SHOW);
		g_txtCreditsSpace->pos_x = screen_size.x / 2;
		g_txtCreditsSpace->pos_y = screen_size.y * 0.8;
		
		on_space = creditsSpace;
	}
}

void credits_populate ()
{
	g_crRow = 0;
	int column = 0;
	
	var cambase = 64;
	var camspeed = 20;
	
	var fadespeed = 15;
	
	double tilt = -14;
	double tiltSpeed = 0.0025;
	
	while (!creditsFinished && !g_bCreditsAllExplode)
	{
		camera->pan = 90;
	
		VECTOR pos;
		
		pos.y = 256 + 128 * g_crRow;
		pos.x = -128 * column + 64 * g_crRow;
		pos.z = 8;
		
		ENTITY* e = ent_create(CR_LOTTI_FILE, pos, credits_lotti);
		
		e->pan = camera->pan + 180 + (10 - random(20)) * sign(g_crRow);
		e->flags |= TRANSLUCENT;
		
		for (e->alpha = 0; e->alpha < 100; e->alpha += fadespeed * time_step)
		{
			camera.z = cambase + camspeed * (g_crRow) + camspeed * ((var)(column + 0.005 * e->alpha) / (g_crRow+1));
			camera.y = -camspeed * (g_crRow) - camspeed * ((var)(column + 0.005 * e->alpha) / (g_crRow+1));
			
			vec_add(camera->x, &g_vecCreditsCamShake);

			tilt -= (double)time_step * tiltSpeed;
			camera.tilt = tilt;
			
			wait(1);
		}
		
		e->alpha = 100;
		
		var i;
		for (i = 0; i < 100; i += fadespeed * time_step)
		{
			camera.z = cambase + camspeed * (g_crRow) + camspeed * ((var)(column + 0.5 + 0.005 * i) / (g_crRow+1));
			camera.y = -camspeed * (g_crRow) - camspeed * ((var)(column + 0.5 + 0.005 * i) / (g_crRow+1));
			
			vec_add(camera->x, &g_vecCreditsCamShake);
			
			tilt -= (double)time_step * tiltSpeed;
			camera.tilt = tilt;
			
			wait(1);
		}
		
		column += 1;
		
		if (column > g_crRow)
		{
			column = 0;
			g_crRow += 1;
		}
	}
}

void credits_fire (PARTICLE *p)
{
	p->z -= random(p->vel_z);
	
	p->vel_x = random(1) - 0.5;
	p->vel_y = random(1) - 0.5;
	p->vel_z = random(1) - 0.5;
	
	p->size = 8 + random(16);
	
	p->red = 128;
	p->green = 64;
	p->blue = 0;
	
	p->flags = BRIGHT | MOVE;
	
	p->event = credits_fire_event;
}

void credits_fire_event (PARTICLE* p)
{
	p->alpha -= 2 * time_step;
	p->z -= 2 * time_step;
	
	if (p->alpha <= 0)
		p->lifespan = 0;
}

void creditsCamShake ()
{
	int f = 2 + random(10);
	
	int strength = 7;
	
	while (f > 0)
	{
		g_vecCreditsCamShake.y += strength - random(strength * 2);
		g_vecCreditsCamShake.z += strength - random(strength * 2);
		
		f--;
		wait(1);
	}
	
	g_vecCreditsCamShake.x = g_vecCreditsCamShake.y = 0;
}

action explo_sprite ()
{
	var s = snd_play(creditsExplo, 100, 0);
	snd_tune(s, 100, 80 + random(40), 0);
	creditsCamShake();
	
	vec_scale(my->scale_x, 1 + random(2));

	my.flags |= BRIGHT | TRANSLUCENT;
	my.alpha = 100;
	
	while (my.frame < 13)
	{
		my.frame += 0.6 * time_step;
		wait(1);
	}
	
	ptr_remove(my);
}

action acCreditsTerrain ()
{
	my->material = mtl_terrain;
	g_entCreditsTerrain = my;
}

action acCreditsSign ()
{
	my->flags |= NOFOG;
	my->material = creditsBlinker;	
}

BOOL crLottiUpdateRaise (ENTITY* e)
{
	e->z = e->alpha - 125;
	e->pan = e->skill1 - (100 - e->alpha) * 5.4;
	ent_animate(e, "idle", 5 * total_ticks + e->crInit, ANM_CYCLE);
	
	return (e->alpha >= 100);
}

BOOL crLottiUpdateIdle (ENTITY* e)
{
	ent_animate(e, "idle", 5 * total_ticks + e->crInit, ANM_CYCLE);
	e->crTime -= time_step;
	
	return (g_bCreditsAllExplode || (e->crTime < 0 && e->crExploder));
}

BOOL crLottiUpdateAttack (ENTITY* e)
{
	ent_animate(e, "attack", 5 * total_ticks + e->crInit, ANM_CYCLE);
	e->crTime -= time_step;
	
	return (e->crTime <= 0);
}

BOOL crLottiUpdateFly (ENTITY* e)
{
	VECTOR v, w;
	
	vec_set(&w, e->crAccel);
	vec_scale(&w, time_step);
	vec_add(e->crSpeed, &w);
	
	vec_set(&v, e->crSpeed);
	vec_scale(&v, time_step);
	vec_add(e->x, &v);
	
	VECTOR feet;
	
	feet.x = feet.y = 0;
	feet.z = 0.5 * e->min_z;
	
	vec_rotate(feet, e->pan);
	vec_add(feet, e->x);
	
	ent_animate(e, "drown", 5 * total_ticks + e->crInit, ANM_CYCLE);
	
	effect(credits_fire, 2, feet, e->crSpeed);
	
	// explode near screen border

	VECTOR v;
	vec_set(&v, e->x);
	
	vec_to_screen(&v, camera);
	
	return (v.x < e->crPercent * screen_size.x || v.x > (1 - e->crPercent) * screen_size.x ||
	        v.y < e->crPercent * screen_size.y || v.y > (1 - e->crPercent) * screen_size.y);
}

BOOL crLottiUpdate (ENTITY* e)
{
	int m = (int)e->crState;
	BOOL b = true;
	
	switch (m)
	{
		case 0:   set(e, INVISIBLE); m++; break;
		
		case 1:   reset(e, INVISIBLE);
		          e->crSwordAtBelt = (random(100) < 20);
		          zorroMeshOptions(e, false, e->crSwordAtBelt, false);
		          e->material = g_mtlLotti;
		          e->crInit = random(100);
				  e->skill1 = e->pan;
				  
				  if (g_crRow > 1)
					e->crExploder = (random(100) < 20);
				  
				  m++;
				  
		case 2:   if (crLottiUpdateRaise(e)) m++; else break;
		
		case 3:   reset(e, TRANSLUCENT);
		          e->crTime = 64 + random(128);
		          m++;
		
		case 4:   if (crLottiUpdateIdle(e)) m++; else break;
		
		case 5:   zorroMeshOptions(e, false, false, true);
		          e->crTime = 8 + random(32);
		          m++;
		
		case 6:   if (crLottiUpdateAttack(e)) m++; else break;
		
		case 7:   if (e->crExploder)
		             vec_set(e->crAccel, vector(2.5 * (random(1) - 0.5), 2.5 * (random(1) - 0.5), 1 + random(2)));
		          else
		             vec_set(e->crAccel, vector(0.25 * (random(1) - 0.5), 0.25 * (random(1) - 0.5), 1 + random(2)));
					 
				  ent_playsound(e, g_sndJetpackCredits, 6000);
	
		          vec_to_angle(e.pan, e->crAccel);
		          e.tilt -= 90;
		          e->crTime = 10 + random(12);
		          e->crPercent = random(0.2);
				  m++;
				  
		case 8:   if (crLottiUpdateFly(e)) m++; else break;
		
		case 9:   ent_create("explo+13.tga", e->x, explo_sprite); m++; break;
		
		case 10:  ptr_remove(e); b = false; break;
	}
	
	e->crState = m;
	
	return b;
}

action credits_lotti ()
{
	while (crLottiUpdate(my))
		wait(1);
}

void creditsSpace ()
{
	creditsExit();
}

#endif /* _CREDITS_C_ */