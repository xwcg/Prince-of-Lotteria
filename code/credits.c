#ifndef _CREDITS_C_
#define _CREDITS_C_

#include "credits.h"

#include "ini.h"
#include <strio.c>
#include "menu.h"
#include "zorroMesh.h"

void creditsInit ()
{
	creditsReset();
	creditsStart();
}

void creditsReset ()
{
	creditsFinished = 0;
	currentRow = 0;
	
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
	
	freeze_mode = 0;
}

void creditsExit ()
{
	creditsReset();	
	menu_open();
}

void creditsStart ()
{
	while (total_frames < 1)
		wait(1);
		
	static char strIni [256];
	sprintf(strIni, "%s\\credits.ini", _chr(work_dir));
	
	// start music
	{
		if (g_fhCreditsSong != 0)
			media_stop(g_fhCreditsSong);
		
		g_fhCreditsSong = media_play("music\\CloneFall.ogg", NULL, 100);
	}
	
	int pos = 0;
	int count = ini_read_int(strIni, "Credits", "max", "0");
	var duration = ini_read_int(strIni, "Credits", "duration", "15");
	
	var itemDuration = duration / (var)count;
	
	creditsHead1.flags |= SHOW;
	creditsBody1.flags |= SHOW;
	creditsHead2.flags |= SHOW;
	creditsBody2.flags |= SHOW;
	
	creditsHead2.pos_y = screen_size.y - 150;
	creditsBody2.pos_y = screen_size.y - 100;
		
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

		ini_read(creditsText1Head, strIni, section, "Head1", "MISSING!");
		ini_read(creditsText1Body, strIni, section, "Body1", "MISSING!");
		ini_read(creditsText2Head, strIni, section, "Head2", "MISSING!");
		ini_read(creditsText2Body, strIni, section, "Body2", "MISSING!");
		
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
		
		var scrollspeed = 120;
		
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
	
	creditsFinished = 1;
	
	//currentRow = 25000;
	g_bCreditsAllExplode = true;
	
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
	int row = 0;
	int column = 0;
	
	var cambase = 64;
	var camspeed = 32;
	
	var fadespeed = 15;
	
	while (!creditsFinished)
	{
		VECTOR pos;
		
		pos.x = 256 + 128 * row;
		pos.y = 128 * column - 64 * row;
		pos.z = 8;
		
		camera.tilt = -17.5;
		
		ENTITY* e = ent_create("zorroLotti.mdl", pos, credits_lotti);
		
		e->pan = 170 + random(20);
		e->flags |= TRANSLUCENT;
		
		for (e->alpha = 0; e->alpha < 100; e->alpha += fadespeed * time_step)
		{
			camera.z = cambase + camspeed * (row) + camspeed * ((var)(column + 0.005 * e->alpha) / (row+1));
			camera.x = -camspeed * (row) - camspeed * ((var)(column + 0.005 * e->alpha) / (row+1));
			
			vec_add(camera->x, &g_vecCreditsCamShake);
			
			wait(1);
		}
		
		e->alpha = 100;
		
		var i;
		for (i = 0; i < 100; i += fadespeed * time_step)
		{
			camera.z = cambase + camspeed * (row) + camspeed * ((var)(column + 0.5 + 0.005 * i) / (row+1));
			camera.x = -camspeed * (row) - camspeed * ((var)(column + 0.5 + 0.005 * i) / (row+1));
			
			vec_add(camera->x, &g_vecCreditsCamShake);
			
			wait(1);
		}
		
		column += 1;
		
		if (column > row)
		{
			column = 0;
			row += 1;
		}
	}
}

void credits_fire (PARTICLE *p)
{
	p->z -= random(p->vel_z);
	
	p->vel_x = random(1) - 0.5;
	p->vel_y = random(1) - 0.5;
	p->vel_z = random(1) - 0.5;
	
	p->size = 4 + random(8);
	
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
	
	int strength = 15;
	
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

action credits_lotti ()
{
	BOOL bSwordAtBelt = (random(100) < 20);
	zorroMeshOptions(my, false, bSwordAtBelt, false);
	
	var init = random(100);
	
	while (my.alpha < 100)
	{
		my.z = my.alpha - 125;
		my.pan = 5.4 * my.alpha;
		wait(1);
	}

	reset(my, TRANSLUCENT);
	
	BOOL exploder = (random(100) < 20);
	
	var time = 64 + random(128);
	
	while (!g_bCreditsAllExplode && !(time < 0 && exploder))
	{
		ent_animate(me, "idle", 5 * total_ticks + init, ANM_CYCLE);
		time -= time_step;
		
		wait(1);
	}
	
	// show jetpack
	zorroMeshOptions(my, false, false, true);
	
	time = 8 + random(32);
	while (time > 0)
	{
		ent_animate(me, "attack", 5 * total_ticks + init, ANM_CYCLE);
		time -= time_step;
		wait(1);
	}
		
	VECTOR accel, speed;
	if (exploder)
	{
		accel.x = 2.5 * (random(1) - 0.5);
		accel.y = 2.5 * (random(1) - 0.5);
	}
	else
	{
		accel.x = 0.25 * (random(1) - 0.5);
		accel.y = 0.25 * (random(1) - 0.5);
	}
	
	accel.z = 1 + random(2);
	
	vec_to_angle(my.pan, accel);
	my.tilt -= 90;
	
	vec_zero(speed);
	
	time = 10 + random(12);
	
	var percent = random(0.2);
	var percent_inv = 1 - percent;
	
	while (1)
	{
		vec_add(speed, vector(accel.x * time_step, accel.y * time_step, accel.z * time_step));
		vec_add(my.x, vector(speed.x * time_step, speed.y * time_step, speed.z * time_step));
		
		VECTOR feet;
		
		feet.x = feet.y = 0;
		feet.z = 0.5 * my.min_z;
		
		vec_rotate(feet, my.pan);
		vec_add(feet, my.x);
		
		ent_animate(me, "drown", 5 * total_ticks + init, ANM_CYCLE);
		
		effect(credits_fire, 10, feet, speed);
		
		// explode near screen border
		{
			VECTOR v;
			vec_set(&v, my->x);
			
			vec_to_screen(&v, camera);
			
			if (v.x < percent * screen_size.x || v.x > percent_inv * screen_size.x || v.y < percent * screen_size.y || v.y > percent_inv * screen_size.y)
				break;
		}
		
		wait(1);
	}
	
	ent_create("explo+13.tga", my->x, explo_sprite);
	wait(1);
	
	ptr_remove(my);
}

void creditsSpace ()
{
	creditsExit();
}

#endif /* _CREDITS_C_ */