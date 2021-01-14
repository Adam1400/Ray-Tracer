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
  virtual Color getColor() { return Color(0,0,0,0); } //default objects 
  virtual Vector getNormalAt(Vector intersection_position ) { return Vector(0,0,0); }
  virtual double findIntersection(Ray ray) { return 0; }

};

Object::Object(){ 
  
}

#endif