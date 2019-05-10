// Diego Batres
// C++ Raytracer
// raytracer.cpp

#include "raytracer.h"
#include "transform.h"


// Variables
int recursionLevel;
double width, height;
double d, near;
double lft, bottom, rght, top;
Vector3d EV, LV, UP, WV, UV, VV;
Vector3d ambient;

vector<int> R, G, B;
vector<Light> lights;
vector<Model> models;
vector<Sphere> spheres;


// raycast(): cast light rays onto objects
void raycast(Ray &ray, Vector3d &color, Vector3d &att, int recursionLevel) {

    Vector3d vn, pt, center;            // vn = normal, pt = point of intersection, center = center of closest sphere
    Vector3d Ka, Kd, Ks;                // Ka, Kd, Ks = illumination coefficients
    Vector3d Kr(1.0, 1.0, 1.0);         // Kr = attenuation coefficient
    Vector3d Ko(1.0, 1.0, 1.0);         // Ko = refraction coefficient
    int mi;                             // mi = material index
    double Ns, t, eta;                  // Ns = Phong constant, t = t value, eta = refraction index
    double beta, gamma;                 // beta, gamma = beta/gamma of intersected triangle
    double t_min = INT_MAX;             // t_min = distance to closest point of intersection
    bool foundIntersection = false;     // foundIntersection = true if light hit an object

    // Iterate over every face in every model to test for ray-triangle intersection
    for (unsigned int i = 0; i < models.size(); i++) {
        for(unsigned int j = 0; j < models[i].f.size(); j++) {
            
            // Get triangle points and test for intersection
            Vector3d Av, Bv, Cv;
            Vector3i face = models[i].f[j];
            Av << models[i].vertices.col(face[0]-1);
            Bv << models[i].vertices.col(face[1]-1);
            Cv << models[i].vertices.col(face[2]-1);
            tie(t, beta, gamma) = testTriangleIntersection(ray, Av, Bv, Cv);

            // Update values if t < t_min
            if (t < t_min && t > 0.000001) {
                foundIntersection = true;
                t_min = t;
                pt = ray.P + t * ray.D;
                mi = models[i].materialIndices[j];
                Ka = models[i].materials[mi].Ka; Kd = models[i].materials[mi].Kd;
                Ks = models[i].materials[mi].Ks; Ns = models[i].materials[mi].Ns;

                // Get normal for smooth or sharp rendering
                vn = (Av - Bv).cross(Av - Cv); vn = vn / vn.norm();
                if (models[i].smooth) vn = interpolateNormal(models[i], face, vn, beta, gamma);
            }
        }
    }

    // Iterate over every sphere to test for for ray-sphere intersection
    for (unsigned int i = 0; i < spheres.size(); i++) {
        t = testSphereIntersection(ray, spheres[i]);

        // Update values f t < t_min
        if (t < t_min && t > 0.000001) {
            foundIntersection = true;
            t_min = t;
            pt = ray.P + t * ray.D;
            vn = pt - spheres[i].center; vn = vn / vn.norm();
            Ka = spheres[i].Ka; Kd = spheres[i].Kd; Ks = spheres[i].Ks;
            Kr = spheres[i].Kr; Ko = spheres[i].Ko; Ns = spheres[i].Ns;
            eta = spheres[i].eta; center = spheres[i].center;
        }
    }

    // If an intersection was found, calculate illumination for closest point of intersection and add to color
    if (foundIntersection) color += att.cwiseProduct(calculateIllumination(Ka, Kd, Ks, Ns, vn, pt, ray)).cwiseProduct(Ko);

    // Perform recursive ray tracing if needed
    if (foundIntersection && recursionLevel > 0) {

        // Reflection
        Vector3d Uinv, refR;
        Vector3d flec(0.0, 0.0, 0.0);
        Uinv = -1 * ray.D;
        refR = (2 * vn.dot(Uinv) * vn) - Uinv; refR = refR / refR.norm();
        Ray reflection(pt, refR);
        att = Kr.cwiseProduct(att);
        raycast(reflection, color, att, recursionLevel-1);
        color += att.cwiseProduct(Ko).cwiseProduct(flec);

        // Refraction
        if (Ko.sum() < 3.0) {
            Vector3d thru(0.0, 0.0, 0.0);
            Ray fraR = refractExit(-1 * ray.D, pt, center, eta);
            if (!fraR.isNull) raycast(fraR, thru, att, recursionLevel-1);
            color += att.cwiseProduct(Vector3d(1.0, 1.0, 1.0) - Ko).cwiseProduct(thru);
        }
    }

}


