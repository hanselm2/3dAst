#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>//need to change this include to #include <GL/GLUT.h> for xcode. and relevant glut library for linux
#include <vector>
#include <iostream>
#include <cmath>
#include "Sphere.h"
#include "Vector3f.h"
#include "Bullet.h"
#include "Asteroid.h"

using std::vector;
using namespace::std;   //this and iostream for troubleshooting

#define PI 3.141592658

int nFPS = 60;
float sensitivity = .5; // the sensitivity of the camera motion
float enviroRad = 14.322;
float enviroHeight = 6;
int bulletCD=0;
int bulletLifeSpan=600;
unsigned int level = 1;
GLuint _textureId;

//ship member variables: this should be the only object defined in here as it is the protaginist, i could make it its own class
float shipRay[2] = {0.f,0.f};    //represent theta and phi respectively
float shipVeloc = 0.f;        //represents the speed at which it moves in the direction of the ray
float shipPos [3] = {0.f,0.f,0.f};
bool turboOn = false;

//control buffer implementation
bool mouseDown;
bool* keyStates = new bool[256];

//object vectors for things other than ship and enviroment
vector<Asteroid*> asteroids;
vector<Bullet*> bullets;

//vector that will hold all orthonormal vectors for or ship detection
vector<Vector3f> orthonormals;	


int randomPosNeg()
{
	if(((rand() % 2) ==1)) return 1;
	else return -1;
}

float randomFloat()
{
	return randomPosNeg()*(float)rand() / ((float)RAND_MAX);
	
}

float randomPosFloat(){
	return (float)rand() / ((float)RAND_MAX);
}

//helper function for the type of collision detectoin we will use for the ship
void makeOrthos(){
	//will manually input all the polygons to find their normals
	Vector3f ortho1 = (Vector3f(-.2f,0.f,0.5f)-Vector3f(0.f,0.f,-.15f)).crossProduct(Vector3f(0.f,0.1f,.05f)-(Vector3f(0.f,0.f,-.15f))).normalize();
	orthonormals.push_back(ortho1);
	Vector3f ortho2 = (Vector3f(0.f,0.1f,.05f)-(Vector3f(0.f,0.f,-.15f))).crossProduct(Vector3f(.2f,0.f,0.5f)-Vector3f(0.f,0.f,-.15f)).normalize();
	orthonormals.push_back(ortho2);
	Vector3f ortho3 = (Vector3f(.2f,0.f,0.5f)-Vector3f(0.f,0.f,-.15f)).crossProduct(Vector3f(0.f,-0.1f,.05f)-(Vector3f(0.f,0.f,-.15f))).normalize();
	orthonormals.push_back(ortho3);
	Vector3f ortho4 = (Vector3f(0.f,-0.1f,.05f)-(Vector3f(0.f,0.f,-.15f))).crossProduct(Vector3f(-.2f,0.f,0.5f)-Vector3f(0.f,0.f,-.15f)).normalize();
	orthonormals.push_back(ortho4);
	Vector3f ortho5 = (Vector3f(-.3f,-.025,.25f)-(Vector3f(-.1f,0.5f,.05f))).crossProduct(Vector3f(-.3f,0.025f,0.25f)-Vector3f(-.2f,0.f,.05f)).normalize();
	orthonormals.push_back(ortho5);
	Vector3f ortho6 = (Vector3f(-.3f,0.025f,0.25f)-Vector3f(-.2f,0.f,.05f)).crossProduct(Vector3f(-.3f,-.025,.25f)-(Vector3f(-.1f,-0.5f,.05f))).normalize();
	orthonormals.push_back(ortho6);
	Vector3f ortho7 = (Vector3f(.3f,-.025,.25f)-(Vector3f(.1f,-0.5f,.05f))).crossProduct(Vector3f(.3f,0.025f,0.25f)-Vector3f(.2f,0.f,.05f)).normalize();
	orthonormals.push_back(ortho7);
	Vector3f ortho8 = (Vector3f(.3f,0.025f,0.25f)-Vector3f(.2f,0.f,.05f)).crossProduct(Vector3f(.3f,-.025,.25f)-(Vector3f(.1f,0.5f,.05f))).normalize();
	orthonormals.push_back(ortho8);
}
/*
//method is learned and copied from video tutorials rock, textures is my weak point :/
GLuint loadText(Image* image)
{
	GLuint textureId;	//GLuint is just GL's way of saying unsigned it, corresponds to the adress in the texture array
	glGenTextures(1, &textureId);	//allow for 3 textures to be held
	glBindTexture(GL_TEXTURE_2D, textureID) //the push matrix of textures
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return textureId;
}*/

