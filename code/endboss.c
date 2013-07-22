#ifndef endboss_c
#define endboss_c

#include "endboss.h"
#include "lvlBoss.h"

void ebReset ()
{
	int i, j;
	
	for (i = 0; i < HAND_HANDS; i++)
		for (j = 0; j < HAND_FINGERS; j++)
			(g_handChop[i])[j] = false;
}

void ebStart ()
{
	ent_create(NULL, nullvector, ebStartSpeech);
}

void ebDoSparkle (ENTITY* e, int modulo)
{
	int i, numVertices = ent_status(e, 0);
	
	for (i = 0; i < numVertices; i++)
	{
		if (i % modulo == 0)
		{
			VECTOR pos;
			vec_for_vertex(&pos, e, i);
			
			effect(bossSparkle_p, 1, &pos, nullvector);
		}
	}
}

void ebStartSpeech ()
{
	var t = 3 * 16;
	
	while (t > 0)
	{
		t -= time_step;
		wait(1);
	}

	var h = snd_play(g_sndWarghostBattleSpeechStart, 100, 0);
	
	while (snd_playing(h))
		wait(1);
		
	ent_create(NULL, nullvector, ebHandsBgFly);
	ptr_remove(my);
}

void ebHandsBgFly ()
{
	var handRadius = 500;
	
	snd_play(g_sndSparkle, 100, 0);

	ENTITY* hands [2];
	hands[0] = ent_create("warhand.mdl", vector(-handRadius, 500, 400), ebHandFlyL);
	hands[1] = ent_create("warhand.mdl", vector(handRadius, 500, 400), ebHandFlyR);
	
	var t = (5 + random(5)) * 16;
	
	while (t > 0)
	{
		t -= time_step;
		wait(1);
	}
	
	snd_play(g_sndWarghostBattleNoChance, 100, 0);
	
	int i;
	var m = (1 + random(2)) * 16;
	for (i = 0; i < 2; i++)
	{
		(hands[i])->skill1 = 1;
		(hands[i])->skill2 = m;
	}
	
	while (m > 0)
	{
		m -= time_step;
		wait(1);
	}
	
	snd_play(g_sndSparkle, 100, 0);
	
	wait(-(0.5+random(1)));
	
	VECTOR v;
	vec_set(&v, player->x);
	v.z += 400;
	
	ent_create("warhand.mdl", &v, ebHandWatch);
	ptr_remove(my);
}

	void ebHandFlyL ()
	{
		my->material = g_mtlHandL;
		ebHandFly();
	}

	void ebHandFlyR ()
	{
		my->material = g_mtlHandR;
		ebHandFly();
	}
	
		void ebHandFly ()
		{
			set(my, PASSABLE);
			my->ambient = 75;
			
			VECTOR startPos, targetPos;
			vec_set(&startPos, my->x);
			
			ebDoSparkle(my, 2000);
			
			var t = 0;
			
			var distX = 500;
			
			var moveDist = 200;
			var moveTime = 0.5;
			var moveLerp = 0.2;
			var moveAnim = 2;
			
			vec_scale(my->scale_x, g_handScale);
			
			while (my->skill1 == 0)
			{
				if (t <= 0)
				{
					vec_set(&targetPos, vector(25+random(moveDist), 0, 0));
					vec_rotate(&targetPos, vector(random(360),random(360),random(360)));
					vec_add(&targetPos, &startPos);
					
					t += moveTime * 16;
				}
				else
					t -= time_step;
				
				VECTOR nuPos;
				vec_set(&nuPos, &targetPos);
				
				if (player != NULL)
					nuPos.x += player->x;
				
				vec_lerp(my->x, my->x, &nuPos, moveLerp * time_step);
					
				ent_animate(my, "idleH", (total_ticks * moveAnim) % 100, ANM_CYCLE);
				my->roll += (-90 - my->roll) * 0.05 * clamp(time_step, 0.001, 1);
				
				wait(1);
			}
			
			while (my->skill2 > 0)
			{
				my->skill2 -= time_step;
				my->z += 10 * time_step;
				my->y -= 20 * time_step;
				my->roll += 5 * time_step;
				wait(1);
			}
			
			set(my, INVISIBLE);
			ebDoSparkle(my, 2000);
			
			wait(1);
			ptr_remove(my);
		}

void ebHandWatch ()
{
	ebDoSparkle(my, 2000);
	snd_play(g_sndSparkle, 100, 0);
	
	set(my, PASSABLE);
	my->ambient = 75;
	
	my->material = g_mtlHandR;
	
	var moveAnim = 10;
	
	var t = (5 + random(3)) * 16;
	
	vec_scale(my->scale_x, g_handScale);
	
	var ground, height;
	var heightSub = 32;
	
	while (t > 0)
	{
		VECTOR vecBoneGround, vecTraceStart, vecTraceEnd;
		vec_for_bone(&vecBoneGround, my, "ground");
		
		vec_set(&vecTraceStart, my->x);
		vecTraceStart.y = vecBoneGround.y;
		
		vec_set(&vecTraceEnd, &vecTraceStart);
		vecTraceEnd.z -= 2000;
		
		height = player->z + 250;
		
		you = player;
		if (c_trace(&vecTraceStart, &vecTraceEnd, IGNORE_ME | IGNORE_YOU | USE_POLYGON) > 0)
		{
			ground = hit.z;
			height = ground + (my->z - vecBoneGround.z);
		}
		
		VECTOR v;
		vec_set(&v, player->x);
		v.z = height;
		
		vec_lerp(my->x, my->x, &v, 0.25 * time_step);
		
		ent_animate(my, "idleH", 50 + sin(total_ticks * moveAnim) * 50, ANM_CYCLE);
		
		t -= time_step;
		
		wait(1);
	}
	
	t = 0;
	
	while (t < 100)
	{
		VECTOR vecBoneGround;
		vec_for_bone(&vecBoneGround, my, "ground");
		height = ground + (my->z - vecBoneGround.z) - heightSub;
		
		VECTOR v;
		vec_set(&v, my->x);
		v.z = height;
		
		vec_lerp(my->x, my->x, &v, 0.25 * time_step);
		
		t = clamp(t + 5 * time_step, 0, 100);
		ent_animate(my, "drop", t, 0);		
		wait(1);
	}
	
	t = (3+random(2)) * 16;
	
	reset(my, PASSABLE);
	set(my, POLYGON);
	
	c_updatehull(my, 0);
	
	while (t > 0)
	{
		ent_animate(my, "down", (total_ticks * 4), ANM_CYCLE);
		t -= time_step;
		wait(1);
	}
	
	set(my, PASSABLE);
	reset(my, POLYGON);
	
	while (t < 100)
	{
		VECTOR vecBoneGround;
		vec_for_bone(&vecBoneGround, my, "ground");
		height = ground + (my->z - vecBoneGround.z) - heightSub;
		
		VECTOR v;
		vec_set(&v, my->x);
		v.z = height;
		
		vec_lerp(my->x, my->x, &v, 0.25 * time_step);
		
		t = clamp(t + 2 * time_step, 0, 100);
		ent_animate(my, "up", t, 0);		
		wait(1);
	}
	
	t = 1.5 * 16;
	
	while (t > 0)
	{
		t -= time_step;
		my->z += 10 * time_step;
		my->y -= 20 * time_step;
		my->roll += 5 * time_step;
		wait(1);
	}

	set(my, INVISIBLE);
	ebDoSparkle(my, 2000);
	snd_play(g_sndSparkle, 100, 0);

	wait(1);
	ptr_remove(my);	
}

#endif /* endboss_c */