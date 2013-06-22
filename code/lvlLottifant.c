#ifndef lvlLottifant_c
#define lvlLottifant_c

#include "lvlLottifant.h"

void lvlLfInit ()
{
	lvlLfReset();
	lvlLfStart();
}

#define LVL_LOTTIFANT_PX_GROUP_ALL 1
#define LVL_LOTTIFANT_PX_GROUP_BALL 3
#define LVL_LOTTIFANT_PX_GROUP_ROCKPIECE 4

void lvlLfReset ()
{
	if (g_fhLvlLottifantSong != 0)
	{
		media_stop(g_fhLvlLottifantSong);
		g_fhLvlLottifantSong = 0;
	}
	
	level_load(NULL);
	
	if (g_bbLfCollisionDust == NULL)
		g_bbLfCollisionDust = newBmapBank(7, "collisionDust%d.tga");
	
	freeze_mode = 0;
	sky_active = 0;
}

void lvlLfStart ()
{
	while (total_frames < 1)
		wait(1);
		
	skychange();
		
	// start music
	{
		if (g_fhLvlLottifantSong != 0)
			media_stop(g_fhLvlLottifantSong);
		
		g_fhLvlLottifantSong = media_loop(LVL_LOTTIFANT_MUSIC, NULL, 100);
	}
	
	fog_color = 0;
	camera.arc = g_lvlLottifantCamArc;
	
	level_load(LVL_LOTTIFANT_WMB);
}

void lvlLfExit ()
{
	lvlLfReset();
	// <start next level>
}

ENTITY* g_entLvlLfBall = NULL;
VECTOR g_vecEntLvlBallLast;

void lvlLfBallEv ()
{
	if (g_entLvlLfBall != NULL)
	{
		VECTOR vecForce;
		vec_set(&vecForce, vector(0, 0, 1000));
		
		VECTOR vecPosExplo;
		vec_set(&vecPosExplo, g_entLvlLfBall->x);
		vec_sub(&vecPosExplo, vector(150,0,50));
		
		pXent_addexplosion(g_entLvlLfBall, &vecPosExplo, 1000, 500); 
	}
}

action lvlLfBall ()
{
	g_entLvlLfBall = my;
	
	//on_b = lvlLfBallEv;
	
	//while (!key_space)
	//	wait(1);

	pXent_settype(my, PH_RIGID, PH_SPHERE);
	pXent_setgroup(my, LVL_LOTTIFANT_PX_GROUP_BALL);	
	
	pXent_setelasticity(my, 0); 
	
	pXent_setangvelocity(my, vector(0, 360, 0)); 
	pXent_setvelocity(my, vector(100, 0, 25));
	
	pXent_setbodyflag(my, NX_BF_FROZEN_POS_Y, 1);
	pXent_setbodyflag(my, NX_BF_FROZEN_PAN, 1);
	pXent_setbodyflag(my, NX_BF_FROZEN_ROLL, 1);
	
	pXent_setmaxspeed(my, 180);
	
	pXent_setmass(my, 200);

	while (1)
	{
		cameraMove(my, g_lvlLottifantCamDist, g_lvlLottifantCamArc);
		
		VECTOR vecPos;
		vec_set(&vecPos, my->x);
		vecPos.y = 0;
		pXent_setposition(my, &vecPos);
		
		vec_set(&g_vecEntLvlBallLast, my->x);
		
		wait(1);
	}
}

action lvlLfPiece ()
{
	my->flags |= POLYGON;
	pXent_settype(my, PH_STATIC, PH_POLY);
	
	while (g_entLvlLfBall == NULL)
		wait(1);
		
	while (!(my->x < g_entLvlLfBall->x && (my.eflags&CLIPPED)))
		wait(1);
		
	pXent_settype(my, 0, 0);
	ptr_remove(my);	
}

void item_particleFaderZUZU (PARTICLE *p) 
{
	p->alpha -= 5 * time_step;
	p->size += time_step;
	if (p->alpha <= 0) 
	{
		p->lifespan = 0;
	}
}

