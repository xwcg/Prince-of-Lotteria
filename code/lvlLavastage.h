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
	
	void lvlLavaSuperReset ();

// defines

	#define LVL_LAVASTAGE_WMB "lavastage.wmb"
	
// hdr

	#define LVL_LAVASTAGE_HDR_THRESHOLD 50
	#define LVL_LAVASTAGE_HDR_STRENGTH 50
	#define LVL_LAVASTAGE_HDR_EXPOSURE 0
	
// shaders and effects

	BmapBank* g_bbankLavaAsh = NULL; // pLavaAsh%d.dds
	BmapBank* g_bbankLavaFire = NULL; // lavafire%d.tga

	MATERIAL* mtlLavaPlane =
	{
		effect = "lavaPlane.fx";
	}

// globals

	BOOL g_bLvlLavastageRunning = false;
	
	var g_lvlLavastageCamDist = 1300;	
	var g_lvlLavastageCamRaiseZ = 128;
	var g_lvlLavastageCamArc = 70;	
	
	BOOL g_lvlLavastageKeepSwirl = false;
	var g_lvlLavastageSwirlRadius = 1;
	var g_lvlLavastageSwirlAngle = 4;
	var g_lvlLavastageSwirlBlend = 1;	
	
	double g_lvlLavastageFireFac = 1;
	
	double g_lvlLavastageSwirlStrength;
	double g_lvlLavastageSwirlStrengthInit = 0.01;
	
	double g_lvlLavastageSwirlSpeed;
	double g_lvlLavastageSwirlSpeedInit = 0.2;	
	
// sounds

	SOUND* g_introLavastage = "lavastageIntro.ogg";
	SOUND* g_musicLavastage = "lavastageLoop.ogg";
	
	var g_fhLvlLavastageIntro = 0;
	var g_fhLvlLavastageSong = 0;	

#include "lvlLavastage.c"

#endif /* lvlLavastage_h */