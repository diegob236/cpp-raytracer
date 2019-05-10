// Diego Batres
// C++ Raytracer
// Ray.h

#ifndef RAY_H_
#define RAY_H_

#include <iostream>
#include "eigen/Eigen/Dense"

using namespace Eigen;
using namespace std;

class Ray {
    public:
        Vector3d P;     // Point
        Vector3d D;     // Direction
        bool isNull = false;
        Ray() {isNull = true;}
        Ray(Vector3d point, Vector3d direction) {P = point; D = direction;}
        Ray(const Ray &ray) {P = ray.P; D = ray.D;}
};

# endif /* RAY_H_ */