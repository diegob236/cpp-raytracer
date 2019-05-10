// Diego Batres
// C++ Raytracer
// transform.h

#ifndef TRANSFORM_H_
#define TRANSFORM_H_
#define PI 3.14159265358979323846

#include <iostream>
#include "eigen/Eigen/Dense"

using namespace Eigen;
using namespace std;

void rotateVertices(Matrix3Xd &p, double wx, double wy, double wz, double theta);
void scaleVertices(Matrix3Xd &p, double scale);
void translateVertices(Matrix3Xd &p, double tx, double ty, double tz);

# endif /* TRANSFORM_H_ */