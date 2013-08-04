#ifndef endboss_c
#define endboss_c

#include "endboss.h"
#include "lvlBoss.h"

void ebDoHit ()
{
	g_ebDoHit = true;
	g_ebDoHitPercent = 0;
	
	while (g_ebDoHitPercent < 100)
	{
		proc_mode = PROC_GLOBAL;		
		
		g_ebDoHitPercent = clamp(g_ebDoHitPercent + time_step * 2, 0, 100);
		
		wait(1);
	}
	
	g_ebDoHit = false;
}

int numChoppedEnt (ENTITY* entHand)
{
	BOOL* arr = NULL;
	
	if (entHand->skill20 == 1) // left
		arr = g_handChopL;
	else // right
		arr = g_handChopR;
		
	int num = 0;
	
	int i;
	for (i = 0; i < 4; i++)
		if (arr[i] == true)
			num++;
			
	return num;
}

int numChoppedSide (int side)
{
	BOOL* arr = NULL;
	
	if (side == 1) // left
		arr = g_handChopL;
	else // right
		arr = g_handChopR;
		
	int num = 0;
	
	int i;
	for (i = 0; i < 4; i++)
		if (arr[i] == true)
			num++;
			
	return num;
}

VECTOR* handFingerPos (ENTITY* entHand, BOOL bLeftHand, int fingerId, VECTOR* vecFingerPos, VECTOR* vecFingerDir)
{
	VECTOR vecFinger0, vecFinger1;
	
	char* j0 = _chr((g_handBoneFarNames->pstring)[fingerId]);
	char* j1 = _chr((g_handBoneNearNames->pstring)[fingerId]);		
	
	vec_for_bone(&vecFinger0, entHand, j0);
	vec_for_bone(&vecFinger1, entHand, j1);
	
	if (bLeftHand)
	{
		vec_to_ent(&vecFinger0, entHand);
		vecFinger0.x *= -1;
		vec_for_ent(&vecFinger0, entHand);	
		
		vec_to_ent(&vecFinger1, entHand);
		vecFinger1.x *= -1;
		vec_for_ent(&vecFinger1, entHand);
	}	
	
	vec_lerp(vecFingerPos, &vecFinger0, &vecFinger1, 0.5);
	vec_diff(vecFingerDir, &vecFinger1, &vecFinger0);
	
	return vecFingerPos;
}

void doHandChopBlood (ENTITY* entHand)
{
	BOOL* arr = NULL;
	
	BOOL bLeftHand = (entHand->skill20 == 1);
	
	if (bLeftHand) // left
		arr = g_handChopL;
	else // right
		arr = g_handChopR;

	int fingerId;
	VECTOR fingerPos, fingerDir;
	
	for (fingerId = 0; fingerId < 4; fingerId++)
	{
		if (arr[fingerId] == true)
		{
			handFingerPos(entHand, bLeftHand, fingerId, &fingerPos, &fingerDir);
			effEbBlood(&fingerPos, &fingerDir, 25, false);
		}			
	}
}

void updateHandChopped (ENTITY* entHand)
{
	BOOL* arr = NULL;
	
	BOOL bLeftHand = (entHand->skill20 == 1);
	
	if (bLeftHand) // left
		arr = g_handChopL;
	else // right
		arr = g_handChopR;
	
	int fingerId;
	for (fingerId = 0; fingerId < 4; fingerId++)
	{
		if (arr[fingerId] != true)
		{
			entHand->vmask &= ~(1 << (4 - fingerId + 1));
		}
		else
		{
			entHand->vmask |= 1 << (4 - fingerId + 1);
		}
	}
}

