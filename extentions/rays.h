#ifndef _RAYS_H
#define _RAYS_H
#include "math.h"

#include "vectors.h"

class Ray {
  Vector origin = Vector(0,0,0), direction = Vector(1,0,0);
  
  //construct
  public:
  Ray (Vector, Vector);

  //methods
  Vector getRayOrigin() { return origin; }
  Vector getRayDirection() {return direction; }

};

Ray::Ray(Vector o, Vector d){
    origin = o;
    direction = d;
  
}

#endif