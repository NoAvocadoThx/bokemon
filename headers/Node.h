//
//  Node.h
//  Project2
//
//  Created by Shengxiang Zhou on 10/31/18.
//  Copyright © 2018 Shengxiang Zhou. All rights reserved.
//

#ifndef Node_h
#define Node_h
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif
#include <iostream>
#include <math.h>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ROBObject.h"
class Node {
public:
    glm::mat4 M; // 4x4 transformation matrix
    bool GEODE = false;
    glm::vec3 origin;

    virtual void draw(GLuint shaderProgram, glm::mat4 C) = 0;  // Matrix4
    // separates bounding sphere updates from rendering
    virtual void update() = 0;
    //    virtual void spin(float deg) = 0;//
    virtual void scale(float val) = 0;
    virtual void drawSphere(GLuint shaderProgram, ROBObject * sphere) = 0;
    
    
    //    glm::vec3 origin_in_CVV(glm::vec3 x);
    //    float dist_to_plane(glm::vec3 x_prime, glm::vec3 normal);
    virtual float dist_to_plane(glm::vec4 origin_NDC, glm::vec3 normal) = 0;
    virtual int in_six_planes(glm::vec3 origin) = 0;
    virtual float radius_in_NDC(float radius, glm::vec3 normal) = 0;
    
};

#endif
