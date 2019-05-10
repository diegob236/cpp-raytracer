// Diego Batres
// C++ Raytracer
// Light.cpp

#include "Light.h"


// Constructor
Light::Light(double lx, double ly, double lz, double lw, double lr, double lg, double lb) {
    if (lw == 0) {
        this->lx = 100000 * lx; this->ly = 100000 * ly; this->lz = 100000 * lz; this->lw = lw;
        position << 100000 * lx, 100000 * ly, 100000 * lz;
    }
    else {
        this->lx = lx; this->ly = ly; this->lz = lz; this->lw = lw;
        position << lx, ly, lz;
    }
    this->lr = lr; this->lg = lg; this->lb = lb;
    color << lr, lg, lb;
}


// print(): print light source
void Light::print() {
    cout << "[(" << lx << "," << ly << "," << lz << "," << lw << "), (" << lr << "," << lg << "," << lb << ")]" << endl;
}