void ebReset ()
{
	int i, j;
	
	for (i = 0; i < HAND_HANDS; i++)
		for (j = 0; j < HAND_FINGERS; j++)
			(g_handChop[i])[j] = false;
			
	g_ebSpeech = true;
	
	g_handDropping = false;
	g_fingerChopped = false;
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

action ebStartSpeech ()
{
	set(my, INVISIBLE | PASSABLE);
	
	// wait
	var t = 3 * 16;
	while (t > 0)
	{
		t -= time_step;
		wait(1);
	}
	
	if (g_ebSpeech)
	{
		g_ebSpeech = false;
		var h = snd_play(g_sndWarghostBattleSpeechStart, 100, 0);
		
		while (snd_playing(h))
			wait(1);
	}
	
	// switch warghost to model without hands
	if (g_entEbWarghost != NULL)
		ent_morph(g_entEbWarghost, "warghostChop.mdl");
		
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
	
	snd_play(g_sndHandFlyBoth, 100, 0);
	
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
	
	ent_create("warhand.mdl", &v, ebHandWatchSelect);
	ptr_remove(my);
}

	void ebHandFlyL ()
	{
		my->material = g_mtlHandL;
		my->skill20 = 1;
		ebHandFly();
	}

	void ebHandFlyR ()
	{
		my->material = g_mtlHandR;
		ebHandFly();
	}
	
		void ebHandFly ()
		{
			updateHandChopped(my);
			
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
				
				doHandChopBlood(my);
				
				wait(1);
			}
			
			while (my->skill2 > 0)
			{
				my->skill2 -= time_step;
				my->z += 10 * time_step;
				my->y -= 20 * time_step;
				my->roll += 5 * time_step;
				
				doHandChopBlood(my);
				
				wait(1);
			}
			
			set(my, INVISIBLE);
			ebDoSparkle(my, 2000);
			
			wait(1);
			ptr_remove(my);
		}
		
void flyawaychoppedhand ()
{
	var t = 5 * 16;
	
	while (t > 0)
	{
		my->y -= 35 * time_step;
		my->roll += time_step;
		my->z += 3 * time_step;
		
		t -= time_step;
		
		wait(1);
	}
	
	ptr_remove(my);
}
		
void doChopped (ENTITY* entHand, int lr, int fingerId)
{
	ENTITY* entzzz = ent_create(entHand->type, entHand->x, flyawaychoppedhand);
	
	entzzz->skill20 = lr;
	
	vec_set(entzzz->scale_x, entHand->scale_x);
	
	entzzz->frame = entHand->frame;
	entzzz->next_frame = entHand->next_frame;
	
	if (lr == 1)
		entzzz->material = g_mtlHandL;
	else
		entzzz->material = g_mtlHandR;
		
	entzzz->skill1 = fingerId;
	
	BOOL* arr = NULL;
	
	if (lr == 1) // left
		arr = g_handChopL;
	else // right
		arr = g_handChopR;
		
	int i;
	for (i = 0; i <= 5; i++)
		entzzz->vmask |= 1 << i;
	
	entzzz->vmask &= ~(1 << (4-fingerId+1));
	
	snd_play(g_sndFingerChop, 100, 0);
}
		
void ebHandJoint ()
{
	set(my, INVISIBLE);
	
	vec_scale(my->scale_x, 2);
	c_updatehull(my, 0);
	
	VECTOR v0, v1, v, diff;
	
	BOOL isChopped = false;
	
	while (1)
	{
		updateHandChopped(g_ebHand);
		
		int myid = (int)my->skill1;
		
		if (myid % 2 == 0)
			set(my, FLAG2);
		
		if (g_ebHand != NULL)
		{
			if (g_ebHand->skill1 != 1)
				break;
				
			BOOL bLeftHand = (g_ebHand->skill20 != 0);
			int fingerId = myid;
			VECTOR vecFingerPos, vecFingerDir;
			
			handFingerPos(g_ebHand, bLeftHand, fingerId, &vecFingerPos, &vecFingerDir);
			
			vecFingerPos.y = player->y;
			vecFingerPos.z -= 32;
			
			vec_diff(&diff, &vecFingerPos, my->x);
			
			var distplayer = absv(my->x - player->x);
			
			if (g_handDropping && vec_length(&diff) > 2)
			{
				var distmove = c_move(my, NULL, &diff, IGNORE_PASSABLE | IGNORE_SPRITES | IGNORE_FLAG2);
				
				if (g_ebHand->skill2 != 0 && distmove <= 0 && (distplayer < my->max_x * 1.25))
					player->PL_HEALTH = 0;
			}
			else
			{
				if (!g_handDropping && !g_fingerChopped)
				{
					if (distplayer < my->max_x * 2)
					{
						if (g_playerIsAttacking)
						{
							set(my, PASSABLE);
						
							if (bLeftHand) // left
								g_handChopL[my->skill1] = true;
							else // right
								g_handChopR[my->skill1] = true;
								
							isChopped = true;
							g_fingerChopped = true;
							
							doChopped(g_ebHand, g_ebHand->skill20, my->skill1);
							ebDoHit();
								
							break;
						}
					}
				}
			}
		}
		
		wait(1);
	}
	
	ptr_remove(my);
}
		
