#ifndef _CAMERA_C_
#define _CAMERA_C_

#include "camera.h"

//void cameraMove (ENTITY* e, var distance, var raiseZ, var arc)
void cameraMove (ENTITY* e, ENTITY* e2, var distance, var raiseZ, var arc)
{
	static VECTOR vecEntLast;
	
	camera->arc += (arc - camera->arc) * g_cameraArcBlendFac;
	
	if (e != NULL)
	{
			ENTITY* er = e2;
		if (er == NULL)
			er = e;
			
		var distAdd = vec_dist(e->x, er->x);
		var d = distance + distAdd;
		
		BOOL bZoom = key_space;
		var yOffset = d + bZoom * d * g_cameraZoomFac;
		
		static VECTOR diff;
		vec_diff(&diff, e->x, &vecEntLast);
		
		static ANGLE cameraAngle;
		static VECTOR cameraPos;
		
		vec_set(&cameraAngle, nullvector);
		vec_set(&cameraPos, nullvector);
		
		cameraAngle.pan = 90;
		
		cameraPos.y = -yOffset;
		
		if (abs(diff.x) > g_cameraDeaddist * time_step)
		{
			cameraAngle.pan += sign(-diff.x) * g_cameraAngLimit;
			cameraPos.x = sign(diff.x) * g_cameraLookaheadX * time_step;
		}
		
		if (abs(diff.z) > g_cameraDeaddist * time_step)
		{
			cameraAngle.tilt += sign(diff.z) * g_cameraAngLimit;
			cameraPos.z = sign(diff.z) * g_cameraLookaheadZ * time_step;
		}
		
		cameraPos.z += raiseZ;
		
		VECTOR vecE;
		vec_lerp(&vecE, e->x, er->x, 0.5);
			
		vec_add(&cameraPos, &vecE);
		
		vec_lerp(camera->x, camera->x, &cameraPos, g_cameraPosBlendFac);
		vec_lerp(camera->pan, camera->pan, &cameraAngle, g_cameraAngBlendFac);
		
		vec_set(&vecEntLast, e->x);
	}
}

void cameraMove (ENTITY* e, var distance, var raiseZ, var arc)
{
	cameraMove(e, NULL, distance, raiseZ, arc);
}

void cameraMove (ENTITY* e, var distance, var arc)
{
	cameraMove(e, distance, 0, arc);
}

// deprecated!
void move_camera ()
{
	// don't allow to turn the camera
	vec_set(camera.pan, vector(90, 0, 0));
	
	camera.arc = 80;
	var angle = 0;

	int anglimit = 5;
	
	var zoom = 1200;
	
	while (1)
	{
		if (player != NULL)
		{
			if (abs(ang(player.pan)) < 90)
				angle = clamp(angle - 5 * time_step, -anglimit, anglimit);
			else
				angle = clamp(angle + 5 * time_step, -anglimit, anglimit);
				
			zoom = 0.95 * zoom + 0.05 * (1200 + 200 * key_space);
			
			VECTOR pos;
			
			vec_set(pos, vector(0, -zoom, 0));
			vec_rotate(pos, vector(angle, 0, 0));
			vec_add(pos, vector(player.x, 0, player.z));
			
			pos.x -= 5 * angle;
			
			vec_lerp(camera.x, camera.x, pos, 0.05);
			
			camera.pan -= 90;
			camera.pan = 0.95 * camera.pan + 0.05 * angle;
			camera.pan += 90;				
		}
		
		wait(1);	
	}
}

void camera_path(STRING* _pathname) {
	me = ent_create(NULL,nullvector,NULL);
	path_set(me,_pathname);
	var dist = 0;
	var vLastPos[3];
	var vDir[3];
	while(1)  {
		
		// place the camera on the path
		path_spline(me,camera.x,dist);
		dist += 5*time_step;
		
		if (player != NULL) {
			// let the camera look at the player	
			vec_diff(vDir,player.x,vLastPos);
			vec_to_angle(camera.pan,vDir);
			vec_set(vLastPos,camera.x);
		}
		wait(1);
	}
}

#endif /* _CAMERA_C_ */