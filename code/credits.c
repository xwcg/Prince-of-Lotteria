#ifndef _CREDITS_C_
#define _CREDITS_C_

#include "credits.h"

#include "ini.h"
#include <strio.c>
#include "menu.h"
#include "zorroMesh.h"
#include "lyrics.h"

void creditsInit ()
{
	creditsReset();
	creditsStart();
}

void creditsReset ()
{
	g_bCrFirstLotti = false;
	
	creditsFinished = 0;
	currentRow = 0;
	
	camera->clip_far = 18000;
	
	vec_set(&g_vecCreditsCamShake, nullvector);
	
	if (g_fhCreditsSong != 0)
	{
		media_stop(g_fhCreditsSong);
		g_fhCreditsSong = 0;
	}
	
	reset(g_txtCreditsSpace, SHOW);
	
	reset(creditsHead1, SHOW);
	reset(creditsHead2, SHOW);
	reset(creditsBody1, SHOW);
	reset(creditsBody2, SHOW);
	
	g_entCreditsTerrain = NULL;
	
	g_bCreditsAllExplode = false;
	
	on_space = NULL;
	
	level_load(NULL);
	
	ent_preload(g_crLottiPreload);
	
	freeze_mode = 0;
}

void creditsExit ()
{
	creditsReset();
	menu_open();
}

void creditsStart ()
{
	wait(1);
		
	static char strIni [256];
	sprintf(strIni, "%s\\credits.ini", _chr(work_dir));
	
	// start music and lyrics
	{
		if (g_fhCreditsSong != 0)
			media_stop(g_fhCreditsSong);
		
		g_fhCreditsSong = media_play("music\\CloneFall.ogg", NULL, 100);
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
	
	camera.arc = 65;
	
	d3d_fogcolor4.red = 123;
	d3d_fogcolor4.green = 174;
	d3d_fogcolor4.blue = 163;	
	
	camera.fog_start = 4096;
	camera.fog_end = 8192;
	fog_color = 4;
	
	sun_light = 100;
	vec_set(sun_color, vector(255,255,255));
	vec_set(ambient_color, vec_scale(vector(255,255,255), 0.25));
	sun_angle.pan = 45;
	sun_angle.tilt = 30;
	
	detail_size = 64;	
	
	level_load(NULL);
	
	ent_create("credits.hmp", vector(0, 0, -64), acCreditsTerrain);
	ent_create("acknex.mdl", nullvector, acCreditsSign);
	ent_createlayer("Sky_1+6.tga", SKY | CUBE, 2);
	
	credits_populate();
	
	var x = 0;
	var len = 0;
	
	while (media_playing(g_fhCreditsSong) > 0 && !key_q)
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
			
			if (key_q)
				break;
				
			wait(1);
		}
		
		for (fade = 0; fade < 100; fade += fadespeed * time_step)
		{
			creditsHead2.pos_x = screen_size.x + size2 - 1.2 * size2 * (1 - pow(1 - 0.01 * fade, 2));
			creditsBody2.pos_x = screen_size.x + size2 - 1.3 * size2 * (1 - pow(1 - 0.01 * fade, 2));
			
			if(key_q)
				break;
				
			wait(1);
		}
		
		if(key_q)
			break;
			
		wait(-0.6 * itemDuration);
		
		var scrollspeed = 100;
		
		for (fade = 0; fade < 100; fade += fadespeed * time_step)
		{
			creditsHead1.pos_x += scrollspeed * time_step;
			
			if (fade > 20)
				creditsBody1.pos_x += scrollspeed * time_step;
				
			if (key_q)
				break;
				
			wait(1);
		}
		
		for (fade = 0; fade < 100; fade += fadespeed * time_step)
		{
			creditsHead2.pos_x -= scrollspeed * time_step;
			
			if (fade > 20)
				creditsBody2.pos_x -= scrollspeed * time_step;
				
			if(key_q)
				break;
				
			wait(1);
		}
		
		pos += 1;
		
		wait(1);
	} 
	
	media_stop(g_fhCreditsSong);
	
	creditsHead1.flags &= ~SHOW;
	creditsBody1.flags &= ~SHOW;
	creditsHead2.flags &= ~SHOW;
	creditsBody2.flags &= ~SHOW;
	
	//currentRow = 25000;
	g_bCreditsAllExplode = true;
	
	creditsFinished = 1;
	
	wait(-5.0);	
	
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
	
	while (!creditsFinished)
	{
		VECTOR pos;
		
		pos.x = 256 + 128 * g_crRow;
		pos.y = 128 * column - 64 * g_crRow;
		pos.z = 8;
		
		ENTITY* e = ent_create(CR_LOTTI_FILE, pos, credits_lotti);
		
		e->pan = 170 + random(20);
		e->flags |= TRANSLUCENT;
		
		for (e->alpha = 0; e->alpha < 100; e->alpha += fadespeed * time_step)
		{
			camera.z = cambase + camspeed * (g_crRow) + camspeed * ((var)(column + 0.005 * e->alpha) / (g_crRow+1));
			camera.x = -camspeed * (g_crRow) - camspeed * ((var)(column + 0.005 * e->alpha) / (g_crRow+1));
			
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
			camera.x = -camspeed * (g_crRow) - camspeed * ((var)(column + 0.5 + 0.005 * i) / (g_crRow+1));
			
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
		g_vecCreditsCamShake.x += strength - random(strength * 2);
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
	while (g_entCreditsTerrain == NULL)
		wait(1);
		
	vec_for_vertex(my->x, g_entCreditsTerrain, 493);
	
	my->flags |= NOFOG;
	my->material = creditsBlinker;	
}

BOOL crLottiUpdateRaise (ENTITY* e)
{
	e->z = e->alpha - 125;
	e->pan = 5.4 * e->alpha;
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