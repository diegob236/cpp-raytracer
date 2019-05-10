// Diego Batres
// C++ Raytracer
// Sphere.h

#ifndef SPHERE_H_
#define SPHERE_H_

#include <iostream>
#include "eigen/Eigen/Dense"

using namespace Eigen;
using namespace std;

class Sphere {   
    public:
        double radius, eta;
        double Ns = 16;
        Vector3d center, color;
        Vector3d Ka, Kd, Ks, Kr, Ko;

        Sphere(double x, double y, double z, double radius);
        void setKa(double Kar, double Kag, double Kab);
        void setKd(double Kdr, double Kdg, double Kdb);
        void setKs(double Ksr, double Ksg, double Ksb);
        void setKr(double Krr, double Krg, double Krb);
        void setKo(double Kor, double Kog, double Kob, double e);
};

# endif /* SPHERE_H_ */