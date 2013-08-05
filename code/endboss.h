#ifndef endboss_h
#define endboss_h

void ebStart ();

void ebStartSpeech ();

void ebHandsBgFly ();
void ebHandFlyL ();
void ebHandFlyR ();
void ebHandFly ();

void ebHandWatch ();

void ebHandWatchSelect ();

BOOL g_ebSpeech = true;

ENTITY* g_ebHand = NULL;

MATERIAL* g_mtlHandR =
{
	effect = "handR.fx";
}

MATERIAL* g_mtlHandL =
{
	effect = "handL.fx";
}

BOOL g_handDropping = false;

var g_handScale = 1.5;

SOUND* g_sndWarghostBattleSpeechStart = "warghostBattleSpeechStart.ogg";
SOUND* g_sndWarghostBattleNoChance = "warghostBattleNoChance.ogg";

BOOL g_fingerChopped = false;

TEXT* g_handBoneFarNames = { string ("K_1","R_1","M_1","Z_1"); }
TEXT* g_handBoneNearNames = { string ("K_2","R_2","M_2","Z_2"); }

void ebReset ();

#define HAND_CHOP_L 0
#define HAND_CHOP_R 1
#define HAND_HANDS 2
#define HAND_FINGERS 4

BOOL* g_handChop[2];
BOOL g_handChopL [4];
BOOL g_handChopR [4];

void effEbBlood (VECTOR* pos, VECTOR* vecVel, var pSpeed, BOOL bInverse);
void effEbBlood_p (PARTICLE* p);
void effEbBlood_ev (PARTICLE* p);

SOUND* g_sndFingerChop = "fingerChop.ogg";

SOUND* g_sndHandFlyBoth = "handFlyBoth.ogg";

SOUND* g_sndHandFly = "handFly.ogg";
SOUND* g_sndHandBliss = "handBliss.ogg";

SOUND* g_sndHandDrop = "handDrop.ogg";
SOUND* g_sndHandUp = "handUp.ogg";

BOOL g_ebDoHit = false;
var g_ebDoHitPercent = 0;

ENTITY* g_entEbWarghost = NULL;

// sprites

	BmapBank* g_bbEbBlood = NULL;

void endboss_h_startup ()
{
	g_handChop[HAND_CHOP_L] = &g_handChopL;
	g_handChop[HAND_CHOP_R] = &g_handChopR;
	
	g_bbEbBlood = newBmapBank(5, "bloodSht%d.TGA");
}



#include "endboss.c"

#endif /* endboss_h */