// refractRay(): calculate refraction
Vector3d refractRay(Vector3d W, Vector3d pt, Vector3d N, double eta1, double eta2) {
    double etar, a, b, wn, radsq;
    etar = eta1 / eta2;
    a = -etar;
    wn = W.dot(N);
    radsq = pow(etar, 2) * (pow(wn, 2) - 1) + 1;
    if (radsq < 0) {
        return Vector3d(0,0,0);
    }
    else {
        b = (etar * wn) - sqrt(radsq);
        return a*W + b*N;
    }
}


// refractExit(): calculate refraction
Ray refractExit(Vector3d W, Vector3d pt, Vector3d center, double eta) {
    Vector3d T1, T2, N, exitr, nin;
    N = pt - center; N = N / N.norm();
    T1 = refractRay(W, pt, N, 1.0, eta);
    if (T1.sum() == 0.0) return Ray();
    else {
        exitr = pt + 2*(center - pt).dot(T1) * T1;
        nin = center - exitr; nin = nin / nin.norm();
        T2 = refractRay(-T1, exitr, nin, eta, 1.0);
        Ray refR(exitr, T2);
        return refR;
    }
}


// interpolateNormal(): get interpolated normal for smoothing
Vector3d interpolateNormal(Model &m, Vector3i &face, Vector3d &vn, double beta, double gamma) {

    Vector3d sum[3] = {
        Vector3d(0.0, 0.0, 0.0), 
        Vector3d(0.0, 0.0, 0.0), 
        Vector3d(0.0, 0.0, 0.0)
    };                                      // sum = sum of normals of shared faces for each of the 3 vertices
    double count[3] = {0, 0, 0};            // count = number of shared faces for each of the 3 vertices
    Vector3d Na, Nb, Nc, normal;            // Na, Nb, Nc = normals of vertices a, b, and c

    // Iterate over shared faces for each vertex of the face
    for (int k = 0; k < 3; k++) {
        for (unsigned int l = 0; l < m.sharedFaces[face[k]].size(); l++) {
            Vector3i sharedFace = m.f[m.sharedFaces[face[k]][l]];
            Vector3d Asf, Bsf, Csf, vnsf;
            Asf << m.vertices.col(sharedFace[0]-1);
            Bsf << m.vertices.col(sharedFace[1]-1);
            Csf << m.vertices.col(sharedFace[2]-1);
            vnsf = (Asf - Bsf).cross(Asf - Csf); vnsf = vnsf / vnsf.norm();

            // Add normal of shared face to sum if vnsf.dot(vn) < cos(22.5)
            if (vnsf.dot(vn) > cos(22.5 * PI / 180.0)) {
                sum[k] += vnsf;
                count[k] += 1;
            }
        }
    }

    // Get average vertex normals and return interpolated normal
    Na = sum[0] / count[0]; Na = Na / Na.norm();
    Nb = sum[1] / count[1]; Nb = Nb / Nb.norm();
    Nc = sum[2] / count[2]; Nc = Nc / Nc.norm();
    normal = (1 - beta - gamma) * Na + beta * Nb + gamma * Nc; vn = vn / vn.norm();
    return normal;
}


// calculateIllumination(): calculate illumination from every light source
Vector3d calculateIllumination(Vector3d &Ka, Vector3d &Kd, Vector3d &Ks, double Ns, Vector3d &vn, Vector3d &pt, Ray &ray) {

    // Calculate ambient illumination
    Vector3d I = Ka.cwiseProduct(ambient);

    // Calculate diffuse illumination
    for (unsigned int i = 0; i < lights.size(); i++) {
        Vector3d L = lights[i].position - pt; L = L / L.norm();
        Ray ptL(pt, L);
        if (!testShadow(ptL) && vn.dot(L) > 0) {
            I += Kd.cwiseProduct(lights[i].color * vn.dot(L));

            // Calculate specular illumination
            Vector3d Cv = ray.P - pt; Cv = Cv / Cv.norm();
            Vector3d Rv = (2 * vn * vn.dot(L)) - L;
            if (Cv.dot(Rv) > 0) {
                I += Ks.cwiseProduct(lights[i].color * pow(Cv.dot(Rv), Ns));
            }
        }
    }

    // Return total illumination
    return I;
}


