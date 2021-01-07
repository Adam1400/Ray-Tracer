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

using namespace std;



int main(int argc, char *argv[]) {
  cout << "rendering..." << endl;

  //size of frame
  int width = 640, height = 480;

  int dpi = 72;
  int n = width*height; //total number of pixles
  RGBType *pixels = new RGBType[n]; //define pixel array
  
  int singlePX; //individual pixle

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

  //light source
  Vector light_position(-7,10,-10);
  Light scene_light (light_position, white_light);


  //loop over each px in frame
  for (int x = 0; x < width; x++){
    for (int y = 0; y < height; y++){
      //get individual pixel
      singlePX = y*width + x;

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