void item_particle2 (PARTICLE *p) 
{
	//vec_sub(&p->blue, vector(random(32), random(32), random(32)));
	set(p, MOVE | TRANSLUCENT);

	p->bmap = getRandomBmapBankBmap(g_bbLfCollisionDust);
	
	p->size  = 32 + random(128);
	p->vel_z = -4-random(4);
	p->vel_y = 4 - random(8);	
	
	p->alpha = 25+random(75);
	p->lifespan = 100;
	
	p->event = item_particleFaderZUZU;
}

void lvlLfRockpiece ()
{
	vec_set(my->pan, vector(random(360), random(360), random(360)));
	
	wait(1);
	
	pXent_settype(my, PH_RIGID, PH_BOX);
	pXent_setgroup(my, LVL_LOTTIFANT_PX_GROUP_BALL); 
	
	pXent_setelasticity(my, 25);
	pXent_setfriction(my, 25);
	pXent_setdamping(my, 10, 20); 
	pXent_setskinwidth(my, 16);
	
	pXent_addexplosion(my, &g_vecEntLvlBallLast, 275, 500); 
	
	int numVertices = ent_status(my, 0);
	
	double timer = 0;

	while (!(my.eflags&CLIPPED))
	{
		if (timer < 0)
		{
			timer += 2;
			
			static VECTOR vecVertex;
			vec_for_vertex(&vecVertex, my, numVertices);
			effect(item_particle2, 1, &vecVertex, nullvector);
		}
		else
			timer -= time_step;
		
		wait(1);
	}
		
	pXent_settype(my, 0, 0);
	ptr_remove(my);
}

void lvlLfRockExplode (VECTOR* vecPos, double s, double r, int p)
{
	int i;	
	for (i = 0; i < p; i++)
	{
		int index = (int)(random(2));
		
		static char strFile [32];
		sprintf(strFile, "rockpiece%d.mdl", index);
		
		static VECTOR vecAdd;
		vec_set(&vecAdd, vector(random(r), 0, 0));
		vec_rotate(&vecAdd, vector(random(360), random(360), random(360)));
		vec_add(&vecAdd, vecPos);
		
		ENTITY* e = ent_create(strFile, &vecAdd, lvlLfRockpiece);
		vec_scale(e->scale_x, s);
	}
}

action lvlLfRock ()
{
	vec_set(my->pan, vector(random(360), random(360), random(360)));

	c_setminmax(my);
	pXent_settype(my, PH_STATIC, PH_POLY);
	
	while (g_entLvlLfBall == NULL)
		wait(1);
		
	var radiusBall = g_entLvlLfBall->max_x;
	var radiusMy = 1.5 * ((my->max_x + my->max_y + my->max_z) / 3);
	
	while (vec_dist(g_entLvlLfBall->x, my->x) > (radiusBall + radiusMy))
		wait(1);
	
	pXent_settype(my, 0, 0);
	lvlLfRockExplode(my->x, my->scale_x * 2, radiusMy, 5 + random(5));
	
	var fh = ent_playsound(my, g_sndLvlLfRockBreak, 5000);
	snd_tune(fh, 0, 100 + (20 - random(40)), 0);
	
	set(my, PASSABLE | INVISIBLE);
	
	while (snd_playing(fh))
		wait(1);
	
	ptr_remove(my);
}

action lvlLfStartRock ()
{
	lvlLfRock();
}

action lvlLfRockBoost ()
{
	set(my, PASSABLE | INVISIBLE); //TRANSLUCENT);
	
	while (g_entLvlLfBall == NULL)
		wait(1);
		
	var radiusBall = g_entLvlLfBall->max_x;
	var radiusMy = 1.5 * ((my->max_x + my->max_y + my->max_z) / 3);
	
	while (vec_dist(g_entLvlLfBall->x, my->x) > (radiusBall + radiusMy))
		wait(1);
		
	lvlLfBallEv();
	
	wait(1);
		
	ptr_remove(my);
}

// physx fix
function physX_ent_remove(ENTITY* ent)
{
	if (!NxPhysicsSDK) return;
	on_ent_remove_px(ent);
}

action lvlLfCloud ()
{
	set(my, PASSABLE | UNLIT);
	my->material = mtl_unlit;
	my->albedo = my->ambient = 255;
	
	vec_scale(my->scale_x, 2.5);
	
	var s = random(5);
	
	while (1)
	{
		my->x -= s * time_step;
		wait(1);
	}
}

#endif /* lvlLottifant_c */