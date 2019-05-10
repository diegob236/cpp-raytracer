// Diego Batres
// C++ Raytracer
// transform.cpp

#include "transform.h"


// rotateVertices(): rotate vertices along the specified axis by theta
void rotateVertices(Matrix3Xd &p, double wx, double wy, double wz, double theta) {

    // Normalize axis of rotation
    Vector3d w(wx, wy, wz);
    w.normalize();

    // Construct rotation matrix
    Matrix3d r;
    r = AngleAxisd(theta*PI/180, w);

    // Rotate points
    cout << "  Rotating along " << ((wx != 0) ? "x" : "") << ((wy != 0) ? "y" : "") << ((wz != 0) ? "z" : "") << "-axis by " << theta << " degrees." << endl;
    p = r * p;
}


// scaleVertices(): scale vertices by the specified amount
void scaleVertices(Matrix3Xd &p, double s) {

    // Construct scale matrix
    Matrix3d scale;
    scale << s, 0, 0,
             0, s, 0,
             0, 0, s;

    // Scale points
    cout << "  Scaling by factor of " << s << "." << endl;
    p = scale * p;
}


// translateVertices(): translate vertices by the specified direction
void translateVertices(Matrix3Xd &p, double tx, double ty, double tz) {
    
    // Translate every point
    cout << "  Translating by [" << tx << " " << ty << " " << tz << "]." << endl;
    for (unsigned int i = 0; i < p.cols(); i++){
        p.col(i)[0] = p.col(i)[0] + tx;
        p.col(i)[1] = p.col(i)[1] + ty;
        p.col(i)[2] = p.col(i)[2] + tz;
    }

}