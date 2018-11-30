#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Node.h"
#include "Window.h"
#include <vector>
#include <list>
#include "Plane.h"
class Transform : public Node {
public:
	glm::mat4 M;
	glm::mat4 tempM;
	std::list<Node*> children;
	Plane pl[6];
	glm::vec3 origin;
	GLfloat radius=5.0f;
	glm::vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
	GLfloat nh, nw, fh, fw;
	GLfloat nDis = 1.0f;
	GLfloat fDis = 100.0f;
	bool isSphere=false;
	bool islArm,isrArm,islLeg,isrLeg,islBrow, isrBrow;
	Transform(glm::mat4 m);
	~Transform();
	
	void draw(GLuint shaderProgram, glm::mat4 C);
	void translate(float x, float y, float z);
	void rotate(glm::vec3 axis, float deg);
	void rotateafter(glm::vec3 axis, float deg);
	void addChild(Node* child);
	void removeChild(Node* child);
	void update();
	void setMatrix(glm::mat4 matrix);
	void scale(float);
	void scale(float x, float y, float z);
	void walk(float);
	void browMove(float);
	void scaleAll(float);
	bool inSphere(glm::vec3 origin, GLfloat radius, Plane pl[]);
	float distanceToPlane(glm::vec3, glm::vec3);
	glm::mat4 returnM();
	glm::vec4 getOrigin(glm::mat4 M);
};
#endif