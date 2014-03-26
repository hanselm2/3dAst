#ifndef _VECTOR3f_H
#define _VECTOR3f_H

#include<iostream>

class Vector3f{

    public:
        Vector3f();
        Vector3f(float x, float y, float z);
		
        float distance(Vector3f const other);
	float magnitude() const;
	
	Vector3f operator*(float number) const;
	Vector3f operator/(float number) const;
	Vector3f operator+(const Vector3f & other)const;
	Vector3f operator-(const Vector3f & other)const;
	Vector3f operator-()const;
	Vector3f const & operator= (Vector3f const & other);

	Vector3f normalize() const;
	float dotProduct(const Vector3f  & other) const;
	Vector3f crossProduct(const Vector3f & other) const;
		
		
	
	float values[3];
        
    private:
        void _copy(Vector3f const & other);
        void _clear();
};
#endif
