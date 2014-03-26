#ifndef _ASTEROID_H_
#define _ASTEROID_H_
#include "Sphere.h"
#include "Vector3f.h"
/* A simple class that inherits sphere and will be used in the asteroid game. Important properties are its health and level
 * each time it colides with a bullet reduce its health by one, it is most practicle to do this in the main.cpp. Also,
 * when it reaches 0 health it will level down, deleting itself and spawning 2 more asteroids in its place each one level 
 * less
 */
class Sphere;

class Asteroid : public Sphere 
{
	
	public:
		Asteroid();
		Asteroid(float radius, Vector3f position, Vector3f velocity, unsigned int level);
		
		bool takeHit();	//once a hit has been registered this function will be called, the true false part is if we should kill asteroid
		Asteroid const & operator= (Asteroid const & other);
		int getLevel();		
		void draw();		

	private:
	void _copy(Asteroid const & other);
	void _clear();
	int _health;
	unsigned int _level;
};
#endif