void init(void)
{ 
    	glClearColor(0.3,0.3,0.3,1.0); // clear color is gray       
    	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment this function if you only want to draw wireframe model
	
	//enable depth
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
/*	//enable texturing
	Image* image = loadBMP("vtr.bmp");
	_textureId = loadTexture(image);
	delete image;*/

	/*
   	//trying my hand at lighting
  	glEnable(GL_LIGHTING);
  	glEnable(GL_LIGHT0);
  	glEnable(GL_LIGHT1);
  	glEnable(GL_NORMALIZE); //enable normalization for lighting ?
  	*/
}

/* **** Draw functions will be called here **** */

//test to see if spheres behave like we would like them too, basically test my sphereclass before writing asteroid class
void makeAsteroids(int level){
	for(int i=0; i<2*level+2; i++)
	{	Vector3f velocity, position;
		float radius = .7;
		for(int j=0; j<3; j++)	//awww jheart
		{	velocity.values[j] = randomFloat()*.03+.01;
			position.values[j] = randomPosNeg()*(randomPosFloat()*4 + 1);
		}
		Asteroid * newastr = new Asteroid;
		Asteroid a(radius, position, velocity, level);
		*newastr = a;
		asteroids.push_back(newastr);
	}
}

//holds the actual comand to plot the vertices/make the ships shape
void makeShip()
{
    glColor4f(.19,.91,.98,1.f);  //declare silver
    glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(0.f,0.f,-.15f);
        glVertex3f(-.2f,0.f,.05f);
        glVertex3f(0.f,.1f,.05f);
        glVertex3f(.2f,0.f,.05f);
        glVertex3f(0.f,0.f,-.35f);
        glVertex3f(0.f,-.1f,.05f);
        glVertex3f(-.2f,0.f,.05f);
        glVertex3f(.2f,0.f,.05f);
    glEnd();
    glColor4f(.55,.47,.14,1.f);     //declarebronze
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(-.3f,-.025f,.25f);
        glVertex3f(-.2f,0.f,.05f);
        glVertex3f(-.1f,.05f,.05f);
        glVertex3f(-.1f,-.05f,.05f);
        glVertex3f(-.2f,0.f,.05f);
    glEnd();
    glColor4f(.55,.47,.14,1.f);     //declarebronze
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(.3f,-.025f,.25f);
        glVertex3f(.2f,0.f,.05f);
        glVertex3f(.1f,.05f,.05f);
        glVertex3f(.1f,-.05f,.05f);
        glVertex3f(.2f,0.f,.05f);
    glEnd();
}

int count=0;  //used to test every number of frames
void drawShip()
{   //made this mistake a bunch so far, x is z essentially. use RHR to see that our angle is actually 90 degress off phase to correct for that just make your x calculations your z ones
    if(turboOn){
        shipVeloc = .09;
    }

    shipPos[2] -= shipVeloc * cos(shipRay[1]*PI/180) * cos(shipRay[0]*PI/180);
    shipPos[1] += shipVeloc * sin(shipRay[1]*PI/180);
    shipPos[0] += shipVeloc * cos(shipRay[1]*PI/180) * sin(shipRay[0]*PI/180); //carefull here, -z is into the screen
    /*if(count%20==0){
    cout<<"shipRay[1] = "<<shipRay[1]<<"\t shipRay[0] = "<<shipRay[0]<<"\tshipVeloc = "<<shipVeloc<<"\t";
    cout<<"ship's x = "<<shipPos[0]<<"\tship's y = "<<shipPos[1]<<"\t ship's z = "<<shipPos[2]<<endl;   
    } */
    if(abs(shipPos[1]) >= enviroHeight) shipPos[1] = -shipPos[1];
    if(shipVeloc>=.2 || shipVeloc<=-.2) shipVeloc -= shipVeloc/50;
    else shipVeloc -= shipVeloc/32;
    makeShip();
}

