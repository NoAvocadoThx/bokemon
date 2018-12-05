#include "window.h"
#include "OBJObject.h"
#include "Geometry.h"
#include "Transform.h"
#include "Terrain.h"
#include <algorithm>
#include <iostream>
#include "Curve.h"
#include <cmath>





const char* window_title = "GLFW Starter Project";
Cube * cube;
GLint shaderProgram;
GLint skyboxShader;
GLint reflectShader;
GLint curveShader;
GLint sphereShader;
GLint terrainShader;
GLfloat fov = 45.0f;
GLint robotNum;
GLfloat distance;
bool isIn = false;
glm::vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
GLfloat nh, nw, fh, fw;
GLfloat nDis;
GLfloat fDis;
GLfloat randn;
Plane pl[6];
GLuint VBO, VAO, VAO2, VBO2;
OBJObject *RC;
OBJObject *currObj;
OBJObject *sphere;


ROBObject *ball;
ROBObject *body1,*body2,*body3;

//culling sphere
//Geometry *sphere;
//bezier curves
Curve *c0;
Curve *c1;
Curve *c2;
Curve *c3;
Curve *c4;
Curve *c5;
Curve *c6;
Curve *c7;



Transform * modelMtx, * modelballMtx, *modelbody1Mtx, *modelbody2Mtx, *modelbody3Mtx;
Transform * singleArmy;
Transform * army;
Transform * ballmtx;
Transform * body1mtx, *body2mtx, * body3mtx;
GLfloat size = 1.0f;
GLint OBJMode = 1;
GLint ptCount = 0;
GLint sptCount = 0;
GLdouble c_initX, c_initY,c_currX,c_currY;
glm::vec3 lastPoint,currPoint;
GLfloat rotationSpeed=1.5f;
GLint normalColor = 1;
GLfloat scroll=0;

glm::mat4 mat =glm::mat4(1.0f);
glm::vec3 top;

vector<Curve*> curves;
vector<glm::vec3> anchor;
vector<glm::vec3> ctrPts;
vector<glm::vec3> selectPts;
glm::vec3 currPt;
glm::vec3 selectColor= glm::vec3(0.1f, 0.9f, 0.1f);

Terrain *terrain;

bool left_release=true;
//toggle parameter
bool nToggle = false;
//lights switch
bool dirToggle = true;
bool ptToggle = false;
bool sptToggle = false;
//object rotation
bool objRotation=false;
bool mouseRot;
bool cameraRotation = true;
bool debugMode;
bool cull;
bool renderSphere;
bool pause=false;
bool selection=true;
bool rider;
bool reachTop;
bool normal;

glm::vec3 xPlane=glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 yPlane=glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 zPlane=glm::vec3(0.0f, 0.0f, 1.0f);


// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"
#define SKYBOX_VERT "../boxShader.vert"
#define SKYBOX_FRAG "../boxShader.frag"
#define REFLECT_VERT "../reflectShader.vert"
#define REFLECT_FRAG "../reflectShader.frag"
#define CURVE_VERT "../curveShader.vert"
#define CURVE_FRAG "../curveShader.frag"
#define SPHERE_VERT "../sphereShader.vert"
#define SPHERE_FRAG "../sphereShader.frag"
#define TERRAIN_VERT "../selectShader.vert"
#define TERRAIN_FRAG "../selectShader.frag"
#define BALL_PATH "../ball.obj"
#define BODY_PATH "../body.obj"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is



int Window::width;
int Window::height;
std::vector<GLfloat> Window::distanceVec;
glm::mat4 Window::P;
glm::mat4 Window::V;
bool Window::toggleSphere;



