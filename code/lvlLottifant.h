#ifndef lvlLottifant_h
#define lvlLottifant_h

#include "bmap.h"

// prototypes

	void lvlLfInit ();
	void lvlLfReset ();
	void lvlLfStart ();
	void lvlLfExit ();
	
// defines

	#define LVL_LOTTIFANT_PX_GROUP_ALL 1
	#define LVL_LOTTIFANT_PX_GROUP_BALL 3
	#define LVL_LOTTIFANT_PX_GROUP_ROCKPIECE 4
	#define LVL_LOTTIFANT_PX_GROUP_PLAYER 5
	
	#define LVL_LOTTIFANT_WMB "lottifant.wmb"
	#define LVL_LOTTIFANT_MUSIC "music\\POL_game_looped.wav"
	
	#define LF_LOTTI_FILE "zorroLotti_sit.mdl"
	
	BOOL g_lvlLfDeregister = false;
	
// sounds

	SOUND* g_sndLvlLfRockBreak = "rockBreakApart.ogg";
	
// sprites

	BmapBank* g_bbLfCollisionDust = NULL;

// globals

	var g_lvlLfGravity = 10;
	var g_lvlLfRideForce = 400;
	var g_lvlLfJumpForce = 32;
	var g_lvlLfSpeedMax = 30;
	var g_lvlLfDownForce = 600;

	var g_fhLvlLottifantSong = 0;
	
	var g_lvlLottifantCamArc = 85;
	var g_lvlLottifantCamDist = 1200;

	
	ENTITY* g_entLvlLfBall = NULL;
	VECTOR g_vecEntLvlBallLast;
	
	ENTITY* g_entLvlLfPlayer = NULL;

#include "lvlLottifant.c"

#endif /* lvlLottifant_h */