void ebCreateHandJoints (ENTITY* entHand)
{
	if (entHand == NULL)
		return;
		
	BOOL* arr = NULL;
	
	if (entHand->skill20 == 1) // left
		arr = g_handChopL;
	else // right
		arr = g_handChopR;
		
	int i;
	for (i = 0; i < HAND_FINGERS; i++)
	{
		if (arr[i] == false)
		{
			char* j0 = _chr((g_handBoneFarNames->pstring)[i]);
			char* j1 = _chr((g_handBoneNearNames->pstring)[i]);
			
			VECTOR v0, v1, v;
			vec_for_bone(&v0, entHand, j0);
			vec_for_bone(&v1, entHand, j1);
			vec_lerp(&v, &v0, &v1, 0.5);
			
			if (entHand->skill20 != 0)
			{
				vec_to_ent(&v, entHand);
				v.x *= -1;
				vec_for_ent(&v, entHand);
			}
			
			ENTITY* e = ent_create("handjoint.mdl", &v, ebHandJoint);
			e->skill1 = i;
			e->skill2 = entHand;
		}
	}
}

void ebHandWatch ()
{
	updateHandChopped(my);
	
	ebDoSparkle(my, 2000);
	snd_play(g_sndSparkle, 100, 0);
	
	set(my, PASSABLE);
	my->ambient = 75;
	
	var moveAnim = 10;
	
	var t = (5 + random(3)) * 16;
	
	vec_scale(my->scale_x, g_handScale);
	
	var ground, height;
	var heightSub = 32;
	
	g_ebHand = my;
	
	snd_play(g_sndHandFly, 100, 0);
	
	// move with player
	set(player, FLAG2);
	while (t > 0)
	{
		VECTOR vecBoneGround, vecTraceStart, vecTraceEnd;
		vec_for_bone(&vecBoneGround, my, "ground");
		
		vec_set(&vecTraceStart, my->x);
		vecTraceStart.y = vecBoneGround.y;
		
		vec_set(&vecTraceEnd, &vecTraceStart);
		vecTraceEnd.z -= 2000;
		
		height = player->z + 250;
		
		if (c_trace(&vecTraceStart, &vecTraceEnd, IGNORE_ME | USE_POLYGON | IGNORE_FLAG2 ) > 0)
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
		
		doHandChopBlood(my);
		
		wait(1);
	}
	
	reset(player, FLAG2);
	
	// drop
	
	g_handDropping = true;
	
	g_ebHand->skill1 = 1; // joints = on
	g_ebHand->skill2 = 1; // joints death = on
	
	g_playerDontScanFlag2 = true;
	g_playerNoYou = true;
	
	ebCreateHandJoints(g_ebHand);
	
	set(player, ZNEAR);
	
	t = 0;
	
	BOOL bSndHandDrop = true;
	while (t < 100)
	{
		VECTOR vecBoneGround;
		vec_for_bone(&vecBoneGround, g_ebHand, "ground");
		height = ground + (g_ebHand->z - vecBoneGround.z) - heightSub;
		
		VECTOR v;
		vec_set(&v, g_ebHand->x);
		v.z = height;
		
		vec_lerp(g_ebHand->x, g_ebHand->x, &v, 0.25 * time_step);
		
		t = clamp(t + 5 * time_step, 0, 100);
		ent_animate(g_ebHand, "drop", t, 0);

		doHandChopBlood(my);
		
		if (t > 90 && bSndHandDrop)
		{
			bSndHandDrop = false;
			snd_play(g_sndHandDrop, 100, 0);
		}
		
		wait(1);
	}
	
	g_handDropping = false;
	
	g_ebHand->skill2 = 0; // joints death = off
	
	t = (3+random(2)) * 16;
	
	g_fingerChopped = false;
	
	// wait	
	while (t > 0 && !g_fingerChopped)
	{
		ent_animate(g_ebHand, "down", (total_ticks * 4), ANM_CYCLE);
		t -= time_step;
		
		doHandChopBlood(my);
		
		wait(1);
	}
	
	snd_play(g_sndHandUp, 100, 0);
	
	reset(player, ZNEAR);
	
	g_ebHand->skill1 = 0;
	
	// up
	while (t < 100)
	{
		VECTOR vecBoneGround;
		vec_for_bone(&vecBoneGround, g_ebHand, "ground");
		height = ground + (g_ebHand->z - vecBoneGround.z) - heightSub;
		
		VECTOR v;
		vec_set(&v, g_ebHand->x);
		v.z = height;
		
		vec_lerp(g_ebHand->x, g_ebHand->x, &v, 0.25 * time_step);
		
		t = clamp(t + 2 * time_step, 0, 100);
		ent_animate(g_ebHand, "up", t, 0);		
		
		doHandChopBlood(my);
		
		wait(1);
	}
	
	g_playerDontScanFlag2 = false;
	g_playerNoYou = false;
	
	t = 1.5 * 16;
	
	snd_play(g_sndHandBliss, 100, 0);
	
	// go away
	while (t > 0)
	{
		t -= time_step;
		g_ebHand->z += 10 * time_step;
		g_ebHand->y -= 20 * time_step;
		g_ebHand->roll += 5 * time_step;
		
		doHandChopBlood(my);
		
		wait(1);
	}

	set(g_ebHand, INVISIBLE);
	ebDoSparkle(g_ebHand, 2000);
	snd_play(g_sndSparkle, 100, 0);

	wait(1);
	
	ent_create(NULL, nullvector, ebHandsBgFly);
	ptr_remove(g_ebHand);
}

