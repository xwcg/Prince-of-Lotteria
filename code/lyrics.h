#ifndef lyrics_h
#define lyrics_h

// prototypes

	void lyricsStart (char* filename, TEXT* txtSrc, var fhMusic);
	double lyricsNext (int* indexLyrics, int numLyrics, STRING* strLyricsLine);
	int lyricsReset (char* filename, TEXT* txtSrc);

// globals

	var g_lyricsBarHeight = 45;
	var g_lyricsBarAlpha = 40;

	TEXT* g_txtLyrics = NULL;
	TEXT* g_txtLyricsRef = NULL;
	
	STRING* g_strCurrentLyricsLine = "#256";
	STRING* g_strNextLyricsLine = "#256";
	
	PANEL* g_pnlLyricsLine =
	{
		flags = TRANSLUCENT;
		layer = 100;
	}
	
	TEXT* g_txtLyricsLine =
	{
		font = "Times#20i";
		flags = CENTER_X | CENTER_Y | SHADOW;
		string = "#256";
		layer = 200;
	}
	
	DWORD GetTickCount();

#include "lyrics.c"

#endif /* lyrics_h */