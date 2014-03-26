#ifndef _SPHERE_H
#define _SPHERE_H
#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include "Vector3f.h"
using std::vector;

/* Basic Sphere class that has properties of physics particular to this game, NOTE the draw function will
 * draw without color, and this class is intended to be the backbone for other classes to inherit.
 */
class Sphere{
	
    // NOTE no heap memory will be allocated within this class so a destructor other than clear wont be necissary
    public:
	//construction and = functions
        Sphere();	//default constructor:the sphere will have a radius and mass but no position or ray or velocity
	Sphere(float radius, Vector3f position, Vector3f velocity); //specific constructor         
	Sphere const & operator= (Sphere const & other);	//standard = operation is all we need
        
	//acess functions
	Vector3f getPosition();	//returns the position vector
	float getRadius();	//returns the radius of the sphere
	Vector3f getVelocity();	//returns the velocity that the sphere is traveling in 
	float getMass();	//returns the mass as a float

	//utility functions
	void move();		//updates the position vector based on the ray and velocity
	void collision(Sphere & other);	// will update the spheres on both parts of the collision
	bool colliding(const Sphere & other);
	void wallCollide(float Wradius);
	bool wallColliding(float Wradius);
	void topbotCollide(float height);
	bool topbotColliding(float height);
	void draw();		//NOTE doesnt include color you have to specify that in your display function
	
	

    protected:
	float _radius,_mass;	//mass will be made from radius with the same density
	Vector3f _position, _velocity;	//the ray will just be theta and phi so the 3rd element of vector3f will always be 0

	void  _copy(Sphere const & other);
        void _clear();

	//helper function to check to see if a collision has occured
};
#endif