void Window::initialize_objects()
{
	
	
	
	//robotNum = 0;

	//culling sphere
	//sphere = new Geometry("../eyeball_s.obj");
	//sphere->isSphere = true;
	ball = new ROBObject(BALL_PATH);
	body1 = new ROBObject(BODY_PATH);
	/*
	ballmtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	ballmtx->addChild(ball);

	body1 = new Geometry(BODY_PATH);
	body1mtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	body1mtx->addChild(body1);

	body2 = new Geometry(BODY_PATH);
	body2mtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	body2mtx->addChild(body2);

	body3 = new Geometry(BODY_PATH);
	body3mtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	body3mtx->addChild(body3); 
*/
	cube = new Cube();

	terrain = new Terrain();
	terrain->translate(glm::vec3(-terrain->vertexCount / 2, -10, -terrain->vertexCount));
	
/*
	modelballMtx = new Transform(glm::mat4(1.0f));
	modelballMtx->addChild(ballmtx);*/
	/*
	modelbod1mtx;
	modelMtx->addChild(body1mtx);
	modelMtx->addChild(body2mtx);
	modelMtx->addChild(body3mtx);
	*/
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skyboxShader = LoadShaders(SKYBOX_VERT, SKYBOX_FRAG);
	reflectShader = LoadShaders(REFLECT_VERT, REFLECT_FRAG);
	curveShader = LoadShaders(CURVE_VERT, CURVE_FRAG);
    sphereShader = LoadShaders(SPHERE_VERT,SPHERE_FRAG);
	terrainShader = LoadShaders(TERRAIN_VERT, TERRAIN_FRAG);
   
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
	delete(currObj);
	delete(terrain);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(skyboxShader);
	glDeleteProgram(reflectShader);
    glDeleteProgram(sphereShader);
	glDeleteProgram(terrainShader);

}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		//P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		P = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
	//currObj->update();
	//modelMtx->update();
	
}

void Window::display_callback(GLFWwindow* window)
{
	glm::vec3 rOrigin;
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//calculateFrustum();
	// Use the shader of programID
	glUseProgram(skyboxShader);
	//ball->draw(skyboxShader);
	//cube->draw(skyboxShader);
	glUseProgram(terrainShader);
	terrain->draw(terrainShader);
	
	glUseProgram(shaderProgram);
	ball->draw(shaderProgram);
	//modelballMtx->draw(shaderProgram, glm::mat4(1.0f));
	//distanceVec.clear();
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	//dirSphere->translate(glm::vec3(10.0f, 10.0f, 5.0f));

	// Swap buffers
	glfwSwapBuffers(window);
	//dirSphere->translate(glm::vec3(10.0f, 10.0f, 5.0f));
	//dirSphere->scaleSize(4.0f);
	robotNum = 0;
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	

		
		
		//spot light rotation switch
		if (key == GLFW_KEY_2) {

			if (currObj->sptRot) {
				currObj->sptRot= false;

			}
			else {
				currObj->sptRot = true;

			}
		}


		if (key == GLFW_KEY_P) {

			if (nToggle) {
				pause = false;
				nToggle = !nToggle;
			}
			else {
				pause = true;
				nToggle = !nToggle;
			}

		}
		if (key == GLFW_KEY_H) {

			if (nToggle) {
				reachTop = false;
				nToggle = !nToggle;
			}
			else {
				reachTop = true;
				nToggle = !nToggle;
			}

		}
		if (key == GLFW_KEY_N) {

			if (nToggle) {
				normal = false;
				nToggle = !nToggle;
			}
			else {
				normal = true;
				nToggle = !nToggle;
			}

		}
		

	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{

		//Camera movement controls (FPS style)
		if (key == GLFW_KEY_W)	//Forward
		{
			translateCamera({ 0.0f,0.0f,1.0f });
			//cout << "Z" << mods << endl;
		}
		else if (key == GLFW_KEY_S) //Back
		{

			translateCamera({ 0.0f,0.0f,-1.0f });

		}
		//cout << "z" << endl;
		else if (key == GLFW_KEY_A) //Left
		{
			translateCamera({ 1.0f,0.0f,0.0f });
			//cout << "X" << mods << endl;
		}
		else if (key == GLFW_KEY_D) //Right
		{
			translateCamera({ -1.0f,0.0f,0.0f });
			//cout << "x" << endl;
		}
	}

}
void Window::translateCamera(glm::vec3 transVec) {
	V = glm::translate(glm::mat4(1.0f), transVec) * V;
}
void Window::scroll_callback(GLFWwindow* window, double x, double y) {
	

		    if (fov >= 1.0f && fov <= 100.0f)
				fov -= y;
			if (fov <= 1.0f)
				fov = 1.0f;
			if (fov >= 100.0f)
				fov = 100.0f;
			P = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 1000.0f);
		
		
	
}

