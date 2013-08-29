#ifndef endboss_h
#define endboss_h

// prototypes

	void ebStart ();

	void ebStartSpeech ();

	void ebHandsBgFly ();
	void ebHandFlyL ();
	void ebHandFlyR ();
	void ebHandFly ();

	void ebHandWatch ();

	void ebHandWatchSelect ();
	
	void ebReset ();
	
	void effEbBlood (VECTOR* pos, VECTOR* vecVel, var pSpeed, BOOL bInverse, int num, double scale);
	void effEbBlood_p (PARTICLE* p);
	void effEbBlood_ev (PARTICLE* p);
	
	void ebSndAttack ();

// defines

	#define HAND_CHOP_L 0
	#define HAND_CHOP_R 1
	#define HAND_HANDS 2
	#define HAND_FINGERS 4

	TEXT* g_handBoneFarNames = { string ("K_1","R_1","M_1","Z_1"); }
	TEXT* g_handBoneNearNames = { string ("K_2","R_2","M_2","Z_2"); }
	
// globals

	BOOL g_ebSpeech = true;
	BOOL g_handDropping = false;

	var g_handScale = 1.5;

	ENTITY* g_ebHand = NULL;
	
	BOOL g_fingerChopped = false;
	
	BOOL* g_handChop [HAND_HANDS];
	BOOL g_handChopL [HAND_FINGERS];
	BOOL g_handChopR [HAND_FINGERS];

	BOOL g_ebDoHit = false;
	var g_ebDoHitPercent = 0;

	ENTITY* g_entEbWarghost = NULL;

	double g_ebCrazyness = 1;
	
	BOOL g_warghostAlive = true;
	
	var g_facTimeEbHandsFly = 1;
	var g_facTimeEbHandsFlyMin = 0.2;
	var g_facTimeEbHandsFlyDec = 0.8;
	
	var g_facTimeEbHandWatch = 1;
	var g_facTimeEbHandWatchMin = 0.2;
	var g_facTimeEbHandWatchDec = 0.8;
	
	var g_facTimeEbHandDropped = 1;
	var g_facTimeEbHandDroppedMin = 0.0;
	var g_facTimeEbHandDroppedDec = 0.8;	
	
	double g_effEbBloodScale = 1;
	
	var g_sndEbVol = 100;
	var g_sndEbVolDec = 2;
	
	var g_rednessInc = 1;
	
	// Falling stones
	var g_playerJustBeenHit = 0;

// shaders

	MATERIAL* g_mtlHandR = {
		effect = "handR.fx";
	}

	MATERIAL* g_mtlHandL = {
		effect = "handL.fx";
	}

// sounds

	SOUND* g_sndWarghostBattleSpeechStart = "warghostBattleSpeechStart.ogg";

	SOUND* g_sndFingerChop = "fingerChop.ogg";

	SOUND* g_sndHandFlyBoth = "handFlyBoth.ogg";

	SOUND* g_sndHandFly = "handFly.ogg";
	SOUND* g_sndHandBliss = "handBliss.ogg";

	SOUND* g_sndHandDrop = "handDrop.ogg";
	SOUND* g_sndHandUp = "handUp.ogg";
	
	SOUND* g_sndBossSplatter = "bossSplatter.ogg";
	SOUND* g_sndBossSpin = "bossSpin.ogg";
	SOUND* g_sndBossDead = "bossDead.ogg";
	
	SOUND* g_sndBossDeadSpeech = "bossDeadSpeech.ogg";
	
	SOUND* g_sndBossAttack01 = "boss01.wav";
	SOUND* g_sndBossAttack02 = "boss02.wav";
	SOUND* g_sndBossAttack03 = "boss03.wav";
	SOUND* g_sndBossAttack04 = "boss04.wav";
	SOUND* g_sndBossAttack05 = "boss05.wav";
	SOUND* g_sndBossAttack06 = "boss06.wav";
	
	SOUND* g_sndStoneImpact = "stone_impact.wav";
	SOUND* g_sndHitByStone = "die.wav";
	

// sprites

	BmapBank* g_bbEbBlood = NULL;
	
#include "endboss.c"

#endif /* endboss_h */