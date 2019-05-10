# Diego Batres
# C++ Raytracer
# Makefile

CC = g++
CFLAGS = -Wall -O3 -c
LFLAGS = -Wall
OBJS = raytracer.o transform.o Light.o Model.o Sphere.o
INCLUDE = -I.

raytracer: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) $(INCLUDE) -o raytracer

raytracer.o: raytracer.cpp raytracer.h
	$(CC) raytracer.cpp $(INCLUDE) $(CFLAGS)

transform.o: transform.cpp transform.h
	$(CC) transform.cpp $(INCLUDE) $(CFLAGS)

Model.o: Model.cpp Model.h
	$(CC) Model.cpp $(INCLUDE) $(CFLAGS)
	
Light.o: Light.cpp Light.h
	$(CC) Light.cpp $(INCLUDE) $(CFLAGS)
	
Sphere.o: Sphere.cpp Sphere.h
	$(CC) Sphere.cpp $(INCLUDE) $(CFLAGS)

clean:
	-rm *.o raytracer
