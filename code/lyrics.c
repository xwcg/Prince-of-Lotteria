#ifndef lyrics_c
#define lyrics_c

#include "lyrics.h"
#include <strio.c>

void lyricsStart (char* filename, TEXT* txtSrc, var fhMusic)
{
	if (fhMusic == 0 || (filename == NULL && txtSrc == NULL))
		return;
	
	int numLyrics = lyricsReset(filename, txtSrc);
	if (numLyrics > 0)
	{
		int indexLyrics = 0;
		int displayedLyrics = 0;
		
		str_cpy(g_strCurrentLyricsLine, "");
		str_cpy(g_strNextLyricsLine, "");
		
		double nextLyrics = lyricsNext(&indexLyrics, numLyrics, g_strNextLyricsLine);
		
		double lyricsTimer = 0;
		
		HMODULE mod = LoadLibrary("Kernel32.dll");
		GetTickCount = GetProcAddress(mod, "GetTickCount");
		
		DWORD startupTime = GetTickCount();
		
		var quadAlpha = 0;
		
		while (media_playing(fhMusic) && nextLyrics >= 0 && displayedLyrics < numLyrics)
		{
			quadAlpha = clamp(quadAlpha + 5 * time_step, 0, g_lyricsBarAlpha);
			
			set(g_pnlLyricsLine, SHOW);
			g_pnlLyricsLine->alpha = quadAlpha;
			g_pnlLyricsLine->pos_y = screen_size.y-g_lyricsBarHeight;
			g_pnlLyricsLine->size_y = g_lyricsBarHeight;
			g_pnlLyricsLine->size_x = screen_size.x;
		
			double currentTime = (GetTickCount() - startupTime) * 0.016;
			if (currentTime >= nextLyrics)
			{
				str_cpy(g_strCurrentLyricsLine, g_strNextLyricsLine);				
				nextLyrics = lyricsNext(&indexLyrics, numLyrics, g_strNextLyricsLine);
				
				displayedLyrics++;
			}
			
			set(g_txtLyricsLine, SHOW);
			str_cpy((g_txtLyricsLine->pstring)[0], g_strCurrentLyricsLine);
			
			g_txtLyricsLine->pos_x = screen_size.x / 2;
			g_txtLyricsLine->pos_y = screen_size.y - g_lyricsBarHeight/2;
			
			wait(1);
		}
		
		reset(g_txtLyricsLine, SHOW);
		
		while (quadAlpha > 0)
		{
			quadAlpha = clamp(quadAlpha - 5 * time_step, 0, g_lyricsBarAlpha);
			
			g_pnlLyricsLine->alpha = quadAlpha;
			g_pnlLyricsLine->pos_y = screen_size.y-g_lyricsBarHeight;
			g_pnlLyricsLine->size_y = g_lyricsBarHeight;
			g_pnlLyricsLine->size_x = screen_size.x;			
			
			wait(1);
		}
		
		reset(g_pnlLyricsLine, SHOW);
	}
}

double lyricsNext (int* indexLyrics, int numLyrics, STRING* strLyricsLine)
{
	if (indexLyrics == NULL || strLyricsLine == NULL)
		return -1;
		
	int index = *indexLyrics;
	int maxStrings = g_txtLyricsRef->strings;
	
	double t = -1;
	
	if (index < maxStrings && index < numLyrics)
	{
		str_cpy(strLyricsLine, (g_txtLyricsRef->pstring)[index]);
		*indexLyrics = index + 1;

		STRING* strTemp = str_create("#256");
		if (strTemp != NULL)
		{
			str_cpy(strTemp, strLyricsLine);
			
			int offset = str_stri(strTemp, " ");
			if (offset >= 0)
			{
				str_trunc(strTemp, str_len(strTemp) - offset);
				str_clip(strLyricsLine, offset);
				
				t = (double)16 * str_to_float(strTemp);
				ptr_remove(strTemp);
			}
		}
	}
		
	return t;
}

int lyricsReset (char* filename, TEXT* txtSrc)
{
	if (g_txtLyrics == NULL && filename != NULL && txtSrc == NULL)
		g_txtLyrics = txt_create(256, 1);
		
	if (g_pnlLyricsLine->bmap == NULL)
	{
		g_pnlLyricsLine->bmap = bmap_createblack(16, 16, 8888);
		bmap_fill(g_pnlLyricsLine->bmap, nullvector, 50);
	}
	
	int numStr = 0;
	
	if (filename != NULL && txtSrc)
	{
		numStr = txt_load(g_txtLyrics, filename);
		g_txtLyricsRef = g_txtLyrics;
	}
	else
	{
		numStr = txtSrc->strings;
		g_txtLyricsRef = txtSrc;
	}
	
	return numStr;
}

#endif /* lyrics_c */