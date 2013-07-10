#ifndef postprocessing_h
#define postprocessing_h

// prototypes

	void initPostprocessing();
	
	void resetPpSwirl ();
	void setPpSwirl (float fRadius, float fAngle, float fX, float fY, float fBlend);
	void setPpSwirlRadius (float fRadius);
	void setPpSwirlAngle (float fAngle);
	void setPpSwirlCenter (float fX, float fY);
	void setPpSwirlBlend (float fBlend);

// effects

	// swirl
		
		#define ppSwirlRadius skill1
		#define ppSwirlAngle skill2
		#define ppSwirlCenterX skill3
		#define ppSwirlCenterY skill4
		#define ppSwirlBlend skill5
		
		MATERIAL* g_mtlSwirl = {effect = "swirl.fx";}
		
		VIEW* pp_viewSwirl =
		{
			material = g_mtlSwirl;
			flags = CHILD | PROCESS_TARGET;
		}

#include "postprocessing.c"

#endif /* postprocessing_h */