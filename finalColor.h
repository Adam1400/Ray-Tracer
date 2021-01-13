#ifndef _FINALCOLOR_H
#define _FINALCOLOR_H

#include "math.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "savebmp.h"
#include "vectors.h"
#include "rays.h"
#include "camera.h"
#include "colors.h"
#include "lighting.h"
#include "objects.h"
#include "spheres.h"
#include "plains.h"
#include "source.h"
#include "validObject.h"

using std::vector;

//implement shadows 
Color getColorAt(Vector intersection_position, 
                 Vector  intersection_ray_direction, 
                 vector<Object*> scene_objects, 
                 int indexOfValidObjects, 
                 vector<Source*> light_sources,
                 double accuracy, 
                 double ambientlight){

                   Color valid_object_color = scene_objects.at(indexOfValidObjects)->getColor();
                   Vector valid_object_normal = scene_objects.at(indexOfValidObjects)->getNormalAt(intersection_position);
                   Color final_color = valid_object_color.colorScalar(ambientlight);

                   //overide color is SPECIAL val is 2 (checkherboard)
                   if (valid_object_color.getSpecial() == 2){
                     //tile pattern 
                     int square = (int)floor(intersection_position.getVectX()) + (int)floor(intersection_position.getVectZ());

                     if ((square % 2) == 0){
                       //black tile
                       valid_object_color.setRed(0.35);
                       valid_object_color.setGreen(0.35);
                       valid_object_color.setBlue(0.35);
                     }
                     else{
                       //white tile
                       valid_object_color.setRed(1);
                       valid_object_color.setGreen(1);
                       valid_object_color.setBlue(1);
                     }
                   }

                   //apply reflections (if sp valis is between 0 and 1)
                   if (valid_object_color.getSpecial() > 0 && valid_object_color.getSpecial() <= 1){
                     
                     //reflection from objects
                     double dot1 = valid_object_normal.dotProduct(intersection_ray_direction.negative());
                     Vector scalar1 = valid_object_normal.vectMultiply(dot1);
                     Vector add1 = scalar1.vectAdd(intersection_ray_direction);
                     Vector scalar2 = add1.vectMultiply(2);
                     Vector add2 = intersection_ray_direction.negative().vectAdd(scalar2);
                     Vector reflection_direction = add2.normalize();

                     Ray reflection_ray (intersection_position, reflection_direction);

                     //determine what the ray intersects with first
                     vector<double> reflection_intersections;
                     for (int reflection_index = 0; reflection_index < scene_objects.size(); reflection_index++){
                       reflection_intersections.push_back(scene_objects.at(reflection_index)->findIntersection(reflection_ray));                       
                     }

                     int indexOfObjectsWithReflections = validObjectIndex(reflection_intersections);

                     //ray didnt intersect with anything else 
                     if (indexOfObjectsWithReflections != -1){
                       if(reflection_intersections.at(indexOfObjectsWithReflections) > accuracy){
                         //determiune position and direction of reflection
                         //only scale color if it relfects 
                         Vector reflection_intersection_position = intersection_position.vectAdd(reflection_direction.vectMultiply(reflection_intersections.at(indexOfObjectsWithReflections)));
                         Vector reflection_intersection_ray_direction = reflection_direction;
                         
                         //recursive reflections
                         Color reflection_intersection_color = getColorAt(reflection_intersection_position, reflection_intersection_ray_direction, scene_objects, indexOfObjectsWithReflections, light_sources, accuracy, ambientlight);
                         
                         //modify color based on degree of special value
                         final_color = final_color.colorAdd(reflection_intersection_color.colorScalar(valid_object_color.getSpecial()));

                       }
                     }
                   }

                   for (int light_index = 0 ; light_index < light_sources.size(); light_index++){
                      Vector light_direction = light_sources.at(light_index) ->getLightPosition().vectAdd(intersection_position.negative()).normalize();
                     
                      float cosine_angle = valid_object_normal.dotProduct(light_direction);

                      if (cosine_angle > 0){
                          //if angle is positive
                          //test for shadows
                          bool shadowed = false;

                          Vector distance_to_light = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();
                          float distance_to_light_magnitude = distance_to_light.magnitude();

                          Ray shadow_ray(intersection_position, light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize());
                          
                          //create ray for each light source
                          vector<double> secondary_intersections;
                          for (int object_index = 0; object_index< scene_objects.size() && shadowed == false; object_index++){
                            secondary_intersections.push_back(scene_objects.at(object_index)->findIntersection(shadow_ray));
                          }

                          //if secondary intersection value exists 
                          for (int c = 0; c< secondary_intersections.size(); c++){
                            if(secondary_intersections.at(c) > accuracy){
                              if(secondary_intersections.at(c) <= distance_to_light_magnitude){
                                shadowed = true;
                              }
                              break;
                            }
                          }

                          //modify final color without shadow
                          if(shadowed == false){
                            final_color = final_color.colorAdd(valid_object_color.colorMultiply(light_sources.at(light_index)->getLightColor()).colorScalar(cosine_angle));
                            //apply reflectivness
                            if(valid_object_color.getSpecial() > 0 && valid_object_color.getSpecial() <= 1){
                              //special between 0 and 1
                              double dot1 = valid_object_normal.dotProduct(intersection_ray_direction.negative());
                              Vector scalar1 = valid_object_normal.vectMultiply(dot1);
                              Vector add1 = scalar1.vectAdd(intersection_ray_direction);
                              Vector scalar2 = add1.vectMultiply(2);
                              Vector add2 = intersection_ray_direction.negative().vectAdd(scalar2);
                              Vector reflection_direction = add2.normalize();

                              double specular = reflection_direction.dotProduct(light_direction);
                              if(specular > 0){
                                specular = pow(specular, 10);
                                final_color = final_color.colorAdd(light_sources.at(light_index)->getLightColor().colorScalar(specular*valid_object_color.getSpecial()));

                              }
                            } 
                          }
                      }
                   }
                   return final_color.trim();
                 }


#endif