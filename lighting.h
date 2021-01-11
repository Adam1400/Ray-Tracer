#ifndef _LIGHTING_H
#define _LIGHTING_H
#include "math.h"
#include "vectors.h"
#include "colors.h"
#include "source.h"

class Light : public Source {
  Vector position = Vector(0,0,0); //default position at origin
  Color color = Color(1,1,1,0); //default color white
  
  //construct
  public:
  Light();
  
  Light (Vector, Color);

  //methods
  virtual Vector getLightPosition() { return position; }
  virtual Color getLightColor() { return color; }

};

Light::Light(Vector p, Color c){
    position = p;
    color = c;
  
}

#endif