//draw the box we will be inside of
void drawEnviroment()
{
    //we will use a high polygon count for clear texturing when we do it
    float theta = 2.0;  //this represents 360/ and is how much the angle of the frame will change for each next one
    float length = .5;     // the length of each pannel making our cylinder
    float x = 0;
    float z=14.322;	//offset to center the cylinder

    //normal convention that Z is coming out twords us and y is up
    glColor4f(0.0, 0.0, 1.0f, 1.0f);
    glBegin( GL_QUAD_STRIP);
    for(float i=0; i<= 360; i+= theta)
    {   glVertex3f(x, -enviroHeight, z);
        glVertex3f(x, enviroHeight, z);
        x += cos(i*PI/180)*length;
        z -= sin(i*PI/180)*length;
    }            
    glEnd();
    
    glColor4f(0.0f,0.0f,0.0f,1.0f);
    glBegin( GL_QUADS);
    	glVertex3f(-14.5f,enviroHeight,-14.5f);
	glVertex3f(-14.5f,enviroHeight,14.5f);	
	glVertex3f(14.5f,enviroHeight,14.5f);
	glVertex3f(14.5f,enviroHeight,-14.5f);
    glEnd();
    glColor4f(0.0f,0.0f,0.0f,1.0f);
    glBegin( GL_QUADS);
    	glVertex3f(-14.5f,-enviroHeight,-14.5f);
	glVertex3f(-14.5f,-enviroHeight,14.5f);	
	glVertex3f(14.5f,-enviroHeight,14.5f);
	glVertex3f(14.5f,-enviroHeight,-14.5f);
    glEnd();
    

}

void keyboard(unsigned int key)
{
    //if sinusodial you will have constant acceleration camera movement, if linear you will have exponential decay movement
    if(key == 119)
    {
        shipVeloc += (.01 - (shipVeloc/.3)*.01); //want it to stop accelerating at .3   
    }
    
    if( key ==115 ) //s has been pressed
    {   shipVeloc -= (.01 + (shipVeloc/.3)*.01); //want it to stop accelerating at .3   
    }
    
    if( key == 106) //j has been registered this will be our booster
    {
        turboOn = true;
    }

    if(key == 32) //space has been pressed 
    {		
	// can only make a bullet every 20 frames
	if(bulletCD == 0)
	{	Bullet * bullet = new Bullet;
		//changing these will authomatically update evertything about the bullet
		float radius = .01;
		float vel = .5;	
	
		float xray = cos(shipRay[1]*PI/180)*sin(shipRay[0]*PI/180);
		float yray = sin(shipRay[1]*PI/180);
		float zray = cos(shipRay[1]*PI/180)*cos(shipRay[0]*PI/180);
		Vector3f position(shipPos[0]+xray*(1+radius),shipPos[1]+yray*(1+radius),shipPos[2]-zray*(1+radius));
		Vector3f velocity(xray*(vel+shipVeloc), yray*(vel+shipVeloc), -zray*(vel+shipVeloc));
		Bullet holder(radius, position, velocity, bulletLifeSpan);
		*bullet = holder;
		//Vector3f veloc = bullet->getVelocity();
		bullets.push_back(bullet);	
    		bulletCD = 8;
	}
    }

    if (key == 27) 
        {
        // ESC hit, so quit
        //clean up our object holders cause their on the heap
	for(unsigned int i=0; i<asteroids.size(); i++)
	{	Asteroid * _asteroid = asteroids[i];
		asteroids.erase(asteroids.begin() + i);
		delete _asteroid;
	}
	for(unsigned int j=0; j<bullets.size(); j++)
	{	Bullet * _bullet = bullets[j];
		bullets.erase(bullets.begin() + j);
		delete _bullet;
	}
	
        printf("demonstration finished.\n");
        exit(0);
        }                                  
}

