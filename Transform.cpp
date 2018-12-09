#include "Transform.h"
#include "Geometry.h"
#include "Window.h"
using namespace std;
const char* filepath;
int counter = 0;

Transform::Transform(glm::mat4 transMat) { // transformation mat
    //M = translation * M;
    M = transMat;
    //    origin = glm::vec3(M[3][0], M[3][1], M[3][2]); // origin of bounding sphere
    //    origin = glm::vec3(M[0][3], M[1][3], M[2][3]);
    origin = glm::vec3(M[0][0], M[1][1], M[2][2]);

   //    cout << "origin is initialized as: " << origin.x << ", " << origin.y << ", " << origin.z << endl;
    //radius = 55.0f; // radius of bounding sphere
    //    midY = (M[3][1] - M[0][1])/2;
}

Transform::~Transform() {
}

void Transform::setM(glm::mat4 newM) {
    M = newM;
}

void Transform::addChild(Node* node) {
    children.push_back(node);
}

void Transform::removeChild(Node *node) {
    children.remove(node);
}

void Transform::draw(GLuint shaderProgram, glm::mat4 C) {
    
    glm::mat4 M_new = C * M;

    list<Node*>::iterator it = children.begin();
    while (it != children.end() && *it != nullptr) {
        Node* child = *it;
        child->draw(shaderProgram, M_new);
        it++;
    }

    
}

void Transform::update() {
    for (list<Node*>::iterator it = children.begin(); it != children.end(); ++it) {
        Node* child = *it; //
        child->update();
    }
    if (counter < 150) {
        if (L_LEG | R_ARM) walk(0.04f);
        if (R_LEG | L_ARM) walk(-0.04f);
        
        if (L_ANTENNA) rotatey(1) ;
        if (R_ANTENNA) rotatey(-1);
        counter++;
    } else {
        if (L_LEG | R_ARM) walk(-0.04f);
        if (R_LEG | L_ARM) walk(0.04f);
        
        if (L_ANTENNA) rotatey(-1) ;
        if (R_ANTENNA) rotatey(1);
        counter++;
    }
    if (counter == 300) counter = 0;
}

void Transform::walk(float deg) {
    //    M = M * glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    //    M = M * glm::rotate(glm::mat4(1.0f), sin(deg), glm::vec3(1.0f, 0.0f, 0.0f));
    M = glm::rotate(glm::mat4(1.0f), sin(deg), glm::vec3(1.0f, 0.0f, 0.0f)) * M;
}
void Transform::rotate(float deg) {
        M = M * glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));

}
void Transform::rotatey(float deg) {
    M = M * glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));

}
void Transform::rotatez(float deg) {
    M = M * glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
    
}
void Transform::scalesize(float val) {
    M = M * glm::scale(glm::mat4(1.0f), glm::vec3(val));
}

void Transform::scale(float val) {
    //    M = M * glm::scale(glm::mat4(1.0f), glm::vec3(val));
    //    M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, val)) * M;
    
    for (list<Node*>::iterator it = children.begin(); it != children.end(); ++it) {
        Node* child = *it; //
        child->scale(val);
    }
    //    M = M * glm::scale(glm::mat4(1.0f), glm::vec3(val));
}

void Transform::drawSphere(GLuint shaderProgram, ROBObject * sphere) {
    for (list<Node*>::iterator it = children.begin(); it != children.end(); ++it) {
        Node* child = *it; //
        sphere->drawLines(shaderProgram, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 50.0f, 0.0f))
                          * child->M * glm::scale(glm::mat4(1.0f), glm::vec3(55.0f))); // strange results
    }
}

void Transform::translateX(float xVal) {
    glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(xVal, 0.0f, 0.0f)); //
    M = translateMat * M;
}

// maps sphere's origin from world coordinates to canonical view volume coordinates
//glm::vec3 Transform::origin_in_CVV(glm::vec3 x) {
//    glm::vec3 x_prime;
//
////    x_prime = x * glm::mat3(M * Window::V * Window::P); // xMVP or PVMx?
//
//    x_prime = glm::mat3(Window::P * Window::V * M) * x; // PVMx
//    return x_prime;
//}

