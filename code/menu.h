#ifndef _MENU_H_
#define _MENU_H_

// opens main menu
void menu_open(void);

// closes the main menu.
void menu_close(void);

SOUND *menuBackgroundMusic = "lotteria_menu_mono.wav";

FONT *menuFont = "Courier#64";
BMAP *menuBackgroundImage = NULL;

PANEL *menuMainmenuBackground = 
{
	pos_x = 0;
	pos_y = 0;
	layer = 50;
} 

PANEL *menuTitle =
{
	bmap = "title.png";
	//bmap = "title.png";
	layer = 51;
	pos_y = 24;
}

TEXT *menuInfo = 
{
	pos_x = 24;
	font = "Courier#24b";
	string = "Press [Q] to exit!";
	layer = 52;
	flags = OUTLINE;
}

MATERIAL *menuBackgroundMaterial = 
{
	effect = "
	float4 vecSkill1;
	float4 ps(float2 pos : TEXCOORD0) : COLOR0
	{
		float4 color = 0;
		
		float3 time = vecSkill1.x * float3(1.2, -0.9, 1.0);
		
		color.rgb = 
			0.5 + 
			0.5 * sin(time + float3(0, 1, 2) + float3(10, -10, 5) * pos.y) * 
			cos(time + float3(3, 2, 1) - float3(-6, 8, -2) * pos.x);
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

TEXT **menuTexts;

int menuLen;

#include "menu.c"

#endif /* _MENU_H_ */