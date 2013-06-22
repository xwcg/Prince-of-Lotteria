#ifndef bmap_c
#define bmap_c

#include "bmap.h"

void getColorFromBmap (BMAP* bmap, int x, int y, COLOR* color)
{
	int format = bmap_lock(bmap, 0);
	if (format != 0)
	{
		var pixel = pixel_for_bmap(bmap, x, y);
		pixel_to_vec(color, NULL, format, pixel);
	}

	bmap_unlock(bmap);
}

BmapBank* newBmapBank (int length, char* filemask)
{
	if ((length <= 0) || (filemask == NULL))
		return(NULL);
		
	if (strlen(filemask) == 0)
		return(NULL);

	BmapBank* bmapBank = (BmapBank*)sys_malloc(sizeof(BmapBank));
	bmapBank->bmaps = (BMAP**)sys_malloc(length * sizeof(BMAP*));
	
	bmapBank->length = length;

	char buffer [256];
	int i;

	for (i = 0; i < length; i++)
	{
		sprintf(buffer, filemask, i);
		(bmapBank->bmaps)[i] = bmap_create(buffer);
	}

	return(bmapBank);
}

void freeBmapBank (BmapBank* bmapBank)
{
	if (bmapBank == NULL)
		return;
		
	int i;
	for (i = 0; i < bmapBank->length; i++)
	{
		bmap_purge((bmapBank->bmaps)[i]);
	}

	sys_free(bmapBank->bmaps);
	sys_free(bmapBank);
}

void deleteBmapBank (BmapBank* bmapBank)
{
	if (bmapBank == NULL)
		return;

	sys_free(bmapBank->bmaps);
	sys_free(bmapBank);
}

int getBmapBankCount (BmapBank* bmapBank)
{
	if (bmapBank == NULL)
		return(0);

	return(bmapBank->length);
}

BMAP* getBmapBankBmap (BmapBank* bmapBank, int index)
{
	if (bmapBank == NULL)
		return(NULL);

	if ((index < 0) || (index >= bmapBank->length))
		return(NULL);

	return((bmapBank->bmaps)[index]);
}

BMAP* getRandomBmapBankBmap (BmapBank* bmapBank)
{
	if (bmapBank == NULL)
		return(NULL);

	int index = (int)random(bmapBank->length);
	return((bmapBank->bmaps)[index]);
}

#endif /* bmap_c */