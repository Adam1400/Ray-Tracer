#ifndef _COLORS_H
#define _COLORS_H
#include "math.h"

class Color {
  //default color grey
  double red = 0.5, green = 0.5, blue = 0.5, special = 0; 
  //special is reflectiveness ratio... 0 is matt 1 is chrome 
  
  //construct
  public:
  Color (double, double, double, double);

  //methods
  double getRed() { return red; }
  double getGreen() { return green; }
  double getBlue() { return blue; }
  double getSpecial() { return special; }

  void setRed(double redValue) { red = redValue; }
  void setGreen(double greenValue) { green = greenValue; }
  void setBlue(double blueVale) { blue = blueVale; }
  void setSpecial(double specialValue) { special = specialValue; }
  
};

Color::Color(double r, double g, double b, double s){
    red = r;
    green = g;
    blue = b;
    special = s;

}

#endif