#include "Plane.h"
#include <stdio.h>

//#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
//#define norm(v)    sqrt(dot(v,v))  // norm = length of  vector
//#define d(P,Q)     norm(P-Q)        // distance = norm of difference
Plane::Plane(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
	set(v1, v2, v3);
}
Plane::Plane() {}

Plane::~Plane() {}
float Plane::distance(glm::vec3 p) {
	GLfloat sb, sn, sd;
	GLfloat distance;
	glm::vec3 temp;
	sn = -glm::dot(normal, (p - point));
	sd = glm::dot(normal, normal);
	sb = sn / sd;
	pOnPlane = p + sb * normal;
	temp = p - pOnPlane;
	distance = glm::dot((p - pOnPlane),normal);
	return distance;
}


void Plane::set(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
	glm::vec3 temp1, temp2;
	
	temp1 = v1 - v2;
	temp2 = v3 - v2;
	normal.x = temp1.y*temp2.z - temp1.z*temp2.y;
	normal.y = temp1.z*temp2.x - temp1.x*temp2.z;
	normal.z = temp1.x*temp2.y - temp1.y*temp2.x;
	normal = glm::normalize(normal);
	point = v2;
}