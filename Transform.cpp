#include "Transform.h";
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <list>
#include <GL/glew.h>
#include "Node.h"
#include "Plane.h"

#define ANG2RAD 3.14159265358979323846/180.0f
GLint count = 0;


Transform::Transform(glm::mat4 m) {
	M = m;
	tempM = M;
	origin = glm::vec3(M[0][0], M[1][1], M[2][2]);
}
Transform::~Transform()
{}

void Transform::draw(GLuint shaderProgram, glm::mat4 C){
	glm::mat4 MC = C * M;
	std::list<Node*>::iterator itr = children.begin();
	while(itr!=children.end()&&*itr!=nullptr) {
		Node* temp = *itr;
		temp->draw(shaderProgram, MC);
		itr++;
	}
}


void Transform::addChild(Node* node) {
	children.push_back(node);
}

void Transform::removeChild(Node *node) {
	children.remove(node);
}
void Transform::update() {
	GLint browCount = 0;
	std::list<Node*>::iterator itr;
	for (itr = children.begin(); itr != children.end(); ++itr) {
		Node* child = *itr; 
		child->update();
	}
/*	if (count < 300) {
		if (islArm) {
			walk(0.035f);

		}
		if (isrLeg) {
			walk(0.035f);
		}
		if (isrArm) {
			walk(-0.035f);
		}
		if (islLeg) {
			walk(-0.035f);
		}
		
	}
	else if (count >=300 ){
		if (count < 600) {
			if (islArm) {
				walk(-0.07f);
			}
			if (isrLeg) {
				walk(-0.07f);
			}
			if (isrArm) {
				walk(0.07f);
			}
			if (islLeg) {
				walk(0.07f);
			}
			if (islBrow) {
				browMove(0.01);
			}
			if (isrBrow) {
				browMove(-0.01);
			}
			
		}
		else {
			if (islArm) {
				walk(0.07f);
			}
			if (isrLeg) {
				walk(0.07f);
			}
			if (isrArm) {
				walk(-0.07f);
			}
			if (islLeg) {
				walk(-0.07f);
			}
			if (islBrow) {
			browMove(-0.01);
		}
		if (isrBrow) {
			browMove(0.01);
		}
			
		}
		
	}
	if (count == 900) {
		count = 300;
	}
	if (browCount == 600) {
		browCount = 0;
	}
	count++;
	browCount++;*/
}
void Transform::walk(float speed) {
	M = M*glm::rotate(glm::mat4(1.0f), sin(speed), glm::vec3(1.0f, 0.0f, 0.0f));
	
}
void Transform::browMove(float dis) {
	M = M * glm::translate(glm::mat4(1.0f), glm::vec3(dis, 0.0f, 0.0f));
}
void Transform::scale(float val) {
	M = M* glm::scale(glm::mat4(1.0f), glm::vec3(val));
	}

void Transform::rotate(glm::vec3 axis, float deg) {
	M= M * glm::rotate(glm::mat4(1.0f), deg, axis);
}

void Transform::scale(float x,float y,float z) {
	M = M * glm::scale(glm::mat4(1.0f), glm::vec3(x,y,z));
}

void Transform::scaleAll(float val) {
	std::list<Node*>::iterator itr = children.begin();
	while (itr != children.end() && *itr != nullptr) {
		Node* temp = *itr;
		temp->scale(val);
		itr++;
	}
}
//ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr
bool Transform::inSphere(glm::vec3 origin, GLfloat radius, Plane planes[]) {
	GLint inNumPlanes=0;
	glm::vec4 realOrigin = Window::P * Window::V * M * glm::vec4(origin, 1.0f);
	realOrigin = realOrigin / realOrigin.w;
	bool isIn = true;
	GLfloat distance;
	glm::vec3 dir, nc, fc, X, Y, Z;
	GLfloat tang = (GLfloat)tan(45.0f*ANG2RAD*0.5f);
	

	for (int i = 0; i < 6; i++) {
		distance = planes[i].distance(realOrigin);
		if (distance < -radius  ) {
			isIn = false;
		}
		else if (distance < radius) {
			isIn = true;
		}
	}
	
	
	/*for (int i = 0; i < 6; i++) {
		if (realOrigin.x >= radius || realOrigin.x <= radius
			&& !(realOrigin.y <= -radius || realOrigin.y >= radius)
			&& !(realOrigin.z <= -radius || realOrigin.z >= radius))
			inNumPlanes++;

		if (realOrigin.y >= radius || realOrigin.x <= radius
			&& !(realOrigin.x <= -radius || realOrigin.x >= radius)
			&& !(realOrigin.z <= -radius || realOrigin.z >= radius))
			inNumPlanes++;

		if (realOrigin.z >= radius || realOrigin.z <= radius
			&& !(realOrigin.y <= -radius || realOrigin.y >= radius)
			&& !(realOrigin.x <= -radius || realOrigin.x >= radius))
			inNumPlanes++;
	}*/
	
	return isIn;
	
	


}

float Transform::distanceToPlane(glm::vec3 plane, glm::vec3 point) {
	GLfloat dist = 0;
	glm::vec3 p = glm::vec3(-point.x, point.y, point.z);
	dist = glm::length((plane - p)*point);
	return dist;
}
glm::vec4 Transform::getOrigin(glm::mat4 M) {
	glm::vec4 realOrigin = Window::P * Window::V * M * glm::vec4(origin, 1.0f);
	realOrigin = realOrigin / realOrigin.w;
	return realOrigin;
}
glm::mat4 Transform::returnM(){
	return M;
}