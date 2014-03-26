CC = g++
CFLAGS = -Wall
PROG = 3DAst


SRCS = main1.cpp Vector3f.h Vector3f.cpp Sphere.h Sphere.cpp Bullet.h Bullet.cpp Asteroid.h Asteroid.cpp

ifeq ($(shell uname), Darwin) 
	LIBS = -framework OpenGL -framework GLUT 
else
	LIBS = -lglut -lGL -lGLU -IGLU
endif

all: $(PROG) 

$(PROG): $(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
