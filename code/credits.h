#ifndef _CREDITS_H_
#define _CREDITS_H_

// prototypes

	// opens the credits
	void creditsInit ();
	
	void creditsReset ();
	void creditsExit ();
	void creditsStart ();

	action acCreditsTerrain ();
	action acCreditsSign ();

	void credits_populate();

	action credits_lotti ();

	void credits_fire (PARTICLE *p);
	void credits_fire_event (PARTICLE *p);

	action explo_sprite ();

	void creditsSpace ();
	
// music, sfx

	var g_fhCreditsSong = 0;
	SOUND *creditsExplo = "explo.wav";
	
// globals

	var creditsFinished = 0;
	int currentRow = 0;
	VECTOR g_vecCreditsCamShake;
	BOOL g_bCreditsAllExplode = false;
	
// gui

	STRING *creditsText1Head = "#256";
	STRING *creditsText1Body = "#256";
	STRING *creditsText2Head = "#256";
	STRING *creditsText2Body = "#256";
	
	TEXT *creditsHead1 = { string = creditsText1Head; font = creditsFontHead; pos_y = 100; }
	TEXT *creditsBody1 = { string = creditsText1Body; font = creditsFontBody; pos_y = 150; }
	TEXT *creditsHead2 = { string = creditsText2Head; font = creditsFontHead; flags = ARIGHT; pos_y = 0; }
	TEXT *creditsBody2 = { string = creditsText2Body; font = creditsFontBody; flags = ARIGHT; pos_y = 0; }

	FONT *creditsFontHead = "Arial#26";
	FONT *creditsFontBody = "Arial#20";

	TEXT* g_txtCreditsSpace =
	{
		font = "Arial#40b";
		string = "Press [SPACE] to quit to main menu.";
		flags = CENTER_X;
	}

// level

	ENTITY* g_entCreditsTerrain = NULL;
	
// materials

	MATERIAL* creditsBlinker = 
	{
		effect = "
		float4 vecTime;
		float4 ps(float2 pos : TEXCOORD0) : COLOR0
		{
			float4 color = 0;
			
			float3 time = vecTime.w * float3(1.2, -0.9, 1.0);
			
			color.rgb = 
				0.5 + 
				0.5 * sin(time + float3(0, 1, 2)) * 
				cos(time + float3(3, 2, 1));
			color.a = 1;
			
			color.rgb = pow(0.5 + color.rgb, 2) - 0.5;
			
			return color;
		}
		
		technique
		{
			pass
			{
				PixelShader = compile ps_2_0 ps();
			}
		}";
	}

#include "credits.c"

#endif /* _CREDITS_H_ */