#ifndef _MENU_H_
#define _MENU_H_

// prototypes

	void menu_open();
	void menu_close();

// globals

	var g_menuFxSpeed = 0.2;

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

MATERIAL* g_menuMtlFx = 
{
	effect = "menu.fx";
}

TEXT **menuTexts;

int menuLen;

#include "menu.c"

#endif /* _MENU_H_ */