void ebHandWatchL ()
{
	my->material = g_mtlHandL;
	my->skill20 = 1;
	ebHandWatch();
}

void ebHandWatchR ()
{
	my->material = g_mtlHandR;
	ebHandWatch();
}

void ebHandWatchSelect ()
{
	// prefer hand with more fingers

	int lnum = numChoppedSide(1);
	int rnum = numChoppedSide(0);
	
	if (lnum < rnum)
		ebHandWatchL();
	else
	{
		if (lnum > rnum)
			ebHandWatchR();
		else
		{
			if (random(100) < 50)
				ebHandWatchL();
			else
				ebHandWatchR();
		}
	}
}

void effEbBlood (VECTOR* pos, VECTOR* vecVel, var pSpeed, BOOL bInverse)
{
	vec_normalize(vecVel, pSpeed);
	
	if (bInverse)
		vec_scale(vecVel, -1);
	
	effect(effEbBlood_p, 1, pos, vecVel);
}

	void effEbBlood_p (PARTICLE* p)
	{
		p->bmap = getRandomBmapBankBmap(g_bbEbBlood);
		
		p->flags |= (TRANSLUCENT | MOVE);
		
		p->gravity = 2;
		
		p->alpha = 100;
		p->size = 64 + random(128);
		
		p->event = effEbBlood_ev;
	}
	
		void effEbBlood_ev (PARTICLE* p)
		{
			p->alpha = clamp(p->alpha - 5 * time_step, 0, 100);
			
			if (p->alpha <= 0)
				p->lifespan = 0;
		}
		
action ebWarghost ()
{
	g_entEbWarghost = my;
	my->material = g_mtlBossGhost;
	
	while (1)
	{
		// pos
		{
			VECTOR vecPos;
			vec_set(&vecPos, my->x);
			
			if (player != NULL)
				vecPos.x = player.x;
				
			vec_lerp(my->x, my->x, &vecPos, time_step * 0.1);
		}
		
		// ang
		{
			VECTOR vecLookAt, vecLook;
			vec_set(&vecLookAt, my->x);
			
			vecLookAt.y -= 1000;
			
			if (player != NULL)
			{
				vecLookAt.x = player->x;
				vecLookAt.y = player->y;
			}
			
			vec_diff(&vecLook, &vecLookAt, my->x);
			vec_to_angle(my->pan, &vecLook);
		}
		
		if (g_ebDoHit)
			ent_animate(my, "jump", g_ebDoHitPercent, ANM_CYCLE);
		else
			ent_animate(my, "attack", total_ticks % 100, ANM_CYCLE);
			
		
		wait(1);
	}
}

#endif /* endboss_c */