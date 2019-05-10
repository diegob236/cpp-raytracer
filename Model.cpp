// Diego Batres
// C++ Raytracer
// Model.cpp

#include "Model.h"


// Constructor
Model::Model(string fname, bool smoothing) {
    filename = fname.substr(0, fname.find_last_of("."));
    smooth = smoothing;
}


// readOBJFile(): read data from OBJ file
void Model::readOBJFile() {

    // Open OBJ file
    string line;
    ifstream file;
    int materialCount = -1;
    file.open(filename + ".obj");
    if (!file) {cout << "  File not found: " << filename << ".obj" << endl; exit(-1);}
    else cout << "  Opened " << filename << ".obj." << endl;

    // Read lines in file
    while (getline(file, line)) {

        // Ignore comments
        while (line[0] == '#') getline(file, line);

        // Read line and check data type
        istringstream ss(line);
        string prefix;
        ss >> prefix;

        // Set proper material
        if (prefix == "usemtl") materialCount++;

        // Read vertices into vector v
        else if (prefix == "v"){
            double vx, vy, vz;
            ss >> vx >> vy >> vz;
            v.push_back(Vector3d(vx, vy, vz));
        }

        // Read faces into vector f
        else if (prefix == "f"){
            string fx, fy, fz;
            ss >> fx >> fy >> fz;
            fx = fx.substr(0, fx.find_first_of("/"));
            fy = fy.substr(0, fy.find_first_of("/"));
            fz = fz.substr(0, fz.find_first_of("/"));
            Vector3i face(stoi(fx), stoi(fy), stoi(fz));
            f.push_back(face);

            // Set material for face
            materialIndices.push_back(materialCount);

            // Add face to shared faces for each vertex
            sharedFaces[stoi(fx)].push_back(f.size()-1);
            sharedFaces[stoi(fy)].push_back(f.size()-1);
            sharedFaces[stoi(fz)].push_back(f.size()-1);
        }
    }

    // Populate matrices from vectors v,f
    populateVertices();
    populateFaces();

    // Close file stream
    file.close();
}


// readMTLFile: read data from MTL file
void Model::readMTLFile() {

    // Open MTL file
    ifstream file;
    file.open(filename + ".mtl");
    if (!file) {cout << "  File not found: " << filename << "." << endl; exit(-1);}
    else cout << "  Opened " << filename << ".mtl." << endl;

    // Iterate over lines in materials file
    string line;
    int materialCount = -1;
    cout << "  Reading materials file..." << endl;
    while (getline(file, line)) {

        // Ignore comments
        while (line[0] == '#') getline(file, line);

        // Read Ka, Kd, Ks, Ke, and Ns
        istringstream ss(line); string s;
        ss >> s;
        if (s == "newmtl"){
            Material m;
            materials.push_back(m);
            materialCount++;
        }
        else if (s == "Ka"){
            double Kax, Kay, Kaz;
            ss >> Kax >> Kay >> Kaz;
            Ka << Kax, Kay, Kaz;
            materials[materialCount].setKa(Ka);
        }
        else if (s == "Kd"){
            double Kdx, Kdy, Kdz;
            ss >> Kdx >> Kdy >> Kdz;
            Kd << Kdx, Kdy, Kdz;
            materials[materialCount].setKd(Kd);
        }
        else if (s == "Ks"){
            double Ksx, Ksy, Ksz;
            ss >> Ksx >> Ksy >> Ksz;
            Ks << Ksx, Ksy, Ksz;  
            materials[materialCount].setKs(Ks);
        }
        else if (s == "Ns"){
            ss >> Ns;
            materials[materialCount].setNs(Ns);
        }
    }
}


// populateVertices(): populate vertices into Matrix3Xd
void Model::populateVertices() {
    cout << "  Found " << v.size() << " vertices." << endl;
    vertices = Matrix3Xd(3, v.size());
    for (unsigned int i = 0; i < v.size(); i++){
        vertices.col(i) = v[i];
    }
}


// populateFaces(): populate faces into Matrix3Xd
void Model::populateFaces() {
    cout << "  Found " << f.size() << " faces." << endl;
    faces = Matrix3Xi(3, f.size());
    for (unsigned int i = 0; i < f.size(); i++){
        faces.col(i) = f[i];
    }
}
