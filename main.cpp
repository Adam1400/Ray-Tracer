
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

#include "extentions/savebmp.h"
#include "extentions/finalColor.h"
#include "extentions/vectors.h"
#include "extentions/rays.h"
#include "extentions/camera.h"
#include "extentions/colors.h"
#include "extentions/lighting.h"
#include "extentions/objects.h"
#include "extentions/spheres.h"
#include "extentions/plains.h"
#include "extentions/source.h"
#include "extentions/validObject.h"
#include "extentions/render.h"

using namespace std;

int main(int argc, char *argv[]) {

  //RENDER SETTINGS
  int width = 3840, height = 2160; //4k
  //int width = 720, height = 480;
  int aadepth = 8;                 //antialiasing -> 1 = no AA (samples per pixel)
  double ambientlight = 0.2;       //amount of light
 
  //define camera position
  Vector camPos(2.5, 1.7, -1.0);      //needs to be in positive y
  Vector look_at(1,2,0);

  //output file path and name
  char fileName[] = "output/test.png";
  

  //OBJECTS AND LIGHT
  //define some colors
  Color grass_green(0.5, 1.0, 0.5, 0.5);
  Color gray(0.5, 0.5, 0.5, 0);
  Color black(0.0, 0.0, 0.0, 0);
  Color maroon(0.5, 0.25, 0.25, 0.5);
  Color fire_red(1.0, 0.0, 0.0, 0.0);
  Color fire_red2(1.0, 0.0, 0.0, 0.45);
  Color fire_red3(1.0, 0.0, 0.0, 0.9);
  Color purpplry_green(0.7,1, 0.3, 0.5);
  Color light_blue(0.6,0.6,1,0.5);
  Color purple(1,0,1,0.2);
  Color yellow(1,1,0,0.9);

  //light source colors
  Color blue_light(0.5,0.5,1,0.0);
  Color red_light(1.0, 0.0, 0.0, 0.0);
  Color high_noon(1,0.8392,0.67,0);
  Color white_light(1.0, 1.0, 1.0, 0);

  //special colors
  Color checkerBoard(1,1,1,2);
  Color chrome(0,0,0,0.9999);

  //define light source
  Vector light_position(-7,10,-10);
  Light scene_light (light_position, white_light);
  Light scene_light2 (light_position, blue_light);
  Light scene_light3 (Vector(7,10,-10), red_light);
  Light scene_light4 (Vector(0,10.3,0), white_light);
  Light scene_light5 (light_position, high_noon);

  vector<Source*> light_sources; //add light sources to array
  light_sources.push_back(dynamic_cast<Source*>(&scene_light));
  //light_sources.push_back(dynamic_cast<Source*>(&scene_light2));
  //light_sources.push_back(dynamic_cast<Source*>(&scene_light3));
  //light_sources.push_back(dynamic_cast<Source*>(&scene_light4));
  //light_sources.push_back(dynamic_cast<Source*>(&scene_light5));

  // Generic vectors 
  Vector Origin(0,0,0);
  Vector X (1,0,0);
  Vector Y (0,1,0);
  Vector Z (0,0,1);

  //scene objects
  vector<Object*> scene_objects; //array of objects
  
  Sphere scene_sphere(Origin, 1, light_blue); //objects first
  Sphere scene_sphere2(Vector(-1.8,0,-1), 0.5, fire_red);
  Sphere scene_sphere3(Vector(1.95,0,1), 0.5, grass_green);
  Sphere scene_sphere4(Vector(-18,0.3,5), 3, maroon);
  Sphere scene_sphere5(Vector(-4,3,4), 3, yellow);
  Sphere scene_sphere6(Vector(2.3,-0.7,-0.2), 0.2, purple);
  Sphere stack2(Vector(0,2,0), 1, light_blue);
  Sphere stack3(Vector(0,4,0), 1, fire_red);
  Sphere moon(Vector(1.0,1.7,-0.6), .15, fire_red);
  Sphere moon2(Vector(1.35,2,-0), .15, fire_red2);
  Sphere moon3(Vector(1.625,2.3,0.6), .15, fire_red3);

  
  

 
  Plain scene_plain(Y, -1, checkerBoard); //place plane below objects
  Plain scene_plain2(Z, 20, black); 
  Plain scene_plain3(X, -20, black); 
  Plain scene_plain4(Z, -20, black); 
  Plain scene_plain5(X, 20, black); 

  //add objects to array of all objects in the scene
  //scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere)); //objects
  //scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere2));
  //scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere3));
  //scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere4));
  //scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere5));
  //scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere6));
  scene_objects.push_back(dynamic_cast<Object*>(&stack2));
  //scene_objects.push_back(dynamic_cast<Object*>(&stack3));
  scene_objects.push_back(dynamic_cast<Object*>(&moon));
  scene_objects.push_back(dynamic_cast<Object*>(&moon2));
  scene_objects.push_back(dynamic_cast<Object*>(&moon3));



  scene_objects.push_back(dynamic_cast<Object*>(&scene_plain5)); //plains
  scene_objects.push_back(dynamic_cast<Object*>(&scene_plain4));
  scene_objects.push_back(dynamic_cast<Object*>(&scene_plain3));
  scene_objects.push_back(dynamic_cast<Object*>(&scene_plain2));
  scene_objects.push_back(dynamic_cast<Object*>(&scene_plain));

  render(width, height, aadepth, ambientlight, fileName, camPos, look_at, light_sources, scene_objects);
 
  return 0;
}




