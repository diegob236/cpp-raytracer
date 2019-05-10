// Diego Batres
// C++ Raytracer
// Light.h

#ifndef LIGHT_H_
#define LIGHT_H_

#include <iostream>
#include "eigen/Eigen/Dense"

using namespace Eigen;
using namespace std;

class Light {
    private:
        double lx, ly, lz, lw;
        double lr, lg, lb;
    public:
        Vector3d position;
        Vector3d color;
        Light(double lx, double ly, double lz, double lw, double lr, double lg, double lb);
        void print();
};

# endif /* LIGHT_H_ */