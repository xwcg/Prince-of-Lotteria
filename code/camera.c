#ifndef _CAMERA_C_
#define _CAMERA_C_

#include "camera.h"

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

#endif /* _CAMERA_C_ */