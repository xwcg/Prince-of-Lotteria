#ifndef lvlBoss_h
#define lvlBoss_h

void lvlBossInit ();
void lvlBossReset ();
void lvlBossStart ();
void lvlBossExit ();

#define LVL_BOSS_WMB "bosslevel.wmb"

// materials

	MATERIAL* g_mtlBossTrail = {effect = "trail.fx";}
	MATERIAL* g_mtlBossGhost = {effect = "warghost.fx";}
	
// effects

	BMAP* g_bmapSparkle = "sparkle.tga";

// sounds

	SOUND* g_musicBoss = "towerLoop.ogg";
	
	SOUND* g_narrateBottleFound = "bottleFound.wav";
	SOUND* g_narrateGenieSpeechAfterRelease = "genieSpeechAfterRelease.wav";
	SOUND* g_sndSwirl = "swirl.ogg";
	SOUND* g_sndSparkle = "sparkle.ogg";
	
// globals

	ENTITY* g_entBossBottle = NULL;

	var g_fhLvlBossSong = 0;
	var g_fhLvlBossSongVol = 100;
	
	var g_lvlBossCamDist = 1200;	
	var g_lvlBossCamRaiseZ = 16;
	var g_lvlBossCamArc = 75;
	
	BOOL g_lvlBossGhostVisible = false;

#include "lvlBoss.c"

#endif /* lvlBoss_h */