#ifndef _TRIGGER_C_
#define _TRIGGER_C_

#include "trigger.h"
#include "narrator.h"

action trigger_visible ()
{
	VECTOR vecTemp;
	while(1)
	{
		vec_set(vecTemp, my.x);
		if (vec_to_screen(vecTemp, camera) != NULL) {
			switch(my.skill1)
			{
			case 0:
				snd_play(snd_narrator_start, 100, 0);
				break;
			case 1:
				snd_play(snd_narrator_sky, 100, 0);
				break;
			case 2:
				snd_play(snd_narrator_enemy, 100, 0);
				break;
			case 3:
				snd_play(snd_narrator_lottifant, 100, 0);
				break;
			}
			break;
		}
		wait(1);
	}
}

#endif /* _TRIGGER_C_ */