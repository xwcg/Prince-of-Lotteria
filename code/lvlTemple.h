#ifndef lvlTemple_h
#define lvlTemple_h

// prototypes

	void lvlTempleInit ();
	void lvlTempleReset ();
	void lvlTempleMusic ();
	void lvlTempleStart ();
	void lvlTempleExit (BOOL bNextLevel);

// defines

	#define LVL_TEMPLE_WMB "level01_v7.wmb"
	
// hdr

	#define LVL_TEMPLE_HDR_THRESHOLD 15
	#define LVL_TEMPLE_HDR_STRENGTH 40
	#define LVL_TEMPLE_HDR_EXPOSURE 0
	
// sounds

	// music
	SOUND* g_musicTempleIntro = "templeIntro.ogg";
	SOUND* g_musicTempleLoop = "templeLoop.ogg";
	
	SOUND* g_sndBatFly = "batFly.ogg";

// globals

	BOOL g_bLvlTempleRunning = false;

	var g_fhLvlTempleIntro = 0;
	var g_fhLvlTempleSong = 0;

	var g_lvlTempleCamDist = 1200;	
	var g_lvlTempleCamRaiseZ = 16;
	var g_lvlTempleCamArc = 80;

#include "lvlTemple.c"

#endif /* lvlTemple_h */