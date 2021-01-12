#ifndef _COLORS_H
#define _COLORS_H
#include "math.h"

class Color {
  //default color grey
  double red = 0.5, green = 0.5, blue = 0.5, special = 0; 
  //special is reflectiveness ratio... 0 is matt 1 is chrome 
  // 2 corrisponds to a checkerboard pattern
  
  //construct
  public:
  Color (double, double, double, double);

  //methods
  double getRed() { return red; }
  double getGreen() { return green; }
  double getBlue() { return blue; }
  double getSpecial() { return special; }

  double setRed(double redValue) { red = redValue; }
  double setGreen(double greenValue) { green = greenValue; }
  double setBlue(double blueVale) { blue = blueVale; }
  double setSpecial(double specialValue) { special = specialValue; }

  //for shadows
  double brightness(){ return (red + green + blue) / 3;}

  Color colorScalar(double scalar) { return 
                                     Color(red*scalar, 
                                          green*scalar,
                                          blue*scalar,
                                          special);}

  Color colorAdd(Color color){ return 
                               Color(red + color.getRed(),
                                    green + color.getGreen(),
                                    blue + color.getBlue(), 
                                    special);}

  Color colorMultiply(Color color){ return
                                    Color(red * color.getRed(),
                                         green * color.getGreen(),
                                         blue * color.getBlue(), 
                                         special);}

  Color colorAverage (Color color) { return 
                                     Color((red + color.getRed())/2,
                                          (green + color.getGreen())/2,
                                          (blue + color.getBlue())/2,
                                           special);}
  
  Color trim() {
    //trim light values so they fall within 0 - 1
    double allLight = red + green + blue;
    double excessLight = allLight - 3;
    if(excessLight > 0.0){

      red = red + excessLight*(red/allLight);
      green = green + excessLight*(green/allLight);
      blue = blue + excessLight*(blue/allLight);
    }

      if(red > 1){ red = 1; }
      if(green > 1){ green = 1; }
      if(blue > 1){ blue = 1; }

      if(red < 0){ red = 0; }
      if(green < 0){ green = 0; }
      if(blue < 0){ blue = 0; }
    

    return Color(red, green, blue, special);
  }
  
  
  
};

Color::Color(double r, double g, double b, double s){
    red = r;
    green = g;
    blue = b;
    special = s;

}

#endif