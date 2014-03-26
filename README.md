This Program was made to compile and run on a unix based system such as linux or Mac. There have been errors in compiliation with the
makefile for Linux machines if they do not have glut installed on their machine. If this is the case please download freeglut and install.
Further, if there is a compilation error saying that glut.h is not found the normal fix for this is to change the #include statement
in the Sphere.h, Bullet.h, Asteroid.h and main1.cpp file to read #include <GL/glut.h> instead of #include <GL/glu.h> 

Once compiled make sure to run with 3d acceleration enabled.

Gameplay:
	The game will automatically start, and the controls are very sensitive. If you run into either a wall or a asteroid you "die"
	and the program exits. If you run into the ceiling or floor you are teleported to the opposite end with all physics kept 
	in place. 
	Controls:
		Spacebar: shoots bullets on a small but set cooldown. If a bullet hits the wall it will richoet off. If a bullet
		hits the ceiling or floor it will teleport to the opposite end and contineu with all physics kept in place. If
		a bullet hits an asteroid it will disapear, after a set number of hits the asteroid will split. Clear all asteroids
		to advance to harder levels
		
		w: Move forward, this is a very fast accelerate and the ship will passively decrease in acceleration over time
	
		s: Move backward, same sensitivity as the w button and same rules of decay on movement

		click and drag left mouse: Moves the orientation of the Ship and camera. This is how you steer. The camera will 
		always be attached to the ship and so navigatino will feel natural

		j: Turbo. Moves you forwards at a much faster rate while pressed

		esc: exit program

	