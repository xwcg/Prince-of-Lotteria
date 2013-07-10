#ifndef postprocessing_c
#define postprocessing_c

#include "postprocessing.h"

void initPostprocessing ()
{
	resetPpSwirl();
	
	camera->stage = pp_viewSwirl;
}

// swirl

void resetPpSwirl ()
{
	setPpSwirl(1, 0, .5, 0.5, 0);
}

void setPpSwirl (float fRadius, float fAngle, float fX, float fY, float fBlend)
{
	setPpSwirlRadius(fRadius);
	setPpSwirlAngle(fAngle);
	setPpSwirlCenter(fX, fY);
	setPpSwirlBlend(fBlend);
}

void setPpSwirlRadius (float fRadius)
{
	g_mtlSwirl->ppSwirlRadius = floatv(fRadius);
}

void setPpSwirlAngle (float fAngle)
{
	g_mtlSwirl->ppSwirlAngle = floatv(fAngle);
}

void setPpSwirlCenter (float fX, float fY)
{
	g_mtlSwirl->ppSwirlCenterX = floatv(fX);
	g_mtlSwirl->ppSwirlCenterY = floatv(fY);
}

void setPpSwirlBlend (float fBlend)
{
	g_mtlSwirl->ppSwirlBlend = floatv(fBlend);
}

#endif /* postprocessing_c */