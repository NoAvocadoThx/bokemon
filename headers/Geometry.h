//
//  Geometry.hpp
//  Project2
//
//  Created by Shengxiang Zhou on 10/31/18.
//  Copyright © 2018 Shengxiang Zhou. All rights reserved.
//

#ifndef Geometry_h
#define Geometry_h

#include <stdio.h>


#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>
#include "Node.h"
//#include "Window.h"
#include "ROBObject.h"
#include "Geometry.h"

#include <stdio.h>
#include <vector>

class Geometry : public Node {

public:
	//    GLint shaderProgram;
	ROBObject * obj;
	static int counter;
	glm::mat4 walker;

	glm::vec3 origin;
	glm::mat4 scaleMat;

	glm::mat4 M = glm::mat4(1.0f); // 4x4 transformation matrix

	bool L_ARM, R_ARM, L_LEG, R_LEG;
	bool GEODE = true;
	bool IS_SPHERE;
	bool IS_LIMB;
	bool DRAW_ALL;

	Geometry(const char *filepath);
	~Geometry();

	//    void setShader(GLuint shaderProgram);

	// set the modelview matrix to the current C matrix

	// initialization method to load a 3D model (OBJ file)
	// whose filename is passed to it
	//    void init (const char *filepath);

	// class method which draws the 3D model associated with this node
	void draw(GLuint shaderProgram, glm::mat4 C);

	void update();

	void scale(float val);

	void drawSphere(GLuint shaderProgram, ROBObject * sphere);

	//    glm::vec3 origin_in_CVV(glm::vec3 x);
	//    float dist_to_plane(glm::vec3 x_prime, glm::vec3 normal);
	float dist_to_plane(glm::vec4 origin_NDC, glm::vec3 normal);
	int in_six_planes(glm::vec3 origin);
	float radius_in_NDC(float radius, glm::vec3 normal);
};

#endif /* Geometry_h */
