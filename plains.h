#ifndef _PLAINS_H
#define _PLAINS_H
#include "math.h"

#include "vectors.h"
#include "colors.h"
#include "objects.h"

//subclass of objects
class Plain : public Object {
  Vector normal = Vector(1,0,0);
  double distance = 0;
  Color color = Color(0.5, 0.5, 0.5, 0); //default grey
  
  //construct
  public:
  Plain (Vector, double, Color);

  //methods
  Vector getPlainNormal() { return normal; }
  double getPlainDistance() { return distance; }
  virtual Color getColor() { return color; }

  virtual Vector getNormalAt(Vector point) { return normal; }

  virtual double findIntersection(Ray ray) { 
      Vector ray_direction = ray.getRayDirection();
      
      double a = ray_direction.dotProduct(normal); 
      if (a == 0){ 
          // ray is parallel to the plain
          return -1;
      }
      else{
          //inverse ray from origin
          double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMultiply(distance).negative()));
          return -1*b/a; //distance from ray to point of intersection
      }
  }


};

Plain::Plain(Vector n, double d, Color c){
    normal = n;
    distance = d;
    color = c;
  
}

#endif