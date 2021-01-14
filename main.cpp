
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
#include "finalColor.h"
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

using namespace std;

int main(int argc, char *argv[]) {
  cout << "rendering..." << endl;
  clock_t t1, t2;
  t1 = clock();

  //size of frame
  //int width = 720, height = 480; // for quick render testing
  int width = 3840, height = 2160; //4k
  int aadepth = 8; //antialiasing -> 1 = no AA (samples per pixel)

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
  Color grass_green(0.5, 1.0, 0.5, 0.3);
  Color gray(0.5, 0.5, 0.5, 0);
  Color black(0.0, 0.0, 0.0, 0);
  Color maroon(0.5, 0.25, 0.25, 0.5);
  Color fire_red(1.0, 0.0, 0.0, 0.4);
  Color purpplry_green(0.7,1, 0.3, 0.5);
  Color light_blue(0.6,0.6,1,0.5);
  Color purple(1,0,1,0.2);
  Color yellow(1,1,0,0.6);

  //light
  Color blue_light(0.5,0.5,1,0.0);
  Color red_light(1.0, 0.0, 0.0, 0.0);
  Color high_noon(1,0.8392,0.67,0);
  Color white_light(1.0, 1.0, 1.0, 0);

  //special colors
  Color checkerBoard(1,1,1,2);
  Color chrome(0,0,0,0.9999);

  //light source
  Vector light_position(-7,10,-10);
  Light scene_light (light_position, white_light);
  Light scene_light2 (light_position, blue_light);
  Light scene_light3 (Vector(7,10,-10), red_light);
  Light scene_light4 (Vector(0,10.3,0), white_light);
  Light scene_light5 (light_position, high_noon);

  vector<Source*> light_sources;
  light_sources.push_back(dynamic_cast<Source*>(&scene_light));
  //light_sources.push_back(dynamic_cast<Source*>(&scene_light2));
  //light_sources.push_back(dynamic_cast<Source*>(&scene_light3));
  //light_sources.push_back(dynamic_cast<Source*>(&scene_light4));
  //light_sources.push_back(dynamic_cast<Source*>(&scene_light5));


  //scene objects
  vector<Object*> scene_objects; //array of objects
  //individual objects
  Sphere scene_sphere(Origin, 1, light_blue);
  Sphere scene_sphere2(Vector(-1.8,0,-1), 0.5, fire_red);
  Sphere scene_sphere3(Vector(1.95,0,1), 0.5, grass_green);
  Sphere scene_sphere4(Vector(-18,0.3,5), 3, maroon);
  Sphere scene_sphere5(Vector(-4,3,4), 3, yellow);
  Sphere scene_sphere6(Vector(2.3,-0.7,-0.2), 0.2, purple);
 

  Plain scene_plain(Y, -1, checkerBoard); //place plane below objects
  Plain scene_plain2(Z, 10, black); 
  Plain scene_plain3(X, -20, black); 
  Plain scene_plain4(Z, -5, black); 
  Plain scene_plain5(X, 5, black); 

  //add objects to array of all objects in the scene
  //objects
  scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
  scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere2));
  scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere3));
  scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere4));
  scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere5));
  scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere6));

  
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

          progress = progress + 1; // increment progress       
        }
        //log progress
        current = (progress/total)*100;
        int barWidth = 70;

        std::cout << "[";
        int pos = barWidth * (current/100);
        for (int i = 0; i < barWidth; ++i) {
          if (i < pos) std::cout << "=";
          else if (i == pos) std::cout << ">";
          else std::cout << " ";
        }
        std::cout << "] " << current << "%\r";
        std::cout.flush();
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




