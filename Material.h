// Diego Batres
// C++ Raytracer
// Material.h

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <iostream>
#include "eigen/Eigen/Dense"

using namespace Eigen;
using namespace std;

class Material {
    public:
        double Ns;
        Vector3d Ka, Kd, Ks;
        Material() {}
        Material(Vector3d a, Vector3d d, Vector3d s) {Ka = a; Kd = d; Ks = s;}
        void setKa(Vector3d a) {Ka = a;}
        void setKd(Vector3d d) {Kd = d;}
        void setKs(Vector3d s) {Ks = s;}
        void setNs(double ns) {Ns = ns;}
        void print() {
            cout << "    Ka: " << Ka[0] << "," << Ka[1] << "," << Ka[2] << endl;
            cout << "    Kd: " << Kd[0] << "," << Kd[1] << "," << Kd[2] << endl;
            cout << "    Ks: " << Ks[0] << "," << Ks[1] << "," << Ks[2] << endl;
            cout << "    Ns: " << Ns << endl;
        }
};

# endif /* MATERIAL_H_ */