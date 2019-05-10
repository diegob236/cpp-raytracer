# C++ Raytracer
Author: Diego Batres

This program renders 3D objects as .ppm images by specifying a driver file with the camera position, 
image frame, resolution, recursion level (reflections), light sources, and objects (models and spheres). 
Supports object smoothing, shadows, reflections, refraction (spheres), multiple materials, and transformations.

## Example images:
![driver00](ppm/driver00.ppm)
![driver01](ppm/driver01.ppm)
![driver02](ppm/driver02.ppm)
![driver03](ppm/driver03.ppm)
![driver01](ppm/driver04.ppm)

## Instructions:

0. Prerequisites:
    - Clone this repository.
    - Install the [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) C++ library in the same folder.
    - Move mtl, obj, and driver files to main folder.

1. Compiling the program:

    - `make`: build executable
    - `make clean`: remove object files
    
    
2. Running the program:

    - Usage: `./raytracer [driver file] [output file]`
    - Example: `./raytracer driver00.txt driver00.ppm`
   
## Files:

1. Driver Files:

    - `driver00.txt`: 6 spheres (2048x2048)
    - `driver01.txt`: 4 spheres, board.obj (2048x2048)
    - `driver02.txt`: 23 spheres, 2 x cat.obj, 2 x checker.obj, 11 x board.obj (2048x2048)
	- `driver03.txt`: 8 spheres, cat.obj, wolf.obj, icosphere.obj (2048x2048)
    - `driver04.txt`: deer.obj, 3 x icosphere.obj (2048x2048)
    
2. Input Files:

    - `*.obj`: object files
    - `*.mtl`: material files

3. Source Files:

    - `Light.*`: light object
    - `Makefile`: makefile
    - `Model.*`: model object
    - `Material.*`: material object
    - `raytracer.*`: rendering, main method
    - `Sphere.*`: sphere object
	- `Ray.*`: ray object
    - `transform.*`: transform functions