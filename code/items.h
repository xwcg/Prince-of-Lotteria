#ifndef ITEMS_H
#define ITEMS_H
 
#define is_collected  FLAG4

void item_setup();
void item_fade();
void item_coin_evt();
void item_life_evt();
void item_jetpack_evt();
void item_particleFader (PARTICLE *p);
void item_particle (PARTICLE *p);

SOUND* sndCollect = "collect.wav";
SOUND* snd_narrator_jetpack = "jetpack.wav";

#include "items.c"

#endif /* ITEMS_H */