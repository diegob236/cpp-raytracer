// Diego Batres
// C++ Raytracer
// Model.h

#ifndef MODEL_H_
#define MODEL_H_

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "eigen/Eigen/Dense"
#include "Material.h"

using namespace Eigen;
using namespace std;

class Model {
    public:
        bool smooth;
        string filename;
        Matrix3Xd vertices;
        Matrix3Xi faces;
        vector<Vector3d> v;
        vector<Vector3i> f;
        map<int, vector<int>> sharedFaces;

        double Ns;
        Vector3d Ka, Kd, Ks;
        vector<Material> materials;
        vector<int> materialIndices;
        vector<int> R, G, B;

        Model(string fname, bool smoothing);
        void readOBJFile();
        void readMTLFile();
        void populateVertices();
        void populateFaces();
};

# endif /* MODEL_H_ */
