#include "Bullet.h"
#include <GL/glut.h>

Bullet::Bullet() :Sphere()
{	timealive = 0;
	maxtime = 0;
}

Bullet::Bullet(float radius, Vector3f position, Vector3f velocity, unsigned int time) :Sphere(radius,position,velocity)
{	timealive = 0;
	maxtime = time;
	Sphere::_mass = _mass * 10;//want our bullets to pack an umph
}

bool Bullet::shouldDelete(){
	return timealive > maxtime;
}

Bullet const & Bullet::operator= (Bullet const & other)
{
        if (this != &other)
        {       _clear();
                _copy(other);
        }
        return *this;
}


void Bullet::move(){
	timealive++;
	Sphere::move();
}

void Bullet::draw(){
	glColor4f(1.0, 1.0, 1.0, 1.0);	//solid white balls are our bullets
	Sphere::draw();
}

void Bullet::_copy(Bullet const & other){
	_radius = other._radius;
        _velocity = other._velocity;
        _mass = other._mass;
        _position = other._position;
	maxtime = other.maxtime;
        timealive = other.timealive;
}

void Bullet::_clear(){
	_radius = 0;
        _mass = 0;
        Vector3f pos(0.f,0.f,0.f);
        pos = _position;
        Vector3f ray(0.f,0.f,0.f);
        ray = _velocity;

        maxtime = 0;
        timealive = 0;
}