// testTriangleIntersection(): test for triangle intersection
tuple<double, double, double> testTriangleIntersection(Ray &ray, Vector3d &Av, Vector3d &Bv, Vector3d &Cv) {

    Matrix3d MM, MMs1, MMs2, MMs3;
    double beta, gamma, t;
    double detM, detM1, detM2, detM3;
    
    // Get Cramer's matrices
    Vector3d Yv = Av - ray.P;
    MM << (Av - Bv), (Av - Cv), ray.D;
    MMs1 = MM; MMs2 = MM; MMs3 = MM;
    for (int l = 0; l < 3; l++) {
        MMs1(l,0) = Yv[l];
        MMs2(l,1) = Yv[l];
        MMs3(l,2) = Yv[l];
    }

    // Calculate determinants
    detM = MM.determinant();
    detM1 = MMs1.determinant();
    detM2 = MMs2.determinant();
    detM3 = MMs3.determinant();

    // Get beta, gamma, t
    beta = detM1 / detM;
    gamma = detM2 / detM;
    t = detM3 / detM;

    // Check for intersection and calculate normal
    if ((beta >= 0) && (gamma >= 0) && ((beta + gamma) <= 1) && (t > 0)) {
        return make_tuple(t, beta, gamma);
    }
    else return make_tuple(-1.0, 0.0, 0.0);
}


// testSphereIntersection(): test for sphere intersection
double testSphereIntersection(Ray &ray, Sphere &sphere) {
    Vector3d Tv = sphere.center - ray.P;
    double v, csq, disc;
    v = Tv.dot(ray.D);
    csq = Tv.dot(Tv);
    disc = pow(sphere.radius, 2) - (csq - pow(v, 2));
    if (disc < 0) return -1;
    else return (v - sqrt(disc));
}


// testShadow(): check if a light ray intersects another model or sphere
bool testShadow(Ray &ray) {

    // Iterate over every face in every model to test for ray-triangle intersection
    for (unsigned int i = 0; i < models.size(); i++) {
        for(unsigned int j = 0; j < models[i].f.size(); j++) {
            Vector3d Av, Bv, Cv;
            Vector3i face = models[i].f[j];
            Av << models[i].vertices.col(face[0]-1);
            Bv << models[i].vertices.col(face[1]-1);
            Cv << models[i].vertices.col(face[2]-1);
            if (get<0>(testTriangleIntersection(ray, Av, Bv, Cv)) > 0.000001) return true;
        }
    }

    // Iterate over every sphere to test for for ray-sphere intersection
    for (unsigned int i = 0; i < spheres.size(); i++) {
        if (testSphereIntersection(ray, spheres[i]) > 0.000001) return true;
    }

    // Return false if no intersection was found
    return false;
}


// pixelPt(): map i,j pixel position to real world coordinates
Vector3d pixelPt(int i, int j) {
    double px = double(i) / double(width - 1) * (rght - lft) + lft;
    double py = double(j) / double(height - 1) * (bottom - top) + top;
    return EV + (near * WV) + (px * UV) + (py * VV);
}


// renderImage(): render image and write to output file
void renderImage(string &outputfile) {

    // Color every pixel
    double progress = 0.0;
    cout << endl << "Rendering " << outputfile << "..." << endl;
    for (int j = 0; j < width; j++){
        for (int i = 0; i < height; i++){

            // Display progress bar
            cout << "[";
            for (int i = 0; i < 70; i++) {
                if (i <= 70*progress) cout << "█";
                else cout << " ";
            }
            cout << "] " << int(progress * 100.0) << " %\r";
            cout.flush();
            progress = (((j+1)*width+(i+1))/(width*height));
            if (j == width-1 && i == height-1) cout << endl;

            // Create ray and raycast
            Vector3d Lv, Uv, color, att;
            Lv = pixelPt(i, j);
            Uv = Lv - EV; Uv = Uv / Uv.norm();
            Ray ray(Lv, Uv);
            color = Vector3d(0.0, 0.0, 0.0); att = Vector3d(1.0, 1.0, 1.0);
            raycast(ray, color, att, recursionLevel);
            R.push_back(min(255, int(color[0] * 255)));
            G.push_back(min(255, int(color[1] * 255)));
            B.push_back(min(255, int(color[2] * 255)));
        }
    }

    // Create output file
    ofstream outfile;
    outfile.open(outputfile);
    outfile << "P3" << endl << height << " " << width << " 255" << endl;

    // Read lines from R,G,B vectors
    for (unsigned int i = 0; i < R.size(); i++)
        outfile << R[i] << " " << G[i] << " " << B[i] << endl;
    
    // Close output file
    outfile << endl;
    outfile.close();
    cout << "  Wrote to output file " << outputfile << "." << endl << endl;
}


