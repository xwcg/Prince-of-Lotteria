#ifndef _CAMERA_H_
#define _CAMERA_H_

// updates camera for current frame and entity
void cameraMove (ENTITY* e, var distance, var arc);
void cameraMove (ENTITY* e, var distance, var raiseZ, var arc);

void cameraMove (ENTITY* e1, ENTITY* e2, var distance, var raiseZ, var arc);

// coroutine for the player (deprecated)
void move_camera ();

var g_cameraArcBlendFac = 0.05;
var g_cameraZoomFac = 0.2;
var g_cameraDeaddist = 5;
var g_cameraLookaheadX = 250;
var g_cameraLookaheadZ = 125;
var g_cameraPosBlendFac = 0.05;
var g_cameraAngBlendFac = 0.03;
var g_cameraAngLimit = 7.5;

#include "camera.c"

#endif /* _CAMERA_H_ */