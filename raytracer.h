// Diego Batres
// C++ Raytracer
// raytracer.h

#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#define _USE_MATH_DEFINES
#define PI 3.14159265358979323846

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <math.h>

#include "eigen/Eigen/Dense"
#include "Light.h"
#include "Material.h"
#include "Model.h"
#include "Ray.h"
#include "Sphere.h"

using namespace Eigen;
using namespace std;

void raycast(Ray &ray, Vector3d &color, Vector3d &att, int recursionLevel);
Vector3d refractRay(Vector3d W, Vector3d pt, Vector3d N, double eta1, double eta2);
Ray refractExit(Vector3d W, Vector3d pt, Vector3d center, double eta);
Vector3d interpolateNormal(Model &m, Vector3i &face, Vector3d &vn, double beta, double gamma);
tuple<double, double, double> testTriangleIntersection(Ray &ray, Vector3d &Av, Vector3d &Bv, Vector3d &Cv);
double testSphereIntersection(Ray &ray, Sphere &sphere);
bool testShadow(Ray &ray);
Vector3d calculateIllumination(Vector3d &Ka, Vector3d &Kd, Vector3d &Ks, double Ns, Vector3d &vn, Vector3d &pt, Ray &ray);
void renderImage(string &outputfile);
Vector3d pixelPt(int i, int j);

# endif /* RAYTRACER_H_ */