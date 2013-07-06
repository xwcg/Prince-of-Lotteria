#ifndef lvlBoss_h
#define lvlBoss_h

void lvlBossInit ();
void lvlBossReset ();
void lvlBossStart ();
void lvlBossExit ();

#define LVL_BOSS_WMB "bosslevel.wmb"

// sounds

	SOUND* g_musicBoss = "towerLoop.ogg";

// globals

	var g_fhLvlBossSong = 0;
	
	var g_lvlBossCamDist = 1200;	
	var g_lvlBossCamRaiseZ = 16;
	var g_lvlBossCamArc = 75;	

#include "lvlBoss.c"

#endif /* lvlBoss_h */