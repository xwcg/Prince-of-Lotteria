#ifndef lvlLottifant_c
#define lvlLottifant_c

#include "lvlLottifant.h"
#include "lvlBoss.h"

void lvlLfInit ()
{
	lvlLfReset();
	lvlLfStart();
}

void on_exit_lotti ()
{
	on_ent_remove = NULL;
}

void lvlLfReset ()
{
	// physx hack
	{
		if (on_exit_restore == NULL)
			on_exit_restore = on_exit;	
			
		on_exit = on_exit_lotti;
	}
	
	snd_stop(g_fhLvlLottifantSong);
	g_fhLvlLottifantSong = 0;
	
	vPlayerWoho = false;
	
	level_load(NULL);
	sky_active = 0;
	
	if (g_bbLfCollisionDust == NULL)
		g_bbLfCollisionDust = newBmapBank(7, "collisionDust%d.tga");
		
	pX_setgravity(vector(0,0,-g_lvlLfGravity));
	
	sky_active = 0;
}

void lvlLfStart ()
{
	while (total_frames < 1)
		wait(1);
		
	freeze_mode = 1;
	physX_open();
	wait(1);
	freeze_mode = 0;
	
		
	skychange();
	setHdr(LVL_LOTTIFANT_HDR_STRENGTH, LVL_LOTTIFANT_HDR_THRESHOLD, LVL_LOTTIFANT_HDR_EXPOSURE);
	
	gui_show();
		
	// start music
	g_fhLvlLottifantSong = snd_loop(g_musicLottifant, 100, 0);
	
	fog_color = 0;
	camera.arc = g_lvlLottifantCamArc;
	
	level_load(LVL_LOTTIFANT_WMB);
}

void lvlLfExit (BOOL bNextLevel)
{
	g_lvlLfDeregister = true;
	on_exit = on_exit_restore;
	
	wait(1);
	
	sky_active = 0;
	
	snd_stop(vGameMusicHandle);
	vGameMusicHandle = 0;
	
	player = NULL;
	
	lvlLfReset();
	
	if (g_fhLvlLottifantSong != 0)
	{
		snd_stop(g_fhLvlLottifantSong);
		g_fhLvlLottifantSong = 0;
	}
	
	g_lvlLfDeregister = false;
	
	wait(1);
	
	physX_close();
	
	level_load("");
	
	wait(1);
	
	if (bNextLevel)
	{
		achievement("lottifantlevel");	
		lvlBossInit();
	}
	else
	{
		menu_open();
		gui_hide();
	}
}

void lvlLfBallEv ()
{
	if (g_entLvlLfBall != NULL)
	{
		
		//VECTOR vecForce;
		//vec_set(&vecForce, vector(0, 0, 1000));
		
		VECTOR vecPosExplo;
		vec_set(&vecPosExplo, g_entLvlLfBall->x);
		vec_sub(&vecPosExplo, vector(100,0,0));
		
		pXent_addexplosion(g_entLvlLfBall, &vecPosExplo, 550, 500); 
	}
}

action lvlLfBall ()
{
	g_entLvlLfBall = my;
	
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
	
	pXent_setmaxspeed(my, g_lvlLfBallSpeedMax);
	
	pXent_setmass(my, 200);

	while (!g_lvlLfDeregister)
	{
		//cameraMove(my, g_lvlLottifantCamDist, g_lvlLottifantCamArc);
		
		//lvlLfBallEv()
		
		VECTOR vecPos;
		vec_set(&vecPos, my->x);
		vecPos.y = 0;
		pXent_setposition(my, &vecPos);
		
		var d = vec_dist(&g_vecEntLvlBallLast, my->x) / time_step;
		if (d < 2 && my->x < 23500)
			lvlLfBallEv();
			
		pXent_addforceglobal(my, vector(0, 0, -g_lvlLfBallDownForce * time_step), my->x);
		
		vec_set(&g_vecEntLvlBallLast, my->x);
		
		wait(1);
	}
	
	pXent_settype(my, 0, 0);
}

action lvlLfPiece ()
{
	my->flags |= POLYGON;
	pXent_settype(my, PH_STATIC, PH_POLY);
	
	while (g_entLvlLfBall == NULL)
		wait(1);
		
	while (!g_lvlLfDeregister)
		wait(1);
		
	pXent_settype(my, 0, 0);
}

void lvlLfRockpiecePart_ev (PARTICLE *p) 
{
	p->alpha -= 3 * time_step;
	p->size += time_step;
	if (p->alpha <= 0) 
	{
		p->lifespan = 0;
	}
}

