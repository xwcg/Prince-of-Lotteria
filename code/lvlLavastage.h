#ifndef lvlLavastage_h
#define lvlLavastage_h

// prototypes

	void lvlLavastageInit ();
	void lvlLavastageReset ();
	void lvlLavastageStart ();
	void lvlLavastageExit (BOOL bNextLevel);
	
	void assertMtlLavaPlane (ENTITY* e);
	void initFxLavaPlane (ENTITY* e, float tile, VECTOR* uv, float turbAmplitude, float turbSpeed, float turbModulation);
	void mtlLavaPlaneEv ();

// defines

	#define LVL_LAVASTAGE_WMB "lavastage.wmb"
	
// shaders

	MATERIAL* mtlLavaPlane =
	{
		effect = "lavaPlane.fx";
	}

// globals

	var g_fhLvlLavastageSong = 0;
	BOOL g_bLvlLavastageRunning = false;
	
	var g_lvlLavastageCamDist = 1300;	
	var g_lvlLavastageCamRaiseZ = 128;
	var g_lvlLavastageCamArc = 70;	
	
	
// sounds

	SOUND* g_musicLavastage = "lavastageLoop.ogg";

#include "lvlLavastage.c"

#endif /* lvlLavastage_h */