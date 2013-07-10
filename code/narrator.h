#ifndef narrator_h
#define narrator_h

// prototypes

	var narrate (SOUND*);

// sounds

	SOUND* snd_narrator_start = "first_movement.wav";
	SOUND* snd_narrator_sky = "sky.wav";
	SOUND* snd_narrator_enemy = "enemy.wav";
	SOUND* snd_narrator_lottifant = "lottifant.wav";

#include "narrator.c"

#endif /* narrator_h */