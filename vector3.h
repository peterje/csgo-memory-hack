//
// Created by Peter on 5/3/2021.
//

#ifndef HACK_VECTOR3_H
#define HACK_VECTOR3_H
#include <math.h>


class vector3 {
public:
    vector3();
    vector3(float x1,float y1,float z1=0);
    float x, y, z;
    double distance(vector3 other) const;
    double magnitude() const;
    vector3 operator+(const vector3 &vec) const;    //addition
    vector3 operator-(const vector3 &vec) const;    //addition

};


#endif //HACK_VECTOR3_H
