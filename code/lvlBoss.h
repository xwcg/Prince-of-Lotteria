#ifndef lvlBoss_h
#define lvlBoss_h

void lvlBossInit ();
void lvlBossReset ();
void lvlBossStart ();
void lvlBossExit ();

#define LVL_BOSS_WMB "bosslevel.wmb"
#define LVL_BOSS_MUSIC "music\\POL_game_looped.wav"

var g_lvlBossCamArc = 75;
var g_lvlBossCamDist = 1200;

var g_fhLvlBossSong = 0;

#include "lvlBoss.c"

#endif /* lvlBoss_h */