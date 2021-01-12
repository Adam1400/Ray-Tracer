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

using namespace std;


int validObjectIndex(vector<double> object_intersections){
    //return the index of valid (intersecting) objects

    double max;
    int index_of_minimum_values;
    //prevent unnessary calculations
    //only worried about rays that intersect in the scene 
    if(object_intersections.size() == 0){
        //no intersections
        return -1;
    }
    else if (object_intersections.size() == 1){
        if(object_intersections.at(0) > 0){
            //intersection is greater than 0 its minimum index is 1
            return 0; //index of 1 item
        }
        else{
            //intersections occur elsewhere (negitive values)
            return -1;
        }
    }
    else{
        //determine intersections closest to camera
        //find maximum value of intersections
        max = 0;
        for(int i =0; i< object_intersections.size(); i++){
            if(max < object_intersections.at(i)){
                max = object_intersections.at(i);
            }
        }

        //then starting from max val find minimum positive value
        if (max > 0){
            //only look at positive intersections
            for (int index = 0; index < object_intersections.size(); index++){
               if (object_intersections.at(index) > 0 && object_intersections.at(index) <= max){
                   max = object_intersections.at(index);
                   index_of_minimum_values = index;
               } 
            }
            return index_of_minimum_values;
        }
        else{
            //all intersections were negitive
            return -1;
        }
    }
}

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
                       valid_object_color.setRed(0);
                       valid_object_color.setGreen(0);
                       valid_object_color.setBlue(0);
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
                          for (int c =0; c< secondary_intersections.size(); c++){
                            if(secondary_intersections.at(c) > accuracy){
                              if(secondary_intersections.at(c) <= distance_to_light_magnitude){
                                shadowed = true;
                              }
                            }
                            break;
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


int main(int argc, char *argv[]) {
  cout << "rendering..." << endl;
  clock_t t1, t2;
  t1 = clock();

  //size of frame
  int width = 3840, height = 2160; //4k
  int aadepth = 8; //antialiasing -> 1 = no AA 

  double aspectratio = (double)width/(double)height;
  double aathreashold = 0.1;
  int dpi = 72;
  int n = width*height; //total number of pixles
  RGBType *pixels = new RGBType[n]; //define pixel array
  
  int singlePX, aa_index; //individual pixle

  double xAmount, yAmount; //slightly left and right of camera

  double tempRed, tempGreen, tempBlue;//pixle values before anialiasing

  //amount of light
  double ambientlight = 0.2;
  double accuracy = 0.0000001;

  //progress 
  double progress = 0, total = height*width, current; 


  // origin vector
  Vector Origin(0,0,0);

  //define 3 dementions
  Vector X (1,0,0);
  Vector Y (0,1,0);
  Vector Z (0,0,1);
  
  //define camera position
  Vector camPos(3, 1.5, -4); //needs to be in positive y
  Vector look_at(0,0,0);
  Vector diff_btw(camPos.getVectX() - look_at.getVectX(),
  				        camPos.getVectY() - look_at.getVectY(),
  				        camPos.getVectZ() - look_at.getVectZ());

  //camera direction
  Vector camDir = diff_btw.negative().normalize();
  Vector camRight = Y.crossProduct(camDir).normalize();
  Vector camDown = camRight.crossProduct(camDir);
  Camera scene_cam(camPos, camDir, camRight, camDown);

  //define some colors
  Color white_light(1.0, 1.0, 1.0, 0);
  Color grass_green(0.5, 1.0, 0.5, 0.3);
  Color gray(0.5, 0.5, 0.5, 0);
  Color black(0.0, 0.0, 0.0, 0);
  Color maroon(0.5, 0.25, 0.25, 0);
  Color fire_red(1.0, 0.0, 0.0, 0.4);
  Color purpplry_green(0.7,1, 0.3, 0.5);
  Color light_blue(0.6,0.6,1,0.5);
  Color checkerBoard(1,1,1,2);

  //light source
  Vector light_position(-7,10,-10);
  Light scene_light (light_position, white_light);
  vector<Source*> light_sources;
  light_sources.push_back(dynamic_cast<Source*>(&scene_light));


  //scene objects
  vector<Object*> scene_objects; //array of objects
  //individual objects
  Sphere scene_sphere(Origin, 1, light_blue);
  Sphere scene_sphere2(Vector(-1.8,0,-1), 0.5, fire_red);
  Sphere scene_sphere3(Vector(1.8,0,1), 0.5, grass_green);


  Plain scene_plain(Y, -1, checkerBoard); //place plane below objects
  Plain scene_plain2(Z, 5, black); 
  Plain scene_plain3(X, -5, black); 
  Plain scene_plain4(Z, -5, black); 
  Plain scene_plain5(X, 5, black); 

  //add objects to array of all objects in the scene
  //objects
  scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
  scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere2));
  scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere3));

  //plains
  scene_objects.push_back(dynamic_cast<Object*>(&scene_plain5));
  scene_objects.push_back(dynamic_cast<Object*>(&scene_plain4));
  scene_objects.push_back(dynamic_cast<Object*>(&scene_plain3));
  scene_objects.push_back(dynamic_cast<Object*>(&scene_plain2));
  scene_objects.push_back(dynamic_cast<Object*>(&scene_plain));
  

  //loop over each px in frame
  for (int x = 0; x < width; x++){
    for (int y = 0; y < height; y++){
      singlePX = y*width + x; //get individual pixel

      
      //start with blank pixel
      double tempRed[aadepth*aadepth];
      double tempGreen[aadepth*aadepth];
      double tempBlue[aadepth*aadepth];

      //loop for anti aliasing scale 
      for(int aax = 0; aax< aadepth; aax++){
        for(int aay =0; aay< aadepth; aay++){

          aa_index = aay*aadepth + aax;

          srand(time(0));

          //creare cray from cam to this pixle

          if(aadepth == 1){
            //no antialiasing
          
              if (width > height){
                // the image is wider than it is tall
                xAmount = ((x+0.5)/width) * aspectratio - (((width-height)/(double)height)/2);
                yAmount = ((height - y) + 0.5) / height;
              }
              else if (height > width){
                //image is taller than it is wide
                xAmount = (x +0.5)  / width;
                yAmount = (((height -y) +0.5) /height) / aspectratio - (((height - width)/ (double)width)/2);
              }
              else{
                // the image is square
                xAmount = (x + 0.5)/width;
                yAmount = ((height - y) + 0.5)/height;
              }
          }
          else{
            //apply anti aliasing

            if (width > height){
                // the image is wider than it is tall
                xAmount = (( x + (double)aax / ((double)aadepth - 1))/width) * aspectratio - (((width-height)/(double)height)/2);
                yAmount = ((height - y) + (double)aax / ((double)aadepth - 1)) / height;
              }
              else if (height > width){
                //image is taller than it is wide
                xAmount = (x + (double)aax / ((double)aadepth - 1))  / width;
                yAmount = (((height -y) + (double)aax / ((double)aadepth - 1)) /height) / aspectratio - (((height - width)/ (double)width)/2);
              }
              else{
                // the image is square
                xAmount = (x + (double)aax / ((double)aadepth - 1))/width;
                yAmount = ((height - y) + (double)aax / ((double)aadepth - 1))/height;
              }

          }

              //create camera rays
              Vector cam_ray_origin = scene_cam.getCamPos();
              Vector cam_ray_direction = camDir.vectAdd(camRight.vectMultiply(xAmount - 0.5).vectAdd(camDown.vectMultiply(yAmount - 0.5))).normalize();
              Ray cam_ray(cam_ray_origin, cam_ray_direction);

              //look for all ray intersections
              vector<double> intersections; //create array of intersections

              for (int index = 0; index < scene_objects.size(); index++){
                //add intersections to array
                intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray));
              }

              //pick out intersecting objects
              int index_of_valid_objects = validObjectIndex(intersections);
            

            //color each pixel
              if(index_of_valid_objects == -1)  {
                //when no intersection
                //set background to black
                tempRed[aa_index] = 0;
                tempGreen[aa_index] = 0;
                tempBlue[aa_index] = 0;
              }
              else{
                //intersection occurs 
                //color coresponds to object in the scene 
                if (intersections.at(index_of_valid_objects) > accuracy){

                  //determine position and direction vectors at intersection
                  Vector intersection_position = cam_ray_origin.vectAdd(cam_ray_direction.vectMultiply(intersections.at(index_of_valid_objects)));
                  Vector intersection_ray_direction = cam_ray_direction;

                  Color intersection_color = getColorAt(intersection_position,
                                                        intersection_ray_direction,
                                                        scene_objects, 
                                                        index_of_valid_objects,
                                                        light_sources,
                                                        accuracy,
                                                        ambientlight);

                  

                  tempRed[aa_index] = intersection_color.getRed();
                  tempGreen[aa_index] = intersection_color.getGreen();
                  tempBlue[aa_index] = intersection_color.getBlue();
                }
              }   
            }
          }
          // return color average for each pixle
          double totalRed = 0;
          double totalGreen = 0;
          double totalBlue = 0;

          for(int iRed = 0; iRed < aadepth * aadepth; iRed++){
            totalRed = totalRed + tempRed[iRed];
          }
           for(int iGreen = 0; iGreen < aadepth * aadepth; iGreen++){
             totalGreen =  totalGreen + tempGreen[iGreen];           
          }
           for(int iBlue = 0; iBlue < aadepth * aadepth; iBlue++){
             totalBlue = totalBlue + tempBlue[iBlue];            
          }

          double avgRed = totalRed/(aadepth*aadepth);
          double avgGreen = totalGreen/(aadepth*aadepth);
          double avgBlue = totalBlue/(aadepth*aadepth);

          //set final pixle color
          pixels[singlePX].r = avgRed;
          pixels[singlePX].g = avgGreen;
          pixels[singlePX].b = avgBlue;

          //log progress
          progress = progress + 1;
          current = (progress/total)*100;

          int barWidth = 70;

          std::cout << "[";
          int pos = barWidth * (current/100);
          for (int i = 0; i < barWidth; ++i) {
              if (i < pos) std::cout << "=";
              else if (i == pos) std::cout << ">";
              else std::cout << " ";
          }
          std::cout << "] " << current << " %\r";
          std::cout.flush();
                
        }
      }

      

  //just save as png instead
  savebmp("scene.png",width,height,dpi,pixels);

  delete pixels, tempRed, tempGreen, tempBlue; //free memory

  //render time calc
  t2 = clock();
  float diff = ((float)t2 - (float)t1)/1000;
  cout << diff << " seconds" << endl;

  return 0;
}




