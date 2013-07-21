#ifndef lvlLavastage_c
#define lvlLavastage_c

#include "lvlLavastage.h"
#include "postprocessing.h"
#include "endboss.h"

void lvlLavastage_c_startup ()
{
	g_bbankLavaAsh = newBmapBank(4, "pLavaAsh%d.dds");
	g_bbankLavaFire = newBmapBank(4, "lavafire%d.tga");
}

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
	
	godmode = 0;
	
	if (!g_lvlLavastageKeepSwirl)
	{
		g_lvlLavastageSwirlRadius = 1;
		g_lvlLavastageSwirlAngle = 4;
		g_lvlLavastageSwirlBlend = 1;		
	}
	else
		g_lvlLavastageKeepSwirl = false;
	
	level_load(NULL);
	sky_active = 0;
}

void lvlLavastageFog ()
{
	camera.fog_start = 0;
	camera.fog_end = 17500;
	
	setPpSwirl(0, 0, 0.5, 0.5, 0);
	
	fog_color = 1;
	vec_set(&d3d_fogcolor1.blue, vector(149, 97, 91));
	
	VECTOR fogColor;
	vec_set(&fogColor, vector(d3d_fogcolor1.blue, d3d_fogcolor1.green, d3d_fogcolor1.red));
	
	while (g_bLvlLavastageRunning)
	{
		vec_lerp(d3d_fogcolor1.blue, &fogColor, &sky_color, 0.5);
		wait(1);
	}
}

void lvlLavastageSwirlOut ()
{
	var blendTarget = 0.1;
	
	while (g_lvlLavastageSwirlAngle > 0.01 && g_bLvlLavastageRunning)
	{
		accelerate(&g_lvlLavastageSwirlRadius, 0, 0.05);
		g_lvlLavastageSwirlRadius = maxv(0.4, g_lvlLavastageSwirlRadius);
		
		accelerate(&g_lvlLavastageSwirlAngle, 0, 0.1);
		g_lvlLavastageSwirlAngle = maxv(0, g_lvlLavastageSwirlAngle);
		
		accelerate(&g_lvlLavastageSwirlBlend, 0, 0.05);
		g_lvlLavastageSwirlBlend = maxv(blendTarget, g_lvlLavastageSwirlBlend);
		
		setPpSwirl(g_lvlLavastageSwirlRadius, g_lvlLavastageSwirlAngle, 0.5, 0.5, g_lvlLavastageSwirlBlend);
		
		wait(1);
	}
	
	double t = 0;
	double a = g_lvlLavastageSwirlAngle;
	
	g_lvlLavastageSwirlRadius = 0.75;
	
	while (g_bLvlLavastageRunning)
	{	
		a = (double)(-sin(t / 16) * 0.05);
		setPpSwirl(g_lvlLavastageSwirlRadius, a, 0.5, 0.5, g_lvlLavastageSwirlBlend);
		
		t += time_step * 0.75;
		
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
	
	level_load(LVL_LAVASTAGE_WMB);
	
	lvlLavastageSwirlOut();
	
	ebStart();
	
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

action acLavaFire ()
{
	var tReset = 0.1;
	var t = 0;
	
	int numVertices = ent_status(my, 0);
	
	set(my, PASSABLE | INVISIBLE);
	
	while (1)
	{
		if (t <= 0)
		{
			VECTOR pos;
			int vertex;
			
			vertex = (int)random(numVertices) + 1;
			vec_for_vertex(&pos, my, vertex);
			effLavaFire(&pos);
			
			if (random(100) < 5)
				effLavaAsh(&pos);
				
			t += tReset;
		}
		
		t -= time_step;
			
		wait(1);
	}
}

void effLavaFire (VECTOR* pos)
{
	effect(effLavaFire_p, 1, pos, nullvector);
}

	void effLavaFire_p (PARTICLE* p)
	{
		p->bmap = getRandomBmapBankBmap(g_bbankLavaFire);
		
		p->flags |= (TRANSLUCENT);

		p->alpha = 0;
		p->size = 64 + random(128);
		
		p->skill_b = 5 + random(5);
		p->skill_c = 5 + random(10);
		
		var d = p->y - camera->y;
		p->skill_d = 100 - clamp(d / (camera->fog_end - camera->fog_start) * 100, 0, 100);
		
		p->event = effLavaFire_ev;
	}
	
		void effLavaFire_ev (PARTICLE* p)
		{
			switch ((int)p->skill_a)
			{
				case 0:   p->alpha = clamp(p->alpha + p->skill_b * time_step, 0, p->skill_d);
				          p->size += 1 * time_step;
				          
						  if (p->alpha >= p->skill_d-5)
						     p->skill_a += 1;
				          
						  break;
						  
				case 1:   p->alpha = clamp(p->alpha - p->skill_c * time_step, 0, p->skill_d);
				          p->size -= 1 * time_step;
				          
						  if (p->alpha <= 5)
						     p->skill_a += 1;
							 
				          break;
						  
				case 2:   p->lifespan = 0;
			}
			
			p->z += 1 * time_step;
			
			var d = clamp((p->y - camera->y) / (camera->fog_end - camera->fog_start), 0, 1);
			vec_lerp(p->blue, vector(128,128,128), d3d_fogcolor1.blue, d);
		}	

void effLavaAsh (VECTOR* pos)
{
	effect(effLavaAsh_p, 1, pos, nullvector);
}
	
	void effLavaAsh_p (PARTICLE* p)
	{
		p->x += 16 - random(32);
		p->y += 16 - random(32);

		p->bmap = getRandomBmapBankBmap(g_bbankLavaAsh);
		
		p->flags |= (TRANSLUCENT);

		p->alpha = 50 + random(50);
		p->size = 8 + random(16);
		
		if (random(100) < 33)
			p->flags |= (BRIGHT);

		p->skill_a = random(360);
		p->skill_b = random(3);

		p->event = effLavaAsh_ev;
	}

		void effLavaAsh_ev (PARTICLE* p)
		{
			p->alpha -= 1 * time_step;
			if (p->alpha < 0) p->alpha = 0;
			
			p->z += 10 * time_step;
			p->x += sin(total_ticks * 2 + p->skill_a) * p->skill_b;
			p->lifespan = (p->alpha > 0) * 100;
		}
		
action acLavaEmitGrid ()
{
	var tReset = 0.5; //8;
	var t = 0;
	int numEmit = 20;
	
	int numVertices = ent_status(my, 0);
	
	set(my, PASSABLE | INVISIBLE);
	
	while (1)
	{
		if (t <= 0)
		{
			BOOL bEmitted = false;
			VECTOR pos, posScreen;
			int vertex, emitted = 0;
			
			while (emitted < numEmit)
			{
				while (!bEmitted)
				{
					vertex = (int)random(numVertices) + 1;
					vec_for_vertex(&pos, my, vertex);
			
					vec_set(&posScreen, &pos);
					if (vec_to_screen(&posScreen, camera) != NULL)
					{
						effLavaAsh(&pos);
						t += tReset;
						bEmitted = true;
					}
				}
				
				emitted++;
			}
			
			// steam
		}
		else
			t -= time_step;
			
		wait(1);
	}
}

#endif /* lvlLavastage_c */