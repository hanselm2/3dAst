#include "Sphere.h"
#include <math.h>
#include <GL/glu.h>
#include <iostream> //used for troubleshooting collisions
#include "Vector3f.h"
using namespace std;

#define PI 3.141592658

//the default radius is .1 and default mass is .1
Sphere::Sphere(){
	_radius = .1f;
	_mass = (4/3)*PI*pow(_radius,3.0);
	Vector3f pos(0.f,0.f,0.f);
	pos = _position;
	Vector3f ray(0.f,0.f,0.f);
	ray = _velocity;
}

Sphere::Sphere(float radius, Vector3f position, Vector3f velocity){
	_radius = radius;
	_position = position;
	_velocity = velocity;
	_mass = (4/3)*PI*pow(_radius,3.0);
}

Sphere const & Sphere::operator= (Sphere const & other){
	if (this != &other)
	{	_clear();
		_copy(other);
	}
	return *this;
}

Vector3f Sphere::getPosition(){
	return _position;
}

float Sphere::getRadius(){
	return _radius;
}

Vector3f Sphere::getVelocity(){
	return _velocity;
}

float Sphere::getMass(){
	return _mass;
}

void Sphere::move(){
	_position = _position + _velocity;
}

//using a method of restitution described at videotutorialsrock, also modified his oct-tree to fit this program
void Sphere::collision(Sphere & other){
	if(!colliding(other)) return;	//if they are not colliding do nothing
		
	Vector3f distVect = (_position-other._position).normalize();
	
	_velocity = _velocity - (distVect*2*_velocity.dotProduct(distVect))*(other._mass/_mass);
	other._velocity = other._velocity - (distVect*2*other._velocity.dotProduct(distVect))*(_mass/other._mass);
}

//ONLY USE IN A DISPLAY FUNCTION: We need to have the identity matrix loaded or we will get huge errors on translate
void Sphere::draw(){
	glPushMatrix();
	glTranslatef(_position.values[0], _position.values[1], _position.values[2]);
	glutSolidSphere(_radius,24,24);	//from online sources 12 seems to be the magick number on slices
	glPopMatrix();
}

bool Sphere::colliding(const Sphere & other){
	float distance = _position.distance(other._position);
	float collidingdist = other._radius + _radius;
	//trying to revise code found this double check online, checks to see if their moving away from eachother
	if(distance < collidingdist)
	{	Vector3f netVelocity = _velocity-other._velocity;
		Vector3f displacement = _position-other._position;
		return netVelocity.dotProduct(displacement) < 0;
	}
	else return false;
}

void Sphere::wallCollide(float Wradius){
	//safety check
	if(!wallColliding(Wradius))
		return;
	//same method as spherecollisions, just simple geometry to figure this out
	Vector3f Wnormal = _position.normalize(); //go from the origin to the balls point, this is the direction of our normal
	Wnormal.values[1] = 0;
	_velocity = _velocity-(Wnormal*2*_velocity.dotProduct(Wnormal));
}	

//the second statement is to check to see if its moving away form collision
bool Sphere::wallColliding(float Wradius){
	return sqrt((_position.values[0]*_position.values[0])+(_position.values[2]*_position.values[2]))+_radius>Wradius
		&& _velocity.dotProduct(_position.normalize())>0;
}

void Sphere::topbotCollide(float height){
	//safety check, make sure they are colliding after this has been called
	if(!topbotColliding(height))
		return;
	//if it colides with a top or bottom telport it the the opposite side but dont change any proporites
	_position.values[1] = -_position.values[1];
}

//want to have the sphere go from the bottom to the top so evaluate at the center of the sphere not the edge
bool Sphere::topbotColliding(float height){
	return abs(_position.values[1])>=height;
}

// HELPER FUNCTIONS BELOW
void Sphere::_copy(Sphere const & other){
	_radius = other._radius;
	_velocity = other._velocity;
	_mass = other._mass;
	_position = other._position;
}

void Sphere::_clear(){
	_radius = 0;
	_mass = 0;
	Vector3f pos(0.f,0.f,0.f);
	pos = _position;
	Vector3f ray(0.f,0.f,0.f);
	ray = _velocity;
}	
