#ifndef _VALIDOBJECT_H
#define _VALIDOBJECT_H

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

using std::vector;

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

#endif