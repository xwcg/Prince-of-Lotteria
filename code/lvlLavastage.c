#ifndef lvlLavastage_c
#define lvlLavastage_c

#include "lvlLavastage.h"

void lvlLavastageInit ()
{
	lvlLavastageReset();
	lvlLavastageStart();
}

void lvlLavastageReset ()
{
	snd_stop(g_fhLvlLavastageSong);
	g_fhLvlLavastageSong = 0;
	flying_man = 0;
	
	g_bLvlLavastageRunning = false;
	
	level_load(NULL);
	freeze_mode = 0;
	sky_active = 0;
}

void lvlLavastageFog ()
{
	camera.fog_start = 0;
	camera.fog_end = 17500;
	fog_color = 1;
	
	VECTOR fogColor;
	vec_set(&fogColor, vector(d3d_fogcolor1.blue, d3d_fogcolor1.green, d3d_fogcolor1.red));
	
	while (g_bLvlLavastageRunning)
	{
		vec_lerp(d3d_fogcolor1.blue, &fogColor, &sky_color, 0.5);
		wait(1);
	}
}

void lvlLavastageStart ()
{
	while (total_frames < 1)
		wait(1);
		
	skychange();
	
	g_bLvlLavastageRunning = true;
	
	gui_show();
	
	// start music
	g_fhLvlLavastageSong = snd_loop(g_musicLavastage, 100, 0);	
	
	lvlLavastageFog();
	
	// set camera + player walk glide
	setPlayerCamera(g_lvlLavastageCamDist, g_lvlLavastageCamRaiseZ, g_lvlLavastageCamArc);
	setPlayerWalkGlide(false);
	
	level_load_ext(LVL_LAVASTAGE_WMB);
	
	// debug
	{
		while (key_a || key_c || key_k)
			wait(1);
		
		while (1)
		{
			if (key_a && key_c && key_k)
				break;
			
			wait(1);
		}
		
		lvlLavastageExit(true);
	}
}

void lvlLavastageExit (BOOL bNextLevel)
{
	sky_active = 0;
	g_bLvlLavastageRunning = false;
	
	snd_stop(g_fhLvlLavastageSong);
	g_fhLvlLavastageSong = 0;
	
	player = NULL;
	
	lvlLavastageReset();
	
	level_load(NULL);
	
	gui_hide();
	
	if (bNextLevel)
	{
		achievement("lavastage");	
		creditsInit();
	}
	else
		menu_open();
}

action acLsPlane ()
{
	initFxLavaPlane(my, 12, vector(0,-0.05,0), 0.005, 0.01, 100);
	
	while (1)
	{
		if (player != NULL)
		{
			if (player->z <= 16)
			{
				setPlayerDead();
				break;
			}
		}
		
		wait(1);
	}
}

void assertMtlLavaPlane (ENTITY* e)
{
	mtlLavaPlane->event = mtlLavaPlaneEv;
	mtlLavaPlane->flags |= ENABLE_RENDER;

	mtlLavaPlane->skin1 = bmap_for_entity(e, 5);

	mtlLavaPlane->emissive_red   = 255;
	mtlLavaPlane->emissive_green = 200;
	mtlLavaPlane->emissive_blue  = 0;

	mtlLavaPlane->specular_red   = 128;
	mtlLavaPlane->specular_green = 64;
	mtlLavaPlane->specular_blue  = 32;
	
	e->material = mtlLavaPlane;
}	

void initFxLavaPlane (ENTITY* e, float tile, VECTOR* uv, float turbAmplitude, float turbSpeed, float turbModulation)
{
	assertMtlLavaPlane(e);

	set(e, PASS_SOLID);

	e->skill41 = floatv(tile); // .x tile factor

	e->skill42 = floatv(0);    // .yz uv shift
	e->skill43 = floatv(0);

	e->skill45 = floatv(turbAmplitude);  // .x amplitude
	e->skill46 = floatv(turbSpeed);      // .y speed
	e->skill47 = floatv(turbModulation); // .z modulation

	e->skill1 = uv.x;
	e->skill2 = uv.y;
}

void mtlLavaPlaneEv ()
{
	assertMtlLavaPlane(my);

	if (mtl->skill1 < total_frames)
	{
		mtl->skill1 = total_frames;
		mtl->skill2 += time_step;
	}

	my->skill42 = floatd(mtl->skill2 * my->skill1, mtl->skin1->width);
	my->skill43 = floatd(mtl->skill2 * my->skill2, mtl->skin1->height);
}   

#endif /* lvlLavastage_c */