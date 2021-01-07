#ifndef _VECTORS_H
#define _VECTORS_H
#include "math.h"

class Vector {
  double x = 0, y = 0, z = 0; 
  
  //construct
  public:
  Vector (double, double, double);

  //methods
  double getVectX() { return x; }
  double getVectY() { return y; }
  double getVectZ() { return z; }

  double magnitude() {
    return sqrt((x*x) + (y*y) + (z*z));
  }

  Vector normalize() {
    double magnitude = sqrt((x*x) + (y*y) + (z*z));
    return Vector(x/magnitude, y/magnitude, z/magnitude);
  }

  Vector negative() {
    return Vector(-x, -y, -z);
  }

  double dotProduct(Vector v){
    return x*v.getVectX() + y*v.getVectY() + z*v.getVectZ();
  }

  Vector crossProduct(Vector v){
    return Vector(y*v.getVectZ() - z*v.getVectY(), 
                  z*v.getVectX() - x*v.getVectZ(), 
                  x*v.getVectY() - y*v.getVectX()); 
  }

  Vector vectAdd(Vector v){
    return Vector(x + v.getVectX(), y + v.getVectY(), z + v.getVectZ());
  }

  Vector vectMultiply(double scaler){
    return Vector(x*scaler, y*scaler, z*scaler);
  }
};

Vector::Vector(double i, double j, double k){
    x = i;
    y = j;
    z = k;

}

#endif