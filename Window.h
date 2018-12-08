#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"
#include "Plane.h"
#include "ROBObject.h"
#include "HeightGenerator.h"
#include "OBJObject.h"
#include "Geometry.h"
#include "Transform.h"
#include "Terrain.h"
#include <algorithm>
#include <iostream>
#include "Curve.h"
#include <cmath>
#include "camera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>                  // ...so now that's defined we can import GLM itself.
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#define ANG2RAD 3.14159265358979323846/180.0f

class Window
{
private:
	
public:
	static std::vector<GLfloat> distanceVec;
	static bool SPHERE_SHOW;
	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static bool toggleSphere;
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double x, double y);
	static glm::vec3 trackBallMapping(glm::vec2);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_pos_callback(GLFWwindow* window, double xPos, double yPos);
	static void CameraRotation(float rot_angle, glm::vec3 rotAxis);
	bool inViewSpace(glm::vec3, glm::vec3 lookAt, glm::vec3 up, glm::vec3 origin, GLfloat radius);
	static void calculateFrustum();
	static bool inSphere(glm::vec3 origin, GLfloat radius, glm::mat4 M);
	static void pushD(GLfloat,std::vector<GLfloat>);
	static void drawLine(GLuint shaderProgram);
	static GLuint select(int xx, int yy);
	static void move();
	static void calculate();
	static void translateCamera(glm::vec3 transVec);
	static void resetCamera();
};

#endif