// assume normal is pointing inwards say (-1, 0, 0)
// then if choose random point p that's on the plane, it should be (1, 0, 0)
//float Transform::dist_to_plane(glm::vec3 x_prime, glm::vec3 normal) {
float Transform::dist_to_plane(glm::vec4 origin_NDC, glm::vec3 normal) {
    
    float dist = 0.0f;
    //    glm::vec3 p = glm::vec3((-1)*normal.x, normal.y, normal.z);
    glm::vec4 p = glm::vec4((-1)*normal.x, normal.y, normal.z, 1.0f);
    
    //    dist = glm::length((x_prime - p) * normal));
    dist = glm::length((origin_NDC - p) * glm::vec4(normal, 1.0f));
    
    return dist;
}


int Transform::in_six_planes(glm::vec3 origin) {
    glm::vec4 origin_NDC = Window::P * Window::V * M * glm::vec4(origin, 1.0f);
    origin_NDC = origin_NDC/origin_NDC.w;
    
    int count = 0;
    glm::vec3 normal1 = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 normal2 = glm::vec3(-1.0f, 0.0f, 0.0f);
    glm::vec3 normal3 = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 normal4 = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::vec3 normal5 = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 normal6 = glm::vec3(0.0f, 0.0f, -1.0f);
    
    vector<glm::vec3> normals;
    normals.push_back(normal1);
    normals.push_back(normal2);
    normals.push_back(normal3);
    normals.push_back(normal4);
    normals.push_back(normal5);
    normals.push_back(normal6);
    
    for (glm::vec3 normal: normals) {

        if ( (origin_NDC.x >= -1.0f || origin_NDC.x <= 1.0f)){
      
            count++;
        }
        if ( (origin_NDC.y >= -1.0f || origin_NDC.y <= 1.0f)){
      
            count++;
        }
        if ( (origin_NDC.z >= -1.0f || origin_NDC.z <= 1.0f) ){
        
            count++;
        }
    }
    count = count/3;
    return count;
}

float Transform::radius_in_NDC(float radius, glm::vec3 normal) {
    // Get the origin of sphere in NDC using actual origin
    glm::vec4 origin_NDC = Window::P * Window::V * M * glm::vec4(origin, 1.0f);
    origin_NDC = origin_NDC/origin_NDC.w;
    
    glm::mat4 inverseCamRot = Window::V;
    inverseCamRot[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 camRot = glm::transpose(inverseCamRot);
    
    // Axis in camera space
    glm::vec4 axis = camRot * glm::vec4(normal, 0.0f);
    
    // Use the axis to find the other side of the radius in NDC space
    glm::vec4 theOtherSide_world = glm::vec4(origin, 1.0f) + axis * radius;
    glm::vec4 theOtherSide = Window::P * Window::V * M * theOtherSide_world;
    theOtherSide = theOtherSide/theOtherSide.w;
    
    // Calculate radius using the magnitude
    
    float radius_NDC = glm::length(theOtherSide - origin_NDC);
    //    cout << "radius in NDC: " << radius_NDC << endl;
    return radius_NDC;
}

bool Transform::check_origin_NDC(glm::vec3 origin, glm::vec3 normal) {
    bool val = false;
    glm::vec4 origin_NDC = Window::P * Window::V * M * glm::vec4(origin, 1.0f);
    origin_NDC = origin_NDC/origin_NDC.w;
    
    cout << "origin is: " << origin.x << ", " << origin.y << ", " << origin.x << endl;
    cout << "origin in NDC is: " << origin_NDC.x << ", " << origin_NDC.y << ", " << origin_NDC.z << endl;
    cout << "normal is: " << normal.x << ", " << normal.y << ", " << normal.z << endl;
    
    //    cout << origin_NDC.x << ", " << origin_NDC.y << ", " << origin_NDC.z << endl;
    
    if (normal.x != 0.0f) {
        if (origin_NDC.x <= -1.0f || origin_NDC.x >= 1.0f) {
            val = true;
            //            cout << "true" << endl;
        }
    } else if (normal.y != 0.0f) {
        if (origin_NDC.y <= -1.0f || origin_NDC.y >= 1.0f) {
            val = true;
            //            cout << "true" << endl;
        }
    } else if (normal.z != 0.0f) {
        if (origin_NDC.z <= -1.0f || origin_NDC.z >= 1.0f) {
            val = true;
            //            cout << "true" << endl;
        }
    }
    return val;
    //    cout << "false" << endl;
    //    return false;
}

