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

  Vector getNormalAt(Vector point) {
    //normal always points away from the center of a sphere
    Vector normal_Vect = point.vectAdd(center.negative()).normalize();
    return normal_Vect;
  }
  double findIntersection(Ray ray) {
    Vector ray_origin = ray.getRayOrigin();
    double ray_origin_x = ray_origin.getVectX();
    double ray_origin_y = ray_origin.getVectY();
    double ray_origin_z = ray_origin.getVectZ();

    Vector ray_direction = ray.getRayDirection();
    double ray_direction_x = ray_direction.getVectX();
    double ray_direction_y = ray_direction.getVectY();
    double ray_direction_z = ray_direction.getVectZ();

    Vector sphere_center = center;
    double sphere_center_x = sphere_center.getVectX();
    double sphere_center_y = sphere_center.getVectY();
    double sphere_center_z = sphere_center.getVectZ();

    double a = 1; // normalize
    double b = (2*(ray_origin_x - sphere_center_x)* ray_direction_x) + 
               (2*(ray_origin_y - sphere_center_y)* ray_direction_y) + 
               (2*(ray_origin_z - sphere_center_z)* ray_direction_z);
    double c = pow(ray_origin_x - sphere_center_x, 2) + 
               pow(ray_origin_y - sphere_center_y, 2) + 
               pow(ray_origin_z - sphere_center_z, 2) - 
               (radius*radius);

    double disciminant = b*b - 4*c;

    if (disciminant > 0){ 
      //ray intersects the sphere (on either side)
      //the first root (closest to camera)
      double root_1 = ((-1*b - sqrt(disciminant))/2) - 0.0000001;
      if (root_1 > 0){
        //the first root is the smallest positive root
        return root_1;
      }
      else{ 
        //second root (closer to the camera)
        //second root is the smallest positive root
        double root_2 = ((sqrt(disciminant) - b)/2) - 0.0000001;
        return root_2;
       }
    }
    else{
      //the ray did not intersect the sphere
      return -1;
    }
  }

};

Sphere::Sphere(Vector cen, double r, Color c){
    center = cen;
    radius = r;
    color = c;
  
}

#endif