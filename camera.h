#ifndef _CAMERA_H
#define _CAMERA_H
#include "math.h"

#include "vectors.h"
#include "rays.h"

class Camera {
  //define camera position
  Vector camPos = Vector(0,0,0), camDir = Vector(0,0,1);
  Vector camRight = Vector(0,0,0), camDown = Vector(0,0,0);
  
  
  //construct
  public:
  Camera (Vector, Vector, Vector, Vector);

  //methods
  Vector getCamPos() { return camPos; }
  Vector getCamDir() {return camDir; }
  Vector getCamRight() { return camRight; }
  Vector getCamDown() {return camDown; }

};

Camera::Camera(Vector pos, Vector dir, Vector right, Vector down){
    camPos = pos;
    camDir = dir;
    camRight = right;
    camDown = down;
  
}

#endif