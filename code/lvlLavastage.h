#ifndef lvlLavastage_h
#define lvlLavastage_h

#include "bmap.h"

// prototypes

	void lvlLavastageInit ();
	void lvlLavastageReset ();
	void lvlLavastageStart ();
	void lvlLavastageExit (BOOL bNextLevel);
	
	void assertMtlLavaPlane (ENTITY* e);
	void initFxLavaPlane (ENTITY* e, float tile, VECTOR* uv, float turbAmplitude, float turbSpeed, float turbModulation);
	void mtlLavaPlaneEv ();
	
	void effLavaFire (VECTOR* pos);
	void effLavaFire_p (PARTICLE* p);
	void effLavaFire_ev (PARTICLE* p);
	
	void effLavaAsh (VECTOR* pos);
	void effLavaAsh_p (PARTICLE* p);
	void effLavaAsh_ev (PARTICLE* p);	

// defines

	#define LVL_LAVASTAGE_WMB "lavastage.wmb"
	
// shaders and effects

	BmapBank* g_bbankLavaAsh = NULL; // pLavaAsh%d.dds
	BmapBank* g_bbankLavaFire = NULL; // lavafire%d.tga

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
	
	BOOL g_lvlLavastageKeepSwirl = false;
	var g_lvlLavastageSwirlRadius = 1;
	var g_lvlLavastageSwirlAngle = 4;
	var g_lvlLavastageSwirlBlend = 1;	
	
	
// sounds

	SOUND* g_musicLavastage = "lavastageLoop.ogg";

#include "lvlLavastage.c"

#endif /* lvlLavastage_h */