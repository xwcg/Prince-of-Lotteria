#ifndef _CREDITS_H_
#define _CREDITS_H_

// prototypes

	// opens the credits
	void creditsInit ();
	
	void creditsReset ();
	void creditsExit ();
	void creditsStart ();
	void creditsText ();

	action acCreditsTerrain ();
	action acCreditsSign ();

	void credits_populate();

	action credits_lotti ();

	void credits_fire (PARTICLE *p);
	void credits_fire_event (PARTICLE *p);

	action explo_sprite ();

	void creditsSpace ();
	
	BOOL crLottiUpdate (ENTITY* e);
	
// sounds

	// music
	SOUND* g_musicCredits = "CloneFall.ogg";
	
	SOUND* creditsExplo = "explo.wav";
	SOUND* g_sndJetpackCredits = "jetpackCredits.ogg";
	
// hdr

	#define LVL_CREDITS_HDR_THRESHOLD 50
	#define LVL_CREDITS_HDR_STRENGTH 25
	#define LVL_CREDITS_HDR_EXPOSURE 25
	
// globals
	
	var g_fhCreditsSong = 0;

	#define CR_LOTTI_FILE "zorroLotti.mdl"
	
	int g_crRow = 0;

	var creditsFinished = 0;
	int currentRow = 0;
	VECTOR g_vecCreditsCamShake;
	BOOL g_bCreditsAllExplode = false;
	
	BOOL g_bCrFirstLotti = false;
	
	MATERIAL* g_mtlLotti =
	{
		effect = "lottiGpu.fx";
	}

	#define crInit skill1
	#define crSwordAtBelt skill2
	#define crExploder skill3
	#define crTime skill4
	#define crAccel skill5 // 6,7
	#define crSpeed skill8 // 9,10
	#define crPercent skill11
	#define crState skill12
	#define crEffTimer skill13
	
	/*
	ENTITY* g_crLottiPreload =
	{
		type = "zorroLotti.mdl";
		material = g_mtlLotti;
	}	
	*/
	
// gui

	STRING *creditsText1Head = "#256";
	STRING *creditsText1Body = "#256";
	STRING *creditsText2Head = "#256";
	STRING *creditsText2Body = "#256";
	
	TEXT *creditsHead1 = { string = creditsText1Head; font = creditsFontHead; flags = SHADOW; pos_y = 100; }
	TEXT *creditsBody1 = { string = creditsText1Body; font = creditsFontBody; flags = SHADOW; pos_y = 150; }
	TEXT *creditsHead2 = { string = creditsText2Head; font = creditsFontHead; flags = ARIGHT | SHADOW; pos_y = 0; }
	TEXT *creditsBody2 = { string = creditsText2Body; font = creditsFontBody; flags = ARIGHT | SHADOW; pos_y = 0; }

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
	
// lyrics

TEXT* g_txtCreditsLyrics =
{
    string  =  (
                "0.0 ",
                "22.9 That was the game,",
                "28.0 it was shitty and a shame,",
                "34.9 JCL did cry just then,",
                "41.6 over, and over... aga-i-i-i-n.",
                "49.2 ‘Cause of this crap game,",
                "54.6 he killed a batch of clones again",
                "61.1 didn't even see it comi-i-i-ing",
                "68.0 flushed away, in the ca-a-a-an.",
                "75.8 Kill the Lotters,",
                "79.2 when he’s Zorro,",
                "82.2 he will hunt Ghosts,",
                "85.6 face the boss, forever - at Clonefall.",
                "92.0 ",
                "95.3 Clonefall is in your heart,",
                "102.8 mad insanity's just the start,",
                "110.3 You should go. and cry in the dark.",
                "114.2 You think you know Lotti, let me tell you a thing:",
                "119.8 he’s a master Clone-Lo-o-o-o-o-rd.",
                "126.3 Kill the Lotters, (kill the Lotters)",
                "129.1 when he’s Zorro, (when he’s Zorro)",
                "132.7 so he'll hunt ghosts, (he will hunt ghosts)",
                "135.7 face the boss, forever",
                "139.2 Kill the Lotters, (kill the Lotters)",
                "142.4 when he’s Zorro, (when he’s Zorro)",
                "145.5 he will hunt ghosts, (he will hunt ghosts)",
                "149.1 face the boss, forever - at Clonefall.",
                "155.0 ",
                "159.3 Where you go, he knows,",
                "162.9 what you see, he sees,",
                "165.5 you can never defeat him,",
                "169.0 without one infinity",
                "172.5 and a clone farm",
                "175.6 and high tech alarms,",
                "179.5 he cut the fingers from his hand,",
                "181.6 and he’ll expand.",
                "186.1 Kill the Lotters, (kill the Lotters)",
                "189.1 when he’s Zorro. (when he’s zorro)",
                "192.7 he will hunt ghosts, (he will hunt ghosts)",
                "196.0 face the boss, forever",
                "199.1 kill the Lotters, (Kill the Lotters)",
                "202.6 when he’s Zorro, (when he’s Zorro)",
                "205.8 he will hunt Ghosts. (he will hunt ghosts)",
                "209.2 face the boss, forever - at Clonefall.",
                "216.0 "
               );
}


#include "credits.c"

#endif /* _CREDITS_H_ */