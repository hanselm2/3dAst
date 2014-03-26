#ifndef _BULLET_H_
#define _BULLET_H_
#include "Sphere.h"
#include "Vector3f.h"

/*	The bullet class is essentially the exact same as the Sphere class except that it will expire after 
 *	some count of time has been reached and then be deleted, the deletion will be done in the main.cpp.
 *	This is because i am unsure if just reducing all of its variables to 0 will correctly remove it from memory
 */
class Sphere;

class Bullet : public Sphere 
{
	
	public:
	Bullet();
	Bullet (float radius, Vector3f position, Vector3f velocity, unsigned int time);	

	bool shouldDelete();	//returns if we should delete the bullet
	void move();	//same as sphere but update timealive by 1;
	void draw();	//just draws a sphere of a particular color  	
	Bullet const & operator= (Bullet const & other);	

	private:

	int timealive;
	int maxtime;	// change this to change the amount of time for a bullet to be allowed on screen:60 = 1second
	void _copy(Bullet const & other);
	void _clear();
};
#endif 
