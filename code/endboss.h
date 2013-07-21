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

SOUND* g_sndWarghostBattleSpeechStart = "warghostBattleSpeechStart.ogg";
SOUND* g_sndWarghostBattleNoChance = "warghostBattleNoChance.ogg";

#include "endboss.c"

#endif /* endboss_h */