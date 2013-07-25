#ifndef _PLAYER_C_
#define _PLAYER_C_

#include "player.h"
#include <particles.c>
#include "gui.h"
#include "platform.h"
#include "traps.h"
#include "PoL_enemy.h"
#include "sky.h"
#include "zorroMesh.h"
#include "camera.h"

void setPlayerCamera (var dist, var raiseZ, var arc)
{
	g_playerCamDist = dist;
	g_playerRaiseZ = raiseZ;	
	g_playerCamArc = arc;
}

void setPlayerWalkGlide (BOOL bUse)
{
	g_playerUseWalkGlide = bUse;
}

void setPlayerControls (BOOL bHasControls)
{
	g_playerHasControls = bHasControls;
}

void setPlayerPan (var a)
{
	proc_mode = PROC_LATE;
	
	if (player != NULL)
		player->pan = a;
}

void setPlayerPan (ENTITY* e)
{
	VECTOR diff, ang;
	vec_diff(&diff, e->x, player->x);
	vec_to_angle(&ang, diff);
	
	setPlayerPan(ang.x);
}

void snd_player_jump ()
{
	switch ((int)(random(5)))
	{
		case 0: snd_play(sndJump01, 100, 0); break;
		case 1: snd_play(sndJump02, 100, 0); break;
		case 2: snd_play(sndJump03, 100, 0); break;
		case 3: snd_play(sndJump04, 100, 0); break;
		case 4: snd_play(sndJump05, 100, 0); break;
	}
}

void snd_player_wohoo ()
{
	switch ((int)(random(5)))
	{
		case 0: snd_play(sndJetpack01, 100, 0); break;
		case 1: snd_play(sndJetpack02, 100, 0); break;
		case 2: snd_play(sndJetpack03, 100, 0); break;
		case 3: snd_play(sndJetpack04, 100, 0); break;
		case 4: snd_play(sndJetpack05, 100, 0); break;
	}
}

void pAlphaFade(PARTICLE *p) {
	p.alpha -= p.skill_a*time_step;
	if (p.alpha <= 0) {
		p.lifespan = 0;
	}
}

void pJetpack(PARTICLE *p) {
	VECTOR vecTemp;
	p.skill_a = 10+random(4);
	vec_randomize(vecTemp, 10);
	vec_add (p.vel_x, vecTemp);
	p.vel_z = -20;
	vec_set(p.blue, vector(255,255,255));
	set(p, MOVE | TRANSLUCENT | BRIGHT);
	p.lifespan = 1+random(4);
	p.size = 40+random(10);
	p.event = pAlphaFade;
}

void pBlood(PARTICLE *p) {
	VECTOR vecTemp;
	p.skill_a = 10+random(4);
	vec_randomize(vecTemp, 10);
	p.bmap = bmapBlood;
	vec_add (p.vel_x, vecTemp);
	//p.vel_z = -20;
	//vec_set(p.blue, vector(255,255,255));
	set(p, MOVE | TRANSLUCENT);
	p.lifespan = 1+random(4);
	p.size = 40+random(10);
	p.event = pAlphaFade;
}

// -----------------------------------------------------------------------------------
// Player code
// -----------------------------------------------------------------------------------

void animate_player (var s)
{
	if (player == NULL)
		return;
		
	if (player->PL_IS_JUMPING == 1)
	{
		player->PL_ANIMATION_PERCENTAGE += 3 * time_step;
		ent_animate(player, "jump", player->PL_ANIMATION_PERCENTAGE, ANM_CYCLE);	
	}
	else
	{
		if (s != 0)
		{
			player->PL_ANIMATION_PERCENTAGE += 10 * time_step;
			ent_animate(player, "walk", player->PL_ANIMATION_PERCENTAGE, ANM_CYCLE);
		}
		else
		{ 
			player->PL_ANIMATION_PERCENTAGE += 3 * time_step;
			ent_animate(player, "idle", player->PL_ANIMATION_PERCENTAGE, ANM_CYCLE);
		}	
	}
	
	if (player->PL_IS_ATTACKING)
	{
		player->PL_ATTACKING_PERCENTAGE += 10 * time_step;
		ent_animate(player, "attack", player->PL_ATTACKING_PERCENTAGE, ANM_ADD);
	}
}

