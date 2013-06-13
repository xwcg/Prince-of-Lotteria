#ifndef _GAME_H_
#define _GAME_H_

void game_start(void);
void game_restart(void);
void level_load_ext(STRING* _lvl);

var vGameMusicHandle = 0;

PANEL* panLoad = NULL;
PANEL* panLoadBar = NULL;

#include "game.c"

#endif // #ifndef _GAME_H_