// main(): read driver file
int main(int argc, char* argv[]) {

    // Check for correct number of arguments
    if (argc != 3) {
        cout << "Usage: $./raytracer [driver file] [output file]" << endl;
        return -1;
    }
    string driverfile = argv[1];
    string outputfile = argv[2];

    // Check for correct output format
    if (outputfile.length() < 4) {
        cout << "Usage: output file must be of type .ppm [" << outputfile << "]" << endl;
        return -1;
    }
    if (outputfile.substr(outputfile.length() - 4) != ".ppm"){
        cout << "Usage: output file must be of type .ppm [" << outputfile << "]" << endl;
        return -1;
    }

    // Open driver file
    ifstream f;
    f.open(driverfile);
    if (!f) {
        cout << "File not found: " << driverfile << "." << endl;
        return -1;
    }
    else cout << "Opened " << driverfile << "." << endl;
    driverfile = driverfile.substr(0, driverfile.find_last_of("."));
    
    // Iterate over lines in driver file
    string line;
    cout << endl << "Reading driver specifications..." << endl;
    while (getline(f, line)) {

        // Ignore comments
        while (line[0] == '#') getline(f, line);

        // Read driver specifications
        istringstream ss(line); string s;
        ss >> s;
        if (s == "eye"){
            double ex, ey, ez;
            ss >> ex >> ey >> ez;
            EV << ex, ey, ez;
            cout << "  eye: " << ex << "," << ey << "," << ez << endl;
        }
        else if (s == "look"){
            double lx, ly, lz;
            ss >> lx >> ly >> lz;
            LV << lx, ly, lz;
            cout << "  look: " << lx << "," << ly << "," << lz << endl;
        }
        else if (s == "up"){
            double ux, uy, uz;
            ss >> ux >> uy >> uz;
            UP << ux, uy, uz;
            cout << "  up: " << ux << "," << uy << "," << uz << endl;
            
            // Initialize WV, UV, VV
            WV = EV - LV; WV = WV / WV.norm();
            UV = UP.cross(WV); UV = UV / UV.norm();
            VV = WV.cross(UV);
        }
        else if (s == "d"){
            ss >> d;
            near = d * -1;
            cout << "  d: " << d << endl;
        }
        else if (s == "bounds"){
            ss >> lft >> bottom >> rght >> top;
            cout << "  bounds: " << lft << "," << bottom << "," << rght << "," << top << endl;
        }
        else if (s == "res"){
            ss >> width >> height;
            cout << "  res: " << width << "," << height << endl;
        }
        else if (s == "recursionLevel"){
            ss >> recursionLevel;
            cout << "  recursionLevel: " <<  recursionLevel<< endl;
        }
        else if (s == "ambient"){
            double ax, ay, az;
            ss >> ax >> ay >> az;
            ambient << ax, ay, az;
            cout << "  ambient: " << ax << "," << ay << "," << az << endl;
        } 
        else if (s == "light"){
            double lx, ly, lz, lw;
            double lr, lg, lb;
            ss >> lx >> ly >> lz >> lw >> lr >> lg >> lb;
            Light l(lx, ly, lz, lw, lr, lg, lb);
            lights.push_back(l);
            cout << "  Added light source: "; l.print();
        }
        else if (s == "model"){

            // Read transformation instructions
            string smooth, filename;
            double wx, wy, wz, theta, scale, tx, ty, tz;
            ss >> wx >> wy >> wz >> theta >> scale >> tx >> ty >> tz >> smooth >> filename;

            // Get model data from OBJ file and set smoothing
            cout << endl << "Processing " << filename << "..." << endl;
            Model m(filename, (smooth == "smooth"));
            m.readOBJFile();
            m.readMTLFile();

            // Transform object
            rotateVertices(m.vertices, wx, wy, wz, theta);
            scaleVertices(m.vertices, scale);
            translateVertices(m.vertices, tx, ty, tz);

            // Add to model list
            models.push_back(m);
        }
        else if (s == "sphere"){

            // Read sphere variables and render sphere
            double x, y, z, radius;
            double kar, kag, kab, kdr, kdg, kdb, ksr, ksg, ksb, krr, krg, krb, kor, kog, kob, eta;
            ss >> x >> y >> z >> radius;
            ss >> kar >> kag >> kab >> kdr >> kdg >> kdb >> ksr >> ksg >> ksb >> krr >> krg >> krb >> kor >> kog >> kob >> eta;

            // Create sphere object
            Sphere sph(x, y, z, radius);
            sph.setKa(kar, kag, kab); sph.setKd(kdr, kdg, kdb); sph.setKs(ksr, ksg, ksb); sph.setKr(krr, krg, krb); sph.setKo(kor, kog, kob, eta);

            // Add to sphere list
            spheres.push_back(sph);
        }
    }

    // Render image
    renderImage(outputfile);
}
