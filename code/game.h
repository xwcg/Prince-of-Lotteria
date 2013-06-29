#ifndef _GAME_H_
#define _GAME_H_

var vGameMusicHandle = 0;

PANEL* panLoad = NULL;
PANEL* panLoadBar = NULL;

var vDummy = 0;
var vDummy2 = 0;

void game_start ();
void game_restart ();
void level_load_ext (STRING* _lvl);

#define LVL_INTRO_MUSIC "music\\intro.ogg"

#include "game.c"

#endif // #ifndef _GAME_H_