//including these to make code more readable
bool beatLevel(){
	return asteroids.empty();
}

void nextLevel(){
	level ++;
	shipPos[0] = 0;
	shipPos[1] = 0;
	shipPos[2] = 0;
}
	

// *** ALL COLISION FUNCTIONS BELOW *** //

/* This was actually pretty simple after a lot of thinking, and im proud to say i didnt consult the internet for this one. 
 * To solve for accurate collision detectoin we will take each plane of our ship, their all triangles, and then take the cross 
 * product of two of their lines, this will give us a orthogonal vector. Next we take the vector from the ships position to the asteroids
 * position and dot product that to our orthogonal vector, this gives us the projection, or inother words how far it is from hitting
 * the plane that makes up a part of the ship. We then consider the smalled dot product because it is the closest to the ship. 
 * finally we put a tolerance on that plane of the ship, ie an average from center of ship to the middle of the plane, and its 3 edges.
 *
 * NOTE:I am brute forceing this, i could employ an oct-tree and an initiall outer sphere before ever considering when an asteroid
 * will be near to collision but with the few number of asteroids in this game so far that is not necessary
 */
bool shipHit(Asteroid * test){
	Vector3f position(shipPos[0], shipPos[1], shipPos[2]);
	
	float magnitude = test->getPosition().distance(position);
	Vector3f distance = test->getPosition()-position;	//subtraction yeilds us a vector point from the ship, follows convention of the orthonormals
	
	unsigned int bestdot=1000;	//huge so it will easily be replaced
	unsigned int bestside = 11;	//11 so it will error if the for loop fails
	//find the best side of to search for collision
	for(unsigned int i=0; i<orthonormals.size(); i++)
	{	if(bestdot < orthonormals[i].dotProduct(distance))
		{	bestdot = orthonormals[i].dotProduct(distance);
			bestside = i;
		}
	}	
	//now for the first 4 in our ortho normals i know the "bubble" or tolerance that i want to be before there is a collision, and the other 4 is the same thing
	if(bestside<5)	return (magnitude-test->getRadius())<=.156;	//the average clipping for colision on the first 4 triangles
	else if(bestside>4 && bestside<9) return (magnitude-test->getRadius())<=.2989;
	
	else{
	cout<<"error in the shipHit() function"<<endl;
	return true;
	}
}

//soft collision detection that just puts a buble around the ship and when a asteroid hits it it does the shipHit function bubble will have a radius of .5
bool softShipHit(Asteroid * test){
	Vector3f position(shipPos[0], shipPos[1], shipPos[2]);
	float magnitude = test->getPosition().distance(position);
	return magnitude < (.5+test->getRadius());
}

void shipAstCollision(){
	bool hit = false;
	for(unsigned int i=0; i<asteroids.size(); i++)
        	if(softShipHit(asteroids[i])) hit = shipHit(asteroids[i]);
        if(hit){
	cout<<"you have been hit"<<endl;
	exit(0);
	}

	//now check to see if it colides with the wall, for this we only have to consider the tips of the tails 
	Vector3f position(shipPos[0], 0.f, shipPos[2]);
	Vector3f origin(0.f,0.f,0.f);
	float magnitude = origin.distance(position);
	
	if( magnitude >= (enviroRad-.33) )	// the tails are .35ish awawy from the center of the ship
	{
		cout<<"you have run into the wall, learn to drive"<<endl;
		exit(0);
	}
}

