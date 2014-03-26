#include <iostream> 
#include "Vector3f.h"
#include <cmath>
Vector3f::Vector3f()
{   values[0] = 0;
    values[1] = 0;
    values[2] = 0;
}

Vector3f::Vector3f(float x, float y, float z)
{   values[0] = x;
    values[1] = y;
    values[2] = z;
}

float Vector3f::distance(Vector3f const other){
	float dist = sqrt( (values[0]-other.values[0])*(values[0]-other.values[0]) + (values[1]-other.values[1])*(values[1]-other.values[1]) + (values[2]-other.values[2])*(values[2]-other.values[2]) );
	return dist;
}

float Vector3f::magnitude() const{
	return sqrt( (values[0]*values[0])+(values[1]*values[1])+(values[2]*values[2]) );
}

Vector3f Vector3f::operator* (float number)const{
	return Vector3f(values[0]*number, values[1]*number, values[2]*number);
}

Vector3f Vector3f::operator/ (float number)const{
	return Vector3f(values[0]/number, values[1]/number, values[2]/number);
}

Vector3f Vector3f::operator- (const Vector3f & other)const{
	return Vector3f(values[0]-other.values[0], values[1]-other.values[1], values[2]-other.values[2]);
}

Vector3f Vector3f::operator- () const{
	return Vector3f(-values[0], -values[1], -values[2]);
}

Vector3f Vector3f::operator+ (const Vector3f & other)const{
	return Vector3f(values[0]+other.values[0], values[1]+other.values[1], values[2]+other.values[2]);
}

Vector3f Vector3f::normalize()const{
	float mag = magnitude();
	return Vector3f(values[0]/mag, values[1]/mag, values[2]/mag);
}

float Vector3f::dotProduct(const Vector3f & other) const{
	return values[0] * other.values[0] + values[1] * other.values[1] + values[2] * other.values[2];
}

Vector3f Vector3f::crossProduct(const Vector3f & other) const{
	return Vector3f( (values[1]*other.values[2]-values[2]*other.values[1]),
			 (values[2]*other.values[0]-values[0]*other.values[2]),
			 (values[0]*other.values[1]-values[1]*other.values[0]) );
}

Vector3f const & Vector3f::operator= (Vector3f const & other)
{
    if (this != &other)
    {
        _clear();
        _copy(other);
    }
    return *this;
}

void Vector3f::_copy(Vector3f const & other)
{
    values[0] = other.values[0];
    values[1] = other.values[1];
    values[2] = other.values[2];
}

void Vector3f::_clear()
{
    values[0] = 0;
    values[1] = 0;
    values[2] = 0;
}

