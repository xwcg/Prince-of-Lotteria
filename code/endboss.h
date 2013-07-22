#ifndef endboss_h
#define endboss_h

void ebStart ();

void ebStartSpeech ();

void ebHandsBgFly ();
void ebHandFlyL ();
void ebHandFlyR ();
void ebHandFly ();

void ebHandWatch ();

MATERIAL* g_mtlHandR =
{
	effect = "handR.fx";
}

MATERIAL* g_mtlHandL =
{
	effect = "handL.fx";
}

var g_handScale = 1.5;

SOUND* g_sndWarghostBattleSpeechStart = "warghostBattleSpeechStart.ogg";
SOUND* g_sndWarghostBattleNoChance = "warghostBattleNoChance.ogg";

TEXT* g_handBoneNames = {
	string ("Z_2","M_2","R_2","K_2");
}

void ebReset ();

#define HAND_CHOP_L 0
#define HAND_CHOP_R 1
#define HAND_HANDS 2
#define HAND_FINGERS 4

BOOL* g_handChop[2];
BOOL g_handChopL [4];
BOOL g_handChopR [4];

void endboss_h_startup ()
{
	g_handChop[HAND_CHOP_L] = &g_handChopL;
	g_handChop[HAND_CHOP_R] = &g_handChopR;
}

#include "endboss.c"

#endif /* endboss_h */