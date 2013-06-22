#ifndef bmap_h
#define bmap_h

void getColorFromBmap (BMAP* bmap, int x, int y, COLOR* color);

typedef struct BmapBank
{
	int length;
	BMAP** bmaps;
} BmapBank;

BmapBank* newBmapBank (int length, char* filemask);
void freeBmapBank (BmapBank* bmapBank);
void deleteBmapBank (BmapBank* bmapBank);

int getBmapBankCount (BmapBank* bmapBank);
BMAP* getBmapBankBmap (BmapBank* bmapBank, int index);
BMAP* getRandomBmapBankBmap (BmapBank* bmapBank);

// Code include
#include "bmap.c"

#endif /* bmap_h */