//this function will be called on bullet-asteroid collision, it will delete the current asteroid and if the level > 0 will spawn 
//2 more asteroids in its place and place them in the same place in the asteroids vector
void split(Asteroid * target,unsigned int adress)
{
	if(target->getLevel()>0)
	{//recylce the make asteroid function, just now we specify their position and velocity 
		Vector3f velocity1, position1, velocity2, position2;
	        float radius = .7;
 	       for(int j=0; j<3; j++)  
        	{       velocity1.values[j] = randomFloat()*.03 + .001;
                	velocity2.values[j] = randomFloat()*.03 + .001;
        	}	
		position1 = target->getPosition() + Vector3f(target->getRadius(), 0, target->getRadius());
		position2 = target->getPosition() - Vector3f(target->getRadius(), target->getRadius(), target->getRadius());
		velocity2 = target->getVelocity() - velocity1;
        
		Asteroid * newastr1 = new Asteroid;
	        Asteroid a(radius, position1, velocity1, target->getLevel());
		Asteroid * newastr2 = new Asteroid;
	        Asteroid b(radius, position2, velocity2, target->getLevel());
	        *newastr1 = a;
		*newastr2 = b;
	        asteroids.push_back(newastr1);
	        asteroids.push_back(newastr2);
		//okay wev made two asteroids and pushed them to the shit, now delete the current asteroid
		asteroids.erase(asteroids.begin() + adress);
		delete target;
	}
	
	//else the level was of the asteroid was 1 now its 0 do nothing but delete it
	else{
	asteroids.erase(asteroids.begin() + adress);
	delete target;
	}		
}


//also handles deleting any asteroid that it colides with. 
void drawBullets()
{
 for(unsigned int i=0; i<bullets.size(); i++)
        {       if(bullets[i]->shouldDelete()) bullets.erase(bullets.begin() +i);
		else{
			bullets[i]->move();
			bullets[i]->wallCollide(enviroRad);
			bullets[i]->topbotCollide(enviroHeight);
			for(unsigned int j=0; j<asteroids.size();j++)
				if(bullets[i]->colliding(*asteroids[j])){
					Bullet * a = bullets[i];
					if( asteroids[j]->takeHit() )	split(asteroids[j], j);
					bullets.erase(bullets.begin() + i);
					delete a;
					break;
				}
                bullets[i]->draw();
		}
        }
}

void drawAsteroids()
{
	for(unsigned int i=0; i<asteroids.size(); i++)
	{	asteroids[i]->move();
		asteroids[i]->wallCollide(enviroRad);
		asteroids[i]->topbotCollide(enviroHeight);
		for(unsigned int j=0; j<asteroids.size(); j++)
			if(i!=j) asteroids[i]->collision(*asteroids[j]);
		asteroids[i]->draw();
	}
}


//NOTE i messed up and my zray is actually the - of what it should be, i didnt catch this fast enough so you will see -zray instead of just fixing zray
void display(void)
{
	//before everything check if the level is beat, if so restart at the next level
	if(beatLevel()) nextLevel();

       	//controls, chekcing first so everything is updated before i draw
	for(unsigned int i=0; i<256; i++)
		if(keyStates[i]) keyboard(i);
       	
	if(asteroids.empty()) makeAsteroids(level);
	      
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
	// reset OpenGL transformation matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity(); // reset transformation matrix to identity
        

        float zray = cos(shipRay[1]*PI/180)*cos(shipRay[0]*PI/180); //we are offset by 90 degrees because the ship starts on the z axis, ie z is the new x brah
        float yray = sin(shipRay[1]*PI/180);
        float xray = cos(shipRay[1]*PI/180)*sin(shipRay[0]*PI/180);
	float xrayRotate = sin(shipRay[0]*PI/180);
	float zrayRotate = cos(shipRay[0]*PI/180);
        // setup look at transformation so that we always have the eye tied to the ship 
        gluLookAt(  (-xray*1.4+shipPos[0]), (-yray*1.3+.4+shipPos[1]), (zray*1.4+shipPos[2]), (xray*2+shipPos[0]),
                    (yray*2+shipPos[1]), (-zray*2+shipPos[2]), 0, 1,0);
      
	/*//Lines drawn to show ray and perpendic line to ray of ship	
	glColor3f(1.f,1.f,1.f);
        glBegin(GL_LINES);
            glVertex3f(shipPos[0], shipPos[1], shipPos[2]);
            glVertex3f(xray*2+shipPos[0], yray*2+shipPos[1], -zray*2+shipPos[2]);
        glEnd();
	
	glColor3f(1.f,1.f,1.f);
        glBegin(GL_LINES);
            glVertex3f(-zray,0, -xray);
            glVertex3f(zray, 0, xray);
        glEnd();*/

        drawEnviroment();
        if(!bullets.empty()) drawBullets();
	if(!asteroids.empty()) drawAsteroids();
	shipAstCollision();
	
	glPushMatrix();
        glTranslatef(shipPos[0], shipPos[1], shipPos[2]);
        glRotatef(shipRay[1], zrayRotate ,0.f, xrayRotate );    //now rotate about the projection of the ray onto the xz plane
        glRotatef(-shipRay[0],0.f,1.f,0.f);  //rotate about the y axis first
        drawShip();
	glPopMatrix();


	glFlush();
        glutSwapBuffers();  // swap front/back framebuffer to avoid flickering 
}


