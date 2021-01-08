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
#include "winningObjects.h"

using namespace std;



int main(int argc, char *argv[]) {
  cout << "rendering..." << endl;

  //size of frame
  int width = 640, height = 480;
  double aspectratio = (double)width/(double)height;

  int dpi = 72;
  int n = width*height; //total number of pixles
  RGBType *pixels = new RGBType[n]; //define pixel array
  
  int singlePX; //individual pixle

  double xAmount, yAmount; //slightly left and right of camera

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

  //light source
  Vector light_position(-7,10,-10);
  Light scene_light (light_position, white_light);

  //scene objects
  vector<Object*> scene_objects; //array of objects
  //individual objects
  Sphere scene_sphere(Origin, 1, grass_green);
  Plain scene_plain(Y, -1, maroon); //place plane below objects
  //add objects to array of all objects in the scene
  scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
  scene_objects.push_back(dynamic_cast<Object*>(&scene_plain));

  //loop over each px in frame
  for (int x = 0; x < width; x++){
    for (int y = 0; y < height; y++){
      singlePX = y*width + x; //get individual pixel

      //start with no anti-aliasing
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
      int index_of_winning_objects = winningObjectIndex(intersections);
     


	  //for testing
      if((x > 200 && x < 440) && (y> 200 && y < 280))  {
        pixels[singlePX].r = 1;
        pixels[singlePX].g = 0;
        pixels[singlePX].b = 1;
      }
      else{
        pixels[singlePX].r = 0;
        pixels[singlePX].g = 0;
        pixels[singlePX].b = 0;

      }

      // return color for each pixle
    }
  }

  //just save as png instead
  savebmp("scene.png",width,height,dpi,pixels);

  return 0;
}



