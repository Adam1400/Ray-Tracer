#ifndef _SPHERES_H
#define _SPHERES_H
#include "math.h"

#include "vectors.h"
#include "colors.h"
#include "objects.h"

//subclass of objects
class Sphere : public Object {
  Vector center = Vector(0,0,0);
  double radius = 1.0;
  Color color = Color(0.5, 0.5, 0.5, 0); //default grey
  
  //construct
  public:
  Sphere (Vector, double, Color);

  //methods
  Vector getSphereCenter() { return center; }
  double getSphereRadius() {return radius; }
  Color getSphereColor() { return color; }

};

Sphere::Sphere(Vector cen, double r, Color c){
    center = cen;
    radius = r;
    color = c;
  
}

#endif