glm::vec3 Window::trackBallMapping(glm::vec2 point)
{
	glm::vec3 v;    // Vector v is the synthesized 3D position of the mouse location on the trackball
	GLfloat d;     // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse
	v.x = (2.0*point.x - Window::width) / Window::width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
	v.y = (Window::height - 2.0*point.y) / Window::height;   // this does the equivalent to the above for the mouse Y position
	v.z = 0.0;   // initially the mouse z position is set to zero, but this will change below
	d = glm::length(v);    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
	d = (d < 1.0) ? d : 1.0;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
	v.z = sqrtf(1.001 - d * d);  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
	glm::normalize(v); // Still need to normalize, since we only capped d, not v.
	return v;  // return the mouse location on the surface of the trackball
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	//if left mouse button is pressed and hold
	if (button == GLFW_MOUSE_BUTTON_1) {
		if (action == GLFW_PRESS) {
			left_release = false;
			glfwGetCursorPos(window, &c_initX, &c_initY);
			lastPoint = trackBallMapping(glm::vec2(c_initX, c_initY));
			
		
			//cout << "selection xyz:" << selectPts[indexSelect].x << endl;

		}
		//if release
		else if (action == GLFW_RELEASE) {
			left_release = true;
		}
		
	}
	else if (button == GLFW_MOUSE_BUTTON_2) {
		if (action == GLFW_PRESS) {
			
			//cout << "selection xyz:" << selectPts[indexSelect].x << endl;
			
		}
	}
}
void Window::cursor_pos_callback(GLFWwindow* window, double xPos, double yPos) {
	//if left mouse button is not hold down
	if (!left_release) {
		glm::vec3 direction;
		glm::vec4 newPos;
		glm::vec4 newPos2;
		GLfloat angle;
		glm::vec3 rotationAxis;
		mouseRot = true;
		//clear currPoint
		currPoint = glm::vec3(0,0,0);
		//get surface location
		currPoint = trackBallMapping(glm::vec2(xPos, yPos));
		direction = currPoint - lastPoint;
		//get the roation axis perpendicular to currPoint X lastPoint 
		rotationAxis = glm::cross(currPoint, lastPoint);
		angle = direction.length()*rotationSpeed;
		float camAngle = glm::dot(lastPoint, currPoint) / (glm::length(lastPoint)*glm::length(currPoint));
		/*if (camAngle > 1) {
			camAngle= 1;
		}*/
		float degree = acos(camAngle);
		if (!cameraRotation) {
			

		}
		else {
			CameraRotation(degree, rotationAxis);
		}
		
	}
	else {
		//mouseRot = false;
	}
	lastPoint = currPoint;
}
void Window::CameraRotation(float angle, glm::vec3 rotAxis) {
	cam_pos = glm::rotate(glm::mat4(1.0f), (30.0f*angle * glm::pi<float>()) / 180.0f, rotAxis)*glm::vec4(cam_pos, 1.0);
	cam_up = glm::rotate(glm::mat4(1.0f), (30.0f*angle * glm::pi<float>()) / 180.0f, rotAxis)*glm::vec4(cam_up, 1.0);
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);

}
void Window::calculateFrustum() {
	nDis = 1.0f;
	fDis = 30.0f;
	bool isIn = true;
	GLfloat distance;
	glm::vec3 dir, nc, fc, X, Y, Z,d,right,up;
	GLfloat tang = (GLfloat)tan(45.0f*ANG2RAD*0.5f);
	nh = 2.0f * tan(fov / 2.0f)*nDis;
	nw = nh * (float)width / (float)height;
	fh = 2.0f*tan(fov / 2.0f)*fDis;
	fw = fh * (float)width / (float)height;

	Z = cam_pos - cam_look_at;
	Z = glm::normalize(Z);
	d = glm::normalize(cam_look_at-cam_pos);
	up = glm::normalize(cam_up);
	right = glm::cross(d, up);
	X = cam_up * Z;
	X = glm::normalize(X);
	Y = Z * X;
	//nc = cam_pos - Z * nDis;
	fc = cam_pos + d * fDis;
	nc = cam_pos + d * nDis;
	/*fc = cam_pos - Z * fDis;
	fc = cam_pos + cam_look_at * fDis;
	ntl = nc + Y * nh - X * nw;
	ntr = nc + Y * nh + X * nw;
	nbl = nc - Y * nh - X * nw;
	nbr = nc - Y * nh + X * nw;

	ftl = fc + Y * fh - X * fw;
	ftr = fc + Y * fh + X * fw;
	fbl = fc - Y * fh - X * fw;
	fbr = fc - Y * fh + X * fw;*/

	ftl = fc + (cam_up*(fh/2.0f)) - (right*(fw / 2.0f));
	ftr = fc + (up*(fh / 2.0f)) + (right*(fw / 2.0f));
	fbl = fc - (up*(fh / 2.0f)) - (right*(fw / 2.0f));
	fbr = fc - (up*(fh / 2.0f)) + (right*(fw / 2.0f));

	ntl = nc + (up*(nh / 2.0f)) - (right*(nw / 2.0f));
	ntr = nc + (up*(nh / 2.0f)) + (right*(nw / 2.0f));
	nbl = nc - (up*(nh / 2.0f)) - (right*(nw / 2.0f));
	nbr = nc - (up*(nh / 2.0f)) + (right*(nw / 2.0f));

	//top
	pl[0].set(ntr, ntl, ftl);
	//bottom
	pl[1].set(nbl, nbr, fbr);
	//left
	pl[2].set(ntl, nbl, fbl);
	//right
	pl[3].set(nbr, ntr, fbr);
	//nearP
	pl[4].set(ntl, ntr, nbr);
	//farP
	pl[5].set(ftr, ftl, fbl);
}
/*bool Window::inFrustum() {
	GLfloat distance;
	bool isIn=true;
	for (int i = 0; i < 6; i++) {
		distance = pl[i].distance(origin);
		/*if (distance < -radius) {
			return false;
		}
		else if (distance < radius){
			isIn = true;
		}
		if (distance < 0) {
			isIn = false;
		}
	}
	return isIn;
}*/
bool Window::inViewSpace(glm::vec3 cam_pos, glm::vec3 lookAt, glm::vec3 up,
	glm::vec3 origin, GLfloat radius) {
	bool isIn = true;
	GLfloat distance;
	glm::vec3 dir, nc, fc, X, Y, Z;
	GLfloat tang = (GLfloat)tan(45.0f*ANG2RAD*0.5f);
	nh = nDis * tang;
	nw = nh;
	fh = fDis * tang;
	fw = fh;
	Z = cam_pos - lookAt;
	Z = glm::normalize(Z);
	X = up * Z;
	X = glm::normalize(X);
	Y = Z * X;
	nc = cam_pos - Z * nDis;
	fc = cam_pos - Z * fDis;
	fc = cam_pos + lookAt * fDis;
	ntl = nc + Y * nh - X * nw;
	ntr = nc + Y * nh + X * nw;
	nbl = nc - Y * nh - X * nw;
	nbr = nc - Y * nh + X * nw;

	ftl = fc + Y * fh - X * fw;
	ftr = fc + Y * fh + X * fw;
	fbl = fc - Y * fh - X * fw;
	fbr = fc - Y * fh + X * fw;



	//top
	pl[0].set(ntr, ntl, ftl);
	//bottom
	pl[1].set(nbl, nbr, fbr);
	//left
	pl[2].set(ntl, nbl, fbl);
	//right
	pl[3].set(nbr, ntr, fbr);
	//nearP
	pl[4].set(ntl, ntr, nbr);
	//farP
	pl[5].set(ftr, ftl, fbl);

	for (int i = 0; i < 6; i++) {
		distance = pl[i].distance(origin);
		/*if (distance < -radius) {
			return false;
		}
		else if (distance < radius){
			isIn = true;
		}*/
		if (distance < 0) {
			isIn = false;
		}
	}
	return isIn;




}
void Window::pushD(GLfloat dis,std::vector<GLfloat> m) {
	m.push_back(dis);
}


void Window::drawLine(GLuint shaderProgram)
{
	glGenVertexArrays(1, &VAO); 
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, ctrPts.size() * sizeof(GLfloat) * 3, ctrPts.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); 
	glm::mat4 modelview = V;
	GLuint uProjection = glGetUniformLocation(curveShader, "projection");
	GLuint uModelview = glGetUniformLocation(curveShader, "modelview");
	

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glBindVertexArray(VAO);

	
	glDrawArrays(GL_LINES, 0, ctrPts.size());
	glBindVertexArray(0);


}

