#ifndef narrator_c
#define narrator_c

#include "narrator.h"

var narrate (SOUND* s)
{
	return snd_play(s, 100, 0);
}

#endif /* narrator_c */