void setPlayerDead ()
{
	if (player != NULL)
		player->PL_HEALTH = 0;
}

action actPlayer ()
{
	move_friction = 0.3;
	ent_preload(my);
	
	// scanning flag2 is default
	g_playerDontScanFlag2 = false;
	
	setPlayerControls(true);
	
	player = my;
	
	my->PL_JUMP_TIME = 1;
	my->PL_JUMP_HEIGHT = PL_MAX_JUMP_HEIGHT;
	my->PL_IS_JUMPING = 0;
	my->PL_PLAYER_POS_Y = my->y;
	my->PL_HEALTH = 3;
	my->PL_A4_COUNT = 0;
	my->trigger_range = 100;
	//my->push = 100;
	
	c_updatehull(my, 0);
	
	wait(1);
	
	vec_set(my->min_x, vector(-20, -40, -50)); 
	vec_set(my->max_x, vector(20, 40, 55));
	
	zorroMeshOptions(me, false, false, false);
	
	move_player();
}

void move_player ()
{
	var vFallSpeed = 0;
	int nSndRandom = 0;
	
	VECTOR vecPlayerMoveSpeed;
	vec_set(&vecPlayerMoveSpeed, nullvector);
	
	while (1)
	{
		player.PL_JUMP_HEIGHT = PL_MAX_JUMP_HEIGHT;
		
		if (g_playerHasControls)
		{
			if (key_force.x > 0)
				my.pan = 0;
				
			if (key_force.x < 0)
				my.pan = -180;
			
			// walk
			vecPlayerMoveSpeed.x = g_playerWalkSpeed * key_force.x * time_step;
		}
		else
			vecPlayerMoveSpeed.x = 0;
		
		// jump
		
		// user released jump key
		if (player->PL_JUMPKEYSTILLPRESSED && !key_cuu)
			player->PL_JUMPKEYSTILLPRESSED = false;
		
		if (g_playerHasControls && key_cuu && player->PL_IS_JUMPING == 0 && !player->PL_JUMPKEYSTILLPRESSED)
		{
			player->PL_IS_JUMPING = 1;
			my.PL_ANIMATION_PERCENTAGE = 0;
			player->PL_JUMP_TIME = PL_MAX_JUMP_TIME;
			
			snd_player_jump();
		}
		
		if (player->PL_IS_JUMPING != 0)
		{
			#ifdef PL_DEBUG
				draw_text("player is jumping", 10, 60, COLOR_BLUE);
			#endif
			
			if (player->PL_JUMP_TIME > -1)
			{
				player->PL_JUMP_TIME -= 0.2 * time_step;
				vecPlayerMoveSpeed.z = player.PL_JUMP_HEIGHT * player.PL_JUMP_TIME * time_step;
			}
		}
	
		// jetpack
		if (flying_man)
		{
			if (key_space && g_playerHasControls)
			{
				if (fPlayerJetpack > 0)
				{
					// play jetpack loop	
					if (!snd_playing(vPlayerSndHandle))
						vPlayerSndHandle = ent_playsound(player, sndJetpackLoop, 500);
					
					// wohoooo
					if (vPlayerWoho == 0)
					{
						snd_player_wohoo();
						vPlayerWoho = 1;
					}
					
					player.PL_IS_JUMPING = 1;
					player.PL_JUMP_TIME += 0.25 * time_step;
					
					#ifdef PL_DEBUG
						draw_text(str_printf(NULL, "player accel up: %.3f", (double)player->PL_JUMP_TIME), 10, 330, COLOR_RED);
					#endif
					
					player.PL_JUMP_TIME = minv(player.PL_JUMP_TIME, PL_JETPACK_MAX_STRENGTH);
					fPlayerJetpack -= 0.1 * time_step;
					fPlayerJetpackCooldown = 10;
					vecPlayerMoveSpeed.z += player.PL_JUMP_TIME * time_step;
					
					if (my.pan == 0)
						effect(pJetpack,maxv(1,10*time_frame),vector(player.x-10,player.y,player.z+5),vector(0,0,-3));
					else
						effect(pJetpack,maxv(1,40*time_frame),vector(player.x+10,player.y,player.z+5),vector(0,0,-3));
				}
			}
			else
			{
				
				if (vPlayerSndHandle != 0)
					snd_stop(vPlayerSndHandle);
					
				// regenerate jetpack
				if (fPlayerJetpackCooldown > 0)
				{
					fPlayerJetpackCooldown -= 0.4 * time_step;
					
					if (fPlayerJetpackCooldown < 0)
						fPlayerJetpackCooldown = 0;
				}
				else
				{
					if (fPlayerJetpack < PL_JETPACK_MAX_FUEL)
						fPlayerJetpack += PL_JETPACK_REFRESH_STEP * time_step;
				}
			}
		}
		
		gui_update_jetpack(100 / PL_JETPACK_MAX_FUEL * fPlayerJetpack);
		gui_update_hearts();
		gui_update_a4();
		
		// Jumping below a ceiling
		var nTraceUp = c_trace(player.x, vector(player.x,player.y,player.z+5000), IGNORE_ME | IGNORE_PASSABLE | USE_BOX | IGNORE_PUSH);

		// If player jumps against the ceiling...
		if (nTraceUp > 0 && nTraceUp < 10)
		{
			// no jetpack: let him fall
			if (!key_space || fPlayerJetpack <= 0)
			{
				player.PL_JUMP_TIME = -0.1;
				vecPlayerMoveSpeed.z = player.PL_JUMP_HEIGHT * player.PL_JUMP_TIME * time_step;
			}
			else
				vecPlayerMoveSpeed.z = -0.1 * time_step;
		}
		
		// Can fall down?
		var nTraceDown = c_trace(player.x, vector(player.x,player.y,player.z-5000), IGNORE_ME | IGNORE_PASSABLE | USE_BOX | IGNORE_PUSH);
		
		// When on platform, move the payer with it
		if (you != NULL && nTraceDown < 10) {
			if (is(you, is_platform)) {
				
				#ifdef PL_DEBUG
					draw_text("Auf Plattform", 10, 130, COLOR_RED);
					draw_text(str_for_num(NULL, you->absspeed_x), 10, 160, COLOR_RED);
					draw_text(str_for_num(NULL, you->absspeed_y), 10, 190, COLOR_RED);
					draw_text(str_for_num(NULL, you->absspeed_z), 10, 220, COLOR_RED);
				#endif
				vec_add(player.x, you->absspeed_x);
			}
		}
		
		if (nTraceDown > 15 || !HIT_TARGET)
		{
			#ifdef PL_DEBUG
				draw_text("Should be falling", 10, 250, COLOR_GREEN);
				draw_text(str_for_num(NULL, vecPlayerMoveSpeed.z), 150, 250, COLOR_GREEN);
			#endif
			
			vecPlayerMoveSpeed.z += -PL_GRAVITY * time_step;
		}
		else
		{
			// on ground
			if (nTraceDown <= 10)
			{
				if (player->PL_IS_JUMPING == 1 && player->PL_JUMP_TIME < 0)
				{
					if (key_cuu && g_playerHasControls)
						player->PL_JUMPKEYSTILLPRESSED = true;
					
					player->PL_IS_JUMPING = 0;
					player->PL_JUMP_TIME = PL_MAX_JUMP_TIME;
					vFallSpeed = 0;
					
					if (!key_space)
						vPlayerWoho = 0;
				}
			}	
		}
		
		#ifdef PL_DEBUG
			draw_text(str_printf(NULL, "jetpack: %.3f", (double)fPlayerJetpack), 10, 10, COLOR_RED);
			draw_text(str_printf(NULL, "jetpack cooldown: %.3f", (double)fPlayerJetpackCooldown), 10, 40, COLOR_RED);
			draw_text(str_printf(NULL, "nTraceUp: %.3f", (double)nTraceUp), 10, 70, COLOR_RED);
			draw_text(str_printf(NULL, "nTraceDown: %.3f", (double)nTraceDown), 10, 100, COLOR_RED);
			draw_text(str_printf(NULL, "normal.z: %.3f", (double)normal.z), 10, 400, COLOR_RED);
		#endif
		
		// correct movement for slopes
		if (normal.z < 1 && normal.z > 0.7)
			vecPlayerMoveSpeed.x *= (2.5 - normal.z);
		
		// Fallen in spikes?
		var nTraceDownShort = c_trace(player.x, vector(player.x,player.y,player.z-10), IGNORE_ME | USE_BOX | IGNORE_PUSH);
		if (HIT_TARGET) {
			if (hit.entity != NULL) {
				if (is(hit.entity, is_trap)) {
					my.PL_HEALTH = 0;
					effect(pBlood,maxv(1,10*time_frame),vector(my.x, my.y-10, my.z) ,vector(0,0,-3));
					#ifdef PL_DEBUG
						draw_text("DEAD", 10, 250, COLOR_RED);
					#endif
					switch(hit.entity.skill1)
					{
						case 0:
							if (!snd_playing(vPlayerSndHandle))
								vPlayerSndHandle = snd_play(snd_narrator_death_spikes, 100, 0);
							break;
						case 1:
							if (!snd_playing(vPlayerSndHandle))
								vPlayerSndHandle = snd_play(snd_narrator_death_drown, 100, 0);
							break;
						case 2:
							if (!snd_playing(vPlayerSndHandle))
								vPlayerSndHandle = snd_play(snd_narrator_death_fire, 100, 0);
							break;
						case 3:
							if (!snd_playing(vPlayerSndHandle))
								vPlayerSndHandle = snd_play(snd_narrator_death_lava, 100, 0);
							break;
						case 4:
							if (!snd_playing(vPlayerSndHandle))
								vPlayerSndHandle = snd_play(snd_narrator_death_stones, 100, 0);
							break;
					}
				}
			}
		}
		
		// Jumped in spikes?
		var nTraceUpShort = c_trace(player.x, vector(player.x,player.y,player.z+10), IGNORE_ME | USE_BOX | IGNORE_PUSH);
		if (HIT_TARGET) {
			if (hit.entity != NULL) {
				if (is(hit.entity, is_trap)) {
					my.PL_HEALTH = 0;
					effect(pBlood,maxv(1,10*time_frame),vector(my.x, my.y-10, my.z),vector(0,0,-3));
					#ifdef PL_DEBUG
						draw_text("DEAD", 10, 250, COLOR_RED);
					#endif
					switch(hit.entity.skill1)
					{
					case 0:
						if (!snd_playing(vPlayerSndHandle))
							vPlayerSndHandle = snd_play(snd_narrator_death_spikes, 100, 0);
						break;
					case 1:
						if (!snd_playing(vPlayerSndHandle))
							vPlayerSndHandle = snd_play(snd_narrator_death_drown, 100, 0);
						break;
					case 2:
						if (!snd_playing(vPlayerSndHandle))
							vPlayerSndHandle = snd_play(snd_narrator_death_fire, 100, 0);
						break;
					case 3:
						if (!snd_playing(vPlayerSndHandle))
							vPlayerSndHandle = snd_play(snd_narrator_death_lava, 100, 0);
						break;
					case 4:
						if (!snd_playing(vPlayerSndHandle))
							vPlayerSndHandle = snd_play(snd_narrator_death_stones, 100, 0);
						break;
					}
				}
			}
		}
	
		
		// Attack
		if (key_ctrl && me.PL_IS_ATTACKING == 0 && g_playerHasControls)
		{
			me.PL_IS_ATTACKING = 1;
			my.PL_ATTACKING_PERCENTAGE = 0;
			nSndRandom = integer(random(5));
			switch(nSndRandom) {
				case 0: snd_play(sndAttack01, 100, 0); break;
				case 1: snd_play(sndAttack02, 100, 0); break;
				case 2: snd_play(sndAttack03, 100, 0); break;
				case 3: snd_play(sndAttack04, 100, 0); break;
				case 4: snd_play(sndAttack05, 100, 0); break;
			}
		}
		
		if (me.PL_IS_ATTACKING) {
			
			if (my.PL_ATTACKING_PERCENTAGE >= 30 && !is(me, PL_ATTACKED_ONCE)) {
				// Check if enemy is hit	
				
				var mode = IGNORE_ME | SCAN_ENTS;
				if (!g_playerDontScanFlag2)
					mode |= SCAN_FLAG2;
				
				c_scan(my.x,my.pan,vector(120,0,200), mode);
				
				if (you != NULL) {
					if (is(you, PoL_is_enemy)) {
						you.PoL_enemy_health -=1;
						set(my, PL_ATTACKED_ONCE);
						effect(pBlood,maxv(1,10*time_frame),vector(you.x, you.y-10, you.z),vector(0,0,-3));
					}
				}
			}
			
			if (my.PL_ATTACKING_PERCENTAGE >= 100) {
				me.PL_IS_ATTACKING = 0;
				my.PL_ATTACKING_PERCENTAGE = 0;
				reset(me, PL_ATTACKED_ONCE);
			}
		}
		
		// Is being attacked?
		if (my.PL_HEALTH <= 0) {
			
			// Cheat
			if (godmode) {
				my.PL_HEALTH = 3;
			} else {
				break;
			}
		}

		#ifdef PL_DEBUG
			draw_text(str_for_num(NULL, my.PL_HEALTH), 10, 10, COLOR_RED);
		#endif
		
		move_friction = 0;
		
		long moveFlags = IGNORE_PASSABLE | IGNORE_PASSENTS | ACTIVATE_TRIGGER | IGNORE_PUSH;
		
		if (g_playerUseWalkGlide || (!g_playerUseWalkGlide && vecPlayerMoveSpeed.x != 0))
			moveFlags |= GLIDE;
		
		c_move(my, nullvector, &vecPlayerMoveSpeed, moveFlags);
		
		my.y = my.PL_PLAYER_POS_Y;
		
		animate_player(vecPlayerMoveSpeed.x * time_step);
		
		// update camera
		cameraMove(my, g_playerCamDist, g_playerRaiseZ, g_playerCamArc);
		
		wait(1);
	}
	
	// Dead? Play animation
	if (my.PL_HEALTH <= 0) {
		ent_playsound(me, sndDie, 1000);
		gui_update_hearts();
		my.PL_ANIMATION_PERCENTAGE = 0;
		while(my.PL_ANIMATION_PERCENTAGE <= 80) {
			ent_animate(me, "die ", my.PL_ANIMATION_PERCENTAGE, ANM_CYCLE);
			my.PL_ANIMATION_PERCENTAGE +=4 * time_step;
			my.PL_ANIMATION_PERCENTAGE = minv(my.PL_ANIMATION_PERCENTAGE, 100);
			wait(1);
		}
		if (nPlayerLifes > 0) {
			nPlayerLifes -=1;
			game_restart();
		} else {
			set(txtGameOver, SHOW);
			
			wait(-2.5);
			set(txtGameOver, TRANSLUCENT);
			for(txtGameOver.alpha = 100; txtGameOver.alpha >= 0; txtGameOver.alpha -= time_step)
				wait(1);
			reset(txtGameOver, SHOW);
			reset(txtGameOver, TRANSLUCENT);
			txtGameOver.alpha = 100;
			
			snd_stop(vGameMusicHandle);
			
			sky_active = 0;
			
			level_load(NULL);
			
			menu_open();
			
			gui_hide();
			
			return;
		}
	}
}

void activate_jetpack ()
{
	flying_man = 1;
	zorroMeshOptions(player, true, false, true);
}

#endif /* _PLAYER_C_ */