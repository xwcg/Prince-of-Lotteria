#ifndef _PLAYER_H_
#define _PLAYER_H_

// prototypes

	action actPlayer();
	void move_player ();
	void activate_jetpack();
	
	void snd_player_jump ();

// defines

	#define PL_GRAVITY               7
	#define PL_MAX_JUMP_TIME         1
	#define PL_MAX_JUMP_HEIGHT       110
	#define PL_JETPACK_REFRESH_STEP  0.5
	#define PL_JETPACK_MAX_STRENGTH  0.5
	#define PL_JETPACK_MAX_FUEL      10

	#define PL_JUMP_TIME             skill1
	#define PL_JUMP_HEIGHT           skill2
	#define PL_IS_JUMPING            skill3
	#define PL_ANIMATION_PERCENTAGE  skill4
	#define PL_IS_ATTACKING          skill5
	#define PL_ATTACKING_PERCENTAGE  skill6
	#define PL_PLAYER_POS_Y          skill7
	#define PL_HEALTH                skill8
	#define PL_A4_COUNT              skill10
	#define PL_ATTACKED_ONCE         FLAG8
	
// globals

	var g_playerWalkSpeed = 35;

	float fPlayerJetpack = PL_JETPACK_MAX_FUEL;
	float fPlayerJetpackCooldown = 0;

	var vPlayerSndHandle = 0;
	var vPlayerWoho = 0;

	var flying_man = 0;

	var godmode = 0;

	int nPlayerLifes = 3;

	BMAP* bmapBlood = "blood.tga";
	
// sounds

	SOUND* sndAttack01 = "attack01.wav";
	SOUND* sndAttack02 = "attack02.wav";
	SOUND* sndAttack03 = "attack03.wav";
	SOUND* sndAttack04 = "attack04.wav";
	SOUND* sndAttack05 = "attack05.wav";
	SOUND* sndDie = "die.wav";
	SOUND* sndFootsteps01 = "footstep01.wav";
	SOUND* sndFootsteps02 = "footstep02.wav";
	SOUND* sndFootsteps03 = "footstep03.wav";
	SOUND* sndFootsteps04 = "footstep04.wav";
	SOUND* sndJetpackLoop = "jetpackloop.wav";
	SOUND* sndJetpack01 = "jetpack01.wav";
	SOUND* sndJetpack02 = "jetpack02.wav";
	SOUND* sndJetpack03 = "jetpack03.wav";
	SOUND* sndJetpack04 = "jetpack04.wav";
	SOUND* sndJetpack05 = "infinity.wav";
	SOUND* sndJump01 = "jump01.wav";
	SOUND* sndJump02 = "jump02.wav";
	SOUND* sndJump03 = "jump03.wav";
	SOUND* sndJump04 = "jump04.wav";
	SOUND* sndJump05 = "jump05.wav";
	SOUND* sndLife01 = "life01.wav";
	SOUND* sndLife02 = "life02.wav";

	SOUND* snd_narrator_death_spikes = "death_by_spikes.wav";
	SOUND* snd_narrator_death_drown  = "death_by_drowning.wav";
	SOUND* snd_narrator_death_fire   = "death_by_fire.wav";
	SOUND* snd_narrator_death_lava   = "death_by_lava.wav";
	SOUND* snd_narrator_death_stones = "death_by_rock.wav";

#include "player.c"

#endif /* _PLAYER_H_ */