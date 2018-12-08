//
//  Geometry.cpp
//  Project2
//
//  Created by Shengxiang Zhou on 10/31/18.
//  Copyright © 2018 Shengxiang Zhou. All rights reserved.
//

#include "Geometry.h"
#include "OBJObject.h"
#include <fstream>
#include <iostream>
#include "Window.h"
glm::mat4 walker = glm::rotate(glm::mat4(1.0f), -90.0f / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
int Geometry::counter = 0;

Geometry::Geometry(const char *filepath) {
	DRAW_ALL = true;
	obj = new ROBObject(filepath);
	//
	if (strcmp(filepath, "./eyeball_s.obj") == 0) {
		IS_SPHERE = true;
		DRAW_ALL = false;
	}
	else {
		IS_SPHERE = false;;
	}
	if (strcmp(filepath, "./robot-parts-2018/limb_s.obj") == 0) {
		IS_LIMB = true;
	}
	else {
		IS_LIMB = false;
	}
	scaleMat = glm::mat4(1.0f);
}

Geometry::~Geometry() {}

void Geometry::draw(GLuint shaderProgram, glm::mat4 C) {
	C = C * scaleMat;
	std::cout << IS_SPHERE << std::endl;
	if (IS_SPHERE && Window::SPHERE_SHOW) {
		//float midY = (obj->toWorld[3][1] - obj->toWorld[0][1]);
		obj->drawLines(shaderProgram, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))
			* C * glm::scale(glm::mat4(1.0f), glm::vec3(3.0f)));
	}
	else if (IS_LIMB) {
		C = C * glm::rotate(glm::mat4(1.0f), -90.0f / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));

		obj->draw(shaderProgram, C);
	}
	else if (DRAW_ALL) {
		obj->draw(shaderProgram, C * glm::rotate(glm::mat4(1.0f), -90.0f / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)));
	}
}

void Geometry::update() {}

void Geometry::scale(float val) {
	scaleMat = scaleMat * glm::scale(glm::mat4(1.0f), glm::vec3(val));
}

void Geometry::drawSphere(GLuint shaderProgram, ROBObject * sphere) {

}

float Geometry::dist_to_plane(glm::vec4 origin_NDC, glm::vec3 normal) {
	return 0.0f;
}
int Geometry::in_six_planes(glm::vec3 origin) {
	return 0;
}
float Geometry::radius_in_NDC(float radius, glm::vec3 normal) {
	return 0.0f;
}