void lvlLfRockpiecePart (PARTICLE *p) 
{
	set(p, MOVE | TRANSLUCENT);

	p->bmap = getRandomBmapBankBmap(g_bbLfCollisionDust);
	
	p->size  = 32 + random(128);
	p->vel_z = -4-random(4);
	p->vel_y = 4 - random(8);	
	
	p->alpha = 50+random(50);
	p->lifespan = 100;
	
	p->event = lvlLfRockpiecePart_ev;
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

	while (!(my.eflags&CLIPPED) && !g_lvlLfDeregister)
	{
		if (timer < 0)
		{
			timer += 2;
			
			static VECTOR vecVertex;
			vec_for_vertex(&vecVertex, my, numVertices);
			effect(lvlLfRockpiecePart, 1, &vecVertex, nullvector);
		}
		else
			timer -= time_step;
		
		wait(1);
	}
		
	pXent_settype(my, 0, 0);
	
	if (!g_lvlLfDeregister)
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
	pXent_settype(my, PH_STATIC, PH_SPHERE);
	
	while (g_entLvlLfBall == NULL)
		wait(1);
		
	var radiusBall = g_entLvlLfBall->max_x;
	var radiusMy = 1.5 * ((my->max_x + my->max_y + my->max_z) / 3);
	
	while (vec_dist(g_entLvlLfBall->x, my->x) > (radiusBall + radiusMy) && !g_lvlLfDeregister)
		wait(1);
		
	if (!g_lvlLfDeregister)
	{
		pXent_settype(my, 0, 0);
		lvlLfRockExplode(my->x, my->scale_x * 2, radiusMy, 5 + random(5));
		
		var fh = ent_playsound(my, g_sndLvlLfRockBreak, 5000);
		snd_tune(fh, 0, 100 + (20 - random(40)), 0);
		
		set(my, PASSABLE | INVISIBLE);
		
		while (snd_playing(fh))
			wait(1);
	}
	
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

action lvlLfCloud ()
{
	set(my, PASSABLE | UNLIT | NOFOG);
	
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

action lvlLfLottiDummy ()
{
	set(my, PASSABLE);
	
	double t = 0;
	double animfac = 0.5;
	
	while (my->skill4 == 0)
	{
		double animspeed = my->skill2 * animfac;
		t += (0.5 + animspeed) * time_step;
		
		ent_animate(my, "ride", (var)t % 100, ANM_CYCLE);		
		
		gui_update_hearts();
		gui_update_a4();		
		
		wait(1);
	}
	
	set(my, ZNEAR);
	
	int num = ent_status(my, 0);
	
	var t = 2 * 16;
	
	ent_playsound(my, sndDie, 2000);
	
	while (t > 0)
	{
		my->z -= 30 * time_step;
		my->y -= 15 * time_step;
		
		my->roll -= 10 * time_step;
		my->tilt += 5 * time_step;
		
		static VECTOR v;
		vec_for_vertex(&v, my, (int)random(num));
		effect(pBlood, 3, &v, vector(0,0,random(16)));
		
		t -= time_step;
		
		gui_update_hearts();
		gui_update_a4();		
		
		wait(1);
	}
	
	proc_mode = PROC_LATE;	
	g_lvlLfDeregister = true;
	
	wait(1);
	
	g_lvlLfDeregister = false;
	
	if (nPlayerLifes > 0)
	{
		nPlayerLifes--;
		game_restart();
	}
	else
	{
		set(txtGameOver, SHOW);
		
		wait(-2.5);
		
		set(txtGameOver, TRANSLUCENT);
		
		for(txtGameOver.alpha = 100; txtGameOver.alpha >= 0; txtGameOver.alpha -= time_step)
			wait(1);
			
		reset(txtGameOver, SHOW);
		reset(txtGameOver, TRANSLUCENT);
		
		txtGameOver.alpha = 100; //fix by firo
		
		lvlLfExit(false);
	}
}

void lvlLfLottifantJump ()
{
	VECTOR v;
	vec_set(&v, g_entLvlLfPlayer->x);
	v.z -= 200;
	
	pXent_addexplosion(g_entLvlLfPlayer, &v, g_lvlLfJumpForce, 250); 
	snd_player_jump();
}

ENTITY* g_entLfLottifant = NULL;

action lvlLfLottifantDummy ()
{
	set(my, PASSABLE);
	
	g_entLfLottifant = my;
	ENTITY* entLotti = ent_create(LF_LOTTI_FILE, my->x, lvlLfLottiDummy);
	
	double t = 0;
	double animfac = 2;	
	
	my->pan = 300;
	
	while (my->skill4 == 0)
	{
		proc_mode = PROC_LATE;
		
		var targetPan = 270 + my->skill1 * 90;
		my->pan += (targetPan - my->pan) * 0.2 * time_step;
		
		vec_for_bone(entLotti->x, my, "sit");		
		vec_set(entLotti->pan, my->pan);
		
		entLotti->skill2 = my->skill2;
		
		double animspeed = my->skill2 * animfac;
		t += (0.5 + animspeed) * time_step;
		
		if (my->skill3 > 0)
		{
			ent_animate(my, "jump", minv(my->skill3, 100), 0);
			
			if (my->skill3 > 100 && !vPlayerWoho)
			{
				vPlayerWoho = true;
				snd_player_wohoo();				
			}
		}
		else
		{
			ent_animate(my, "walk", (var)t % 100, ANM_CYCLE);
			vPlayerWoho = false;
		}
		
		wait(1);
	}
	
	entLotti->skill4 = 1;
	
	int i, num = ent_status(my, 0);
	for (i = 0; i < num; i++)
	{
		if (i % 3 == 0)
		{
			static VECTOR v;
			vec_for_vertex(&v, my, i);
			effect(pBlood, 1, &v, vector(0,0,random(16)));
		}
	}
	
	//ptr_remove(my);
	set(my, INVISIBLE);
}

action lvlLfLottifantRide ()
{
	g_entLvlLfPlayer = my;
	player = my;
	
	my->PL_HEALTH = 3;
	my->trigger_range = 100;
	
	while (g_entLvlLfBall == NULL)
		wait(1);

	pXent_settype(my, PH_RIGID, PH_SPHERE);
	pXent_setelasticity(my, 25); 
	
	pXent_setbodyflag(my, NX_BF_FROZEN_POS_Y, 1);
	pXent_setbodyflag(my, NX_BF_FROZEN_PAN, 1);
	pXent_setbodyflag(my, NX_BF_FROZEN_ROLL, 1);
	
	pXent_setfriction(my, 15); 
	pXent_setdamping(my, 50, 50);
	
	pXent_setmaxspeed(my, 180);
	
	pXent_setgroup(my, LVL_LOTTIFANT_PX_GROUP_PLAYER); 
	pX_setgroupcollision(LVL_LOTTIFANT_PX_GROUP_PLAYER, LVL_LOTTIFANT_PX_GROUP_ROCKPIECE, false);
	
	set(my, INVISIBLE);
	ENTITY* entLottifant = ent_create(my->type, my->x, lvlLfLottifantDummy);
	
	var direction = 1;
	
	var radiusBall = g_entLvlLfBall->max_x;
	var radiusMy = 1.5 * ((my->max_x + my->max_y + my->max_z) / 3);
	
	pXent_addforceglobal(my, vector(2 * g_lvlLfRideForce * time_step, 0, 0), my->x);
	
	var jumptimer = 0;
	
	while (my->skill4 == 0)
	{
		VECTOR* vec = pXent_raycast(my, vector(0,0,-99999), NX_STATIC_SHAPES, NX_RAYCAST_IMPACT);
		
		BOOL bInAir = true;
		BOOL bNearBall = false;
		
		if (g_entLvlLfBall != NULL)
			bNearBall = (vec_dist(g_entLvlLfBall->x, my->x) < (radiusBall + radiusMy));
		
		if (vec != NULL)
			bInAir = (vec_dist(vec, my->x) > 100);
		
		if ((!bInAir || bNearBall) && key_cuu)
			lvlLfLottifantJump();
			
		if (bInAir)
			jumptimer += 5 * time_step;
		else
			jumptimer = 0;
			
		var dist = vec_dist(vector(my->x,0,0), vector(entLottifant->x,0,0));
		var force = 1 + minv(key_force.x * 2, 0);
		
		if (dist / time_step < g_lvlLfSpeedMax) 
		{
			
			pXent_addforceglobal(my, vector(force * g_lvlLfRideForce * time_step, 0, 0), my->x);
		}
			
		pXent_addforceglobal(my, vector(0, 0, -g_lvlLfDownForce * time_step), my->x);
			
		cameraMove(my, g_lvlLottifantCamDist, 200, g_lvlLottifantCamArc);
		
		if (sign(force) != 0)
			direction = sign(force);
			
		entLottifant->skill1 = direction;
		entLottifant->skill2 = dist;
		entLottifant->skill3 = bInAir * jumptimer;
		
		vec_set(entLottifant->x, my->x);
		
		c_move(my, nullvector, vector(1,0,0), ACTIVATE_TRIGGER);
		
		wait(1);
	}
	
	entLottifant->skill4 = 1;
	
	pXent_settype(my, 0, 0);
	ptr_remove(my);
}

action lvlLfTrigger ()
{
	while (1)
	{
		if (g_entLvlLfPlayer != NULL)
		{
			if (vec_dist(vector(g_entLvlLfPlayer->x,0,0), vector(my->x,0,0)) < 300 || (key_a && key_c && key_k))
			{				
				break;
			}
		}
		
		wait(1);
	}
	
	snd_stopall(4);
	snd_play(g_sndDoor, 100, 0);
	lvlLfExit(true);
}

action lvlLfSpike ()
{
	while (g_entLvlLfPlayer == NULL)
		wait(1);
		
	var radiusPlayer = g_entLvlLfPlayer->max_x;
	var radiusMy = 1 * ((my->max_x + my->max_y + my->max_z) / 3);
	
	while (vec_dist(g_entLvlLfPlayer->x, my->x) > (radiusPlayer + radiusMy))
		wait(1);
		
	if (!godmode)
		g_entLvlLfPlayer->skill4 = 1;
}

#endif /* lvlLottifant_c */