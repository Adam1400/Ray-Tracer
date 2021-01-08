#ifndef _OBJECTS_H
#define _OBJECTS_H
#include "math.h"

#include "rays.h"
#include "vectors.h"
#include "colors.h"

//super class
class Object {
  
  //construct
  public:
  Object ();

  //methods
  Color getObjectColor() { return Color(0,0,0,0); } //default objects 
  double findIntersection(Ray ray) { return 0; }

};

Object::Object(){ 
  
}

#endif