//
//  Transform.hpp
//  Project2
//
//  Created by Shengxiang Zhou on 10/31/18.
//  Copyright © 2018 Shengxiang Zhou. All rights reserved.
//

#ifndef Transform_h
#define Transform_h

#include "Node.h"
#include <vector>
#include <stdio.h>
#include <iostream>
#include <list>
#include "Node.h"
#include "Group.h"
#include "ROBObject.h"
class Transform : public Node {
public:

	glm::mat4 M; // 4x4 transformation matrix
	glm::vec3 origin; // vec3 or 4?

	float radius; // hard-coded as 55
	//    static float midY;
	bool SPHERE = false;
	bool LEFT = false;
	bool RIGHT = false;
	bool L_ARM = false;
	bool R_ARM = false;
	bool L_LEG = false;
	bool R_LEG = false;
	bool BODY = false;

	bool L_ANTENNA = false;
	bool R_ANTENNA = false;
	bool GEODE = false;
	int counter = 0;
	Transform(glm::mat4 transMat);
	~Transform();

	void setM(glm::mat4 newM);

	void init();

	std::list<Node*> children; // list of pointers to child nodes
	std::list<Node*>::iterator it;

	void addChild(Node* node);
	void removeChild(Node* node);

	// draw() needs to traverse the list of children and call
	// each child nodes from the list
	// when draw(C) is called, multiply matrix M with C
	void draw(GLuint shaderProgram, glm::mat4 C);

	void update();
	void walk(float deg);
	//    void spin(float deg);
	void scale(float val);
	void rotate(float deg);
	void rotatey(float deg);
	void rotatez(float deg);
	void scalesize(float val);
	void drawSphere(GLuint shaderProgram, ROBObject * sphere);
	bool inview(glm::vec3 planePos[6], glm::vec3 planeNorm[6]);
	void translateX(float xVal);

	void translateY(float yVal);
	void translateZ(float zVal);
	glm::vec3 origin_in_CVV(glm::vec3 x);
	//    float dist_to_plane(glm::vec3 x_prime, glm::vec3 normal);
	float dist_to_plane(glm::vec4 origin_NDC, glm::vec3 normal);
	int in_six_planes(glm::vec3 origin);
	float radius_in_NDC(float radius, glm::vec3 normal);

	bool check_origin_NDC(glm::vec3 origin, glm::vec3 normal);
};

#endif /* Transform_hpp */
