#ifndef _SOURCE_H
#define _SOURCE_H
#include "math.h"

class Source {
    public:

    Source();

    virtual Vector getLightPosition() {return Vector(0,0,0);}
    virtual Color getLightColor() {return Color(1,1,1,0); }

};

Source::Source() {}

#endif