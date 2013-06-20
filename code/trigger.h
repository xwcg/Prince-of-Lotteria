#ifndef _TRIGGER_H_
#define _TRIGGER_H_

action trigger_wall();
action trigger_visible();

SOUND* snd_narrator_start = "first_movement.wav";
SOUND* snd_narrator_sky = "sky.wav";
SOUND* snd_narrator_enemy = "enemy.wav";
SOUND* snd_narrator_lottifant = "lottifant.wav";

#include "trigger.c"

#endif /* _TRIGGER_H_ */