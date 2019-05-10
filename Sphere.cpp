// Diego Batres
// C++ Raytracer
// Sphere.cpp

#include "Sphere.h"


// Constructor
Sphere::Sphere(double x, double y, double z, double radius) {
    cout << "Created sphere with radius " << radius << " at (" << x << ", " << y << ", " << z << ")." << endl;
    cout << "    Ns: " << Ns << endl;
    center << x, y, z;
    this->radius = radius;
}


// setKr(): set diffuse illumination coefficients
void Sphere::setKa(double Kar, double Kag, double Kab) {
    Ka << Kar, Kag, Kab;
    cout << "    Ka: " << Kar << "," << Kag << "," << Kab << endl;
}


// setKr(): set diffuse illumination coefficients
void Sphere::setKd(double Kdr, double Kdg, double Kdb) {
    Kd << Kdr, Kdg, Kdb;
    cout << "    Kd: " << Kdr << "," << Kdg << "," << Kdb << endl;
}


// setKs(): set specular illumination coefficients
void Sphere::setKs(double Ksr, double Ksg, double Ksb) {
    Ks << Ksr, Ksg, Ksb;
    cout << "    Ks: " << Ksr << "," << Ksg << "," << Ksb << endl;
}


// setKr(): set attenuation coefficients
void Sphere::setKr(double Krr, double Krg, double Krb) {
    Kr << Krr, Krg, Krb;
    cout << "    Kr: " << Krr << "," << Krg << "," << Krb << endl;
}


// setKo(): set refraction coefficients
void Sphere::setKo(double Kor, double Kog, double Kob, double e) {
    eta = e;
    Ko << Kor, Kog, Kob; 
    cout << "    Ko: " << Kor << "," << Kog << "," << Kob << endl;
}