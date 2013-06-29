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
	
	#define LVL_TEMPLE_INTRO "music\\templeIntro.ogg"
	#define LVL_TEMPLE_MUSIC "music\\templeLoop.ogg"

// globals

	BOOL g_bLvlTempleRunning = false;

	var g_fhLvlTempleIntro = 0;
	var g_fhLvlTempleSong = 0;
	
	var g_lvlTempleCamArc = 80;

#include "lvlTemple.c"

#endif /* lvlTemple_h */