void reshape (int w, int h)
{
        // reset viewport ( drawing screen ) size
        glViewport(0, 0, w, h);
        float fAspect = ((float)w)/h; 
        // reset OpenGL projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(70.f,fAspect,0.001f,30.f); 
}

//two functions for buffered key input
void keyup(unsigned char key, int x, int y){
	keyStates[key] = false;
}

void keydown(unsigned char key, int x, int y){
	keyStates[key] = true;
}

//These two functions will allow for fluid mouse movement controll, some of the commented out sections are peices i used for
//trouble shooting and im keeping incase i need to troubleshoot more
int oldx, oldy; 
int newx, newy;
void mouseUp(int x, int y){
	oldx = x;
	oldy = y;
}

void mouseMotion(int x, int y)
{ 

    //edge case: first frame
    newx = x;
    newy = y;

    float thetaXZ = sensitivity*(oldx-newx);       //if we move left, this will be positive
    float thetaY = sensitivity*(oldy-newy);        //if we move down, this will be negative
   
    oldx=newx;
    oldy=newy;

   // update the ships position:the eye will be tied to the ship
    shipRay[0] += thetaXZ;
    shipRay[1] += thetaY;
    
  /*  xsum +=thetaXZ;
    ysum +=thetaY;
    count++;
    if(count % 50 == 0)
    {   cout<<"summation of thetaXZ "<<xsum<<"\tsummation of thetaY "<<ysum<<endl;  
        cout<<"shipray[0] "<<shipRay[0]<<"\tshipRay[1] "<<shipRay[1]<<endl;
        xsum=0;
        ysum=0;
    }*/

    //tidy up
    if(shipRay[0] > 360) shipRay[0] -= 360;
    if(shipRay[0] < 0) shipRay[0] += 360;
    if(shipRay[1] > 90) shipRay[1] = 89; 
    if(shipRay[1] < -90) shipRay[1] = -89;
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        	glutMotionFunc(mouseMotion); 	
}


//will put cooldowns and other timed events that use counters in here
void timer(int v)
{	
	if(bulletCD >0) bulletCD--; 
        glutPostRedisplay(); // trigger display function by sending redraw into message queue
        glutTimerFunc(1000/nFPS,timer,v); // restart timer again
}

int main(int argc, char* argv[])
{
 	srand((unsigned int) time(0));	//seeding the RNG
	makeOrthos();	//loading our vector that will be used for collision detection	

        glutInit(&argc, (char**)argv);
        //set up for double-buffering & RGB color buffer & depth test
        glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
        glutInitWindowSize (1200, 900); 
        glutInitWindowPosition (100, 100);
        glutCreateWindow ((const char*)"hanselm2 MP2");
        
        init(); // setting up user data & OpenGL environment       

        // set up the call-back functions 
        glutDisplayFunc(display);  // called when drawing 
        glutReshapeFunc(reshape);  // called when change window size
        glutKeyboardFunc(keydown); // called when received keyboard interaction
        glutKeyboardUpFunc(keyup);
	glutMouseFunc(mouse);       // called when received mouse interaction
	glutPassiveMotionFunc(mouseUp);
	glutTimerFunc(100,timer,nFPS); // a periodic timer. Usually used for updating animation


        glutMainLoop(); // start the main message-callback loop
        
        return 0;
}
