#include "Asteroid.h"
#include <GL/glut.h>

Asteroid::Asteroid() :Sphere()
{	_level = 0;
	_health = 0;
}	
//radius of an asteroid will be based on level
Asteroid::Asteroid(float radius, Vector3f position, Vector3f velocity, unsigned int level) :Sphere((radius+.7*(level-1))/3,position,velocity)
{	_level = level;
	_health = level+2;
}

Asteroid const & Asteroid::operator= (Asteroid const & other)
{
	if (this != &other)
        {       _clear();
                _copy(other);
        }
        return *this;
}

bool Asteroid::takeHit(){
	_health--;
	if(_health == 0){
		_level--;
		_health = _level+2;
		return true;
	}
	else return false;
}

void Asteroid::draw(){
	glColor4f(.3,.6,.3,1.f);
	Sphere::draw();
}

int Asteroid::getLevel(){
	return _level;
}

void Asteroid::_copy(Asteroid const & other){
	_radius = other._radius;
        _velocity = other._velocity;
        _mass = other._mass;
        _position = other._position;
	_level = other._level;
	_health = other._health;
}

void Asteroid::_clear(){
	_radius = 0;
        _mass = 0;
        Vector3f pos(0.f,0.f,0.f);
        pos = _position;
        Vector3f ray(0.f,0.f,0.f);
        ray = _velocity;
	_level = 0;
	_health = 0;
}
		
	
