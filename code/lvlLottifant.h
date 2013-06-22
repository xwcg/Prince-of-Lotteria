#ifndef lvlLottifant_h
#define lvlLottifant_h

#include "bmap.h"

// prototypes

	void lvlLfInit ();
	void lvlLfReset ();
	void lvlLfStart ();
	void lvlLfExit ();
	
// defines

	#define LVL_LOTTIFANT_WMB "lottifant.wmb"
	#define LVL_LOTTIFANT_MUSIC "music\\POL_game_looped.wav"
	
// sounds

	SOUND* g_sndLvlLfRockBreak = "rockBreakApart.ogg";
	
// sprites

	BmapBank* g_bbLfCollisionDust = NULL;

// globals

	var g_fhLvlLottifantSong = 0;
	
	var g_lvlLottifantCamArc = 75;
	var g_lvlLottifantCamDist = 1000;

#include "lvlLottifant.c"

#endif /* lvlLottifant_h */