//
// Created by Peter on 5/3/2021.
//

#include "vector3.h"

vector3::vector3(){
    this->x=0;
    this->y=0;
    this->z=0;
}

vector3::vector3(float x1,float y1,float z1){
    this->x=x1;
    this->y=y1;
    this->z=z1;
}
double vector3::distance(vector3 other) const {
    return sqrt(pow(other.x-this->x, 2) + pow(other.y-this->y, 2) + pow(other.z - this->z, 2));
}
vector3 vector3 ::operator+(const vector3 &vec) const
{
    return {x+vec.x,y+vec.y,z+vec.z};
}

vector3 vector3::operator-(const vector3 &vec) const {
    return {x-vec.x,y-vec.y,z-vec.z};
}

double vector3::magnitude() const {
    return sqrt(x*x + y*y + z*z);
}
