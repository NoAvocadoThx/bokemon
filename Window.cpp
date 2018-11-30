#include "window.h"
#include "OBJObject.h"
#include "Geometry.h"
#include "Transform.h"
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

glm::vec3 p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13,
p14, p15, p16, p17, p18, p19, p20,p21,p22, p23;

Transform * modelMtx;
Transform * singleArmy;
Transform * army;

GLfloat size = 1.0f;
GLint OBJMode = 1;
GLint ptCount = 0;
GLint sptCount = 0;
GLdouble c_initX, c_initY,c_currX,c_currY;
glm::vec3 lastPoint,currPoint;
GLfloat rotationSpeed=1.5f;
GLint normalColor = 1;
GLfloat scroll=0;
GLint grid_size=10;
GLfloat gridSpace =6.0f;
GLint indexSelect=0;
GLint counter=0;
GLfloat lineSeg;
GLint countCurve;
GLint riderCount;
GLint counterR = 0;
GLint counterCurveN;
GLint lineSegN;
glm::mat4 mat =glm::mat4(1.0f);
glm::vec3 top;

vector<Curve*> curves;
vector<glm::vec3> anchor;
vector<glm::vec3> ctrPts;
vector<glm::vec3> selectPts;
glm::vec3 currPt;
glm::vec3 selectColor= glm::vec3(0.1f, 0.9f, 0.1f);

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
	sphere = new OBJObject("../sphere.obj");
	sphere->scaleSize(0.3f);
	cube = new Cube();

	RC = new OBJObject("../sphere.obj");
	//RC->scaleSize(20.0f);
	RC->objRotation = false;
	RC->camRot = false;
	//RC->translate(glm::vec3(2.0f, 1.0f, 1.0f));
	currObj = RC;
	randn = RC->randomFloat(0.0f, 35.0f);
	
	//define points
	//c0
	p0 = glm::vec3(10.0f, 14.0f, -15.0f);
	p1 = glm::vec3(5.0f, 8.0f, -15.0f);
	p2 = glm::vec3(10.0f, 10.0f, -15.0f);
	p3 = glm::vec3(18.0f, 5.0f, -10.0f);
	//c1
	p4 = glm::vec3(p3 - p2 + p3);
	p5 = glm::vec3(0.0f, 5.0f, 5.0f);
	p6 = glm::vec3(10.0f, 8.0f, -15.0f);
    //c2
	p7 = glm::vec3(p6 - p5 + p6);
	p8 = glm::vec3(15.0f, 0.0f, -10.0f);
	p9 = glm::vec3(6.0f, 8.0f, -18.0f);
	//c3
	p10 = glm::vec3(p9 - p8 + p9);
	p11 = glm::vec3(8.0f, 5.0f, 8.0f);
	p12 = glm::vec3(-5.0f, -5.0f, 10.0f);
	//c4
	p13 = glm::vec3(p12 - p11 + p12);
	p14 = glm::vec3(-10.0f, 4.0f, 14.0f);
	p15 = glm::vec3(-10.0f, -5.0f, -14.0f);
	//c5
	p16 = glm::vec3(p15 - p14 + p15);
	p17 = glm::vec3(0.0f, 0.0f, -10.0f);
	p18 = glm::vec3(18.0f, 10.0f, -10.0f);
	//c6
	p19 = glm::vec3(p18 - p17 + p18);
	p20 = glm::vec3(16.0f, 15.0f, -20.0f);
	p21 = glm::vec3(13.0f, 10.0f, -13.0f);
	//c7
	p22 = glm::vec3(p21 - p20 + p21);
	p23 = glm::vec3(p0 - p1 + p0);
	currPt = p0;
	//define curves
	c0 = new Curve(p0, p1, p2, p3);
	c1 = new Curve(p3, p4, p5, p6);
	c2 = new Curve(p6, p7, p8, p9);
	c3 = new Curve(p9, p10, p11, p12);
	c4 = new Curve(p12, p13, p14, p15);
	c5 = new Curve(p15, p16, p17, p18);
	c6 = new Curve(p18, p19, p20, p21);
	c7 = new Curve(p21, p22, p23, p0);
	//push curves
	curves.push_back(c0);
	curves.push_back(c1);
	curves.push_back(c2);
	curves.push_back(c3);
	curves.push_back(c4);
	curves.push_back(c5);
	curves.push_back(c6);
	curves.push_back(c7);
	
	for (Curve *c : curves) {
		//push anchors
		anchor.push_back(c->p0);
		//push selectable points
		selectPts.push_back(c->p0);
		selectPts.push_back(c->p1);
		selectPts.push_back(c->p2);
	}
	
	//push control points
	
	ctrPts.push_back(p2);
	ctrPts.push_back(p4);
	ctrPts.push_back(p5);
	ctrPts.push_back(p7);
	ctrPts.push_back(p8);
	ctrPts.push_back(p10);
	ctrPts.push_back(p11);
	ctrPts.push_back(p13);
	ctrPts.push_back(p14);
	ctrPts.push_back(p16);
	ctrPts.push_back(p17);
	ctrPts.push_back(p19);
	ctrPts.push_back(p20);
	ctrPts.push_back(p22);
	ctrPts.push_back(p1);
	ctrPts.push_back(p23);
	
	
	

	RC->scaleSize(0.9f);
	RC->translate(p0);
	
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skyboxShader = LoadShaders(SKYBOX_VERT, SKYBOX_FRAG);
	reflectShader = LoadShaders(REFLECT_VERT, REFLECT_FRAG);
	curveShader = LoadShaders(CURVE_VERT, CURVE_FRAG);
   sphereShader = LoadShaders(SPHERE_VERT,SPHERE_FRAG);
   
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
	delete(currObj);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(skyboxShader);
	glDeleteProgram(reflectShader);
    glDeleteProgram(sphereShader);

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
	GLfloat temp=-88;
	for (Curve * c : curves) {
		for (glm::vec3 p: c->curvePts) {
			if (p.y > temp) {
				temp = p.y;
				top = p;
			}
		}
	}
	if (!pause) {
		move();
	}
	if (reachTop) {
		lineSeg = 0;
		counter = 0;
		countCurve = 0;
		reachTop = false;
	}
	
}
void Window::move() {
   GLfloat speed=2.0f;
   GLfloat tempV=0.0f;
   GLfloat acceleration=0.000005f;
   GLfloat curHeight=mat[3][1];
   
   Curve * curCurve = curves.at(countCurve);
  // glm::vec3 nextPoint = curCurve->curvePts.at(lineSeg);
   if (rider) {
	
	   Curve * curRCurve = curves.at(counterR);

	   glm::vec3 nextPoint = curRCurve->curvePts.at(riderCount);
	   mat[3] = glm::vec4(nextPoint, 1.0f);
	  
	   cam_pos = nextPoint;
	   //cam_pos = pow(1 - riderCount / 150.0f, 3)*curRCurve->p0 + 3 * pow(1 - riderCount / 150.0f, 2)*riderCount / 150.0f*curRCurve->p1 +
		 //  3 * (1 - riderCount / 150.0f)*riderCount / 150.0f*riderCount / 150.0f*curRCurve->p2 + riderCount / 150.0f * riderCount / 150.0f*riderCount / 150.0f*curRCurve->p3;
	   cam_look_at = glm::vec3((-3 * (1 - riderCount / 150.0f)*(1 - riderCount / 150.0f))*glm::vec3(curRCurve->p0.x, curRCurve->p0.y, curRCurve->p0.z)
		   + (GLfloat)(9 * riderCount / 150.0f*riderCount / 150.0f - 12 * riderCount / 150.0 + 3)*glm::vec3(curRCurve->p1.x, curRCurve->p1.y, curRCurve->p1.z)
		   + (GLfloat)(-9 * riderCount / 150.0f*riderCount / 150.0f + 6 * riderCount / 150.0f)*glm::vec3(curRCurve->p2.x, curRCurve->p2.y, curRCurve->p2.z) + 3 * riderCount / 150.0f*riderCount / 150.0f*glm::vec3(curRCurve->p3.x, curRCurve->p3.y, curRCurve->p3.z));

	   V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	   riderCount = riderCount+1;
	   if (riderCount == 150) {
		   riderCount = 0;
		   counterR++;
	   }
	   if (counterR == 8) {
		   counterR = 0;
	   }
	  
	   
		 
		   
		 
	  
   }
   else {
	   cam_pos=glm::vec3(0.0f, 0.0f, 20.0f);
	   cam_look_at=glm::vec3(0.0f, 0.0f, 0.0f);
	   V = glm::lookAt(cam_pos, cam_look_at, cam_up);
   }
  
  if (reachTop) {
	   lineSeg = 0;
	   counter = 0;
	   countCurve = 0;
	   reachTop = false;
   }
   GLfloat topY = top.y;
   GLfloat heightChange = curHeight - topY;
   speed = (GLfloat)sqrt(-9.0f*heightChange*acceleration)+0.001f;
   if (lineSeg + speed <= 1.0f) {
	   lineSeg = counter / 150.0f + speed;
	   glm::vec4 T = glm::vec4(pow(lineSeg, 3.0f), pow(lineSeg, 2.0f), lineSeg, 1.0f);
	   mat[3] = curCurve->G*curCurve->B*T;
	   counter++;
	   if (!normal) {
		   cout << "v: " << speed << endl;
	   }
   }
   else {
	   lineSeg -= 1;
	   counter = 0;
	   countCurve++;
   }
   if (countCurve == 8) {
	   countCurve = 0;
	 
   }
    
	if(normal){
	 Curve * curRCurve = curves.at(counterCurveN);

	   glm::vec3 nextPoint = curRCurve->curvePts.at(lineSegN);
	   mat[3] = glm::vec4(nextPoint, 1.0f);mat[3] = glm::vec4(nextPoint, 1.0f);
   lineSegN=lineSegN+2.0f;
   if (lineSegN == 150) {
       lineSegN = 0;
        counterCurveN++;
   }
   if (counterCurveN == 8) {
	   counterCurveN = 0;
   }
   //cout << "v: " << speed << endl;
   }
}

void Window::display_callback(GLFWwindow* window)
{
	glm::vec3 rOrigin;
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//calculateFrustum();
	// Use the shader of programID
	glUseProgram(skyboxShader);
	cube->draw(skyboxShader);
	glUseProgram(curveShader);
	
	
	for (Curve* c : curves) {
		c->draw(curveShader);
	}
	glUseProgram(reflectShader);
	RC->draw(reflectShader,mat);
	//draw anchors
	glUseProgram(sphereShader);
	
	glm::mat4 transMat = glm::mat4(1.0f);
	transMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.15f));
	//draw anchors
	for (int i = 0; i < anchor.size(); i++) {
		transMat[3] = glm::vec4(anchor[i], 1.0f);
		glUniform3f(glGetUniformLocation(sphereShader, "pickColor"), 0.9f, 0.1f, 0.1f);
		if (i * 3 == indexSelect) {
			glUniform3f(glGetUniformLocation(sphereShader, "pickColor"), 0.3f, 0.1f, 0.8f);
		}
		sphere->draw(sphereShader, transMat);
		
	}
	//draw control points
	
	for (int i = 0; i < ctrPts.size(); i++) {
		transMat[3] = glm::vec4(ctrPts[i], 1.0f);
		glUniform3f(glGetUniformLocation(sphereShader, "pickColor"), selectColor.x, selectColor.y, selectColor.z);
		if ((i == 0&&indexSelect==2) ) {
			glUniform3f(glGetUniformLocation(sphereShader, "pickColor"), 0.3f, 0.1f, 0.8f);
		}
		sphere->draw(sphereShader, transMat);
	}

	drawLine(curveShader);

	calculate();
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
void Window::calculate() {
	if (selection) {
		int i = 0;
		p0 = selectPts[i];
		i++;//1
		p1 = selectPts[i];
		i++;//2
		p2 = selectPts[i];
		i++;//3
		p3 = selectPts[i];
		i++;//4
		p4 = selectPts[i];
		i++;//5
		p5 = selectPts[i];
		i++;//6
		p6 = selectPts[i];
		i++;//7
		p7 = selectPts[i];
		i++;//8
		p8 = selectPts[i];
		i++;//9
		p9 = selectPts[i];
		i++;//10
		p10 = selectPts[i];
		i++;//11
		p11 = selectPts[i];
		i++;//12
		p12 = selectPts[i];
		i++;//13
		p13 = selectPts[i];
		i++;//14
		p14 = selectPts[i];
		i++;//15
		p15 = selectPts[i];
		i++;//16
		p16 = selectPts[i];
		i++;//17
		p17 = selectPts[i];
		i++;//18
		p18 = selectPts[i];
		i++;//19
		p19 = selectPts[i];
		i++;//20
		p20 = selectPts[i];
		i++;//21
		p21 = selectPts[i];
		i++;//22
		p22 = selectPts[i];
		i++;//23
		p23 = selectPts[i];
	}
	//if (indexSelect == 2 || indexSelect == 5 || indexSelect == 8 || indexSelect == 11 || indexSelect == 14 ||
	//	indexSelect == 17 || indexSelect == 20 || indexSelect == 23) {
		p1 = (p0 + p0 - p23);
		p4 = (p3 + p3 - p2);
		p7 = (p6 + p6 - p5);
		p10 = (p9 + p9 - p8);
		p13 = (p12 + p12 - p11);
		p16 = (p15 + p15 - p14);
		p19 = (p18 + p18 - p17);
		p22 = (p21 + p21 - p20);
	//}
    if (indexSelect == 1 || indexSelect == 4 || indexSelect == 7 || indexSelect == 10 || indexSelect == 13 ||
		indexSelect == 16 || indexSelect == 29 || indexSelect == 22){

		/*p23 = p0 + p0 - p1;
		p2 = p3 + p3 - p4;
		p5 = p6 + p6 - p7;
		p8 = p0 + p9 - p10;
		p11 = p12 + p12 - p13;
		p14 = p15 + p15 - p16;
		p17 = p18 + p18 - p19;
		p20 = p21 + p21 - p22;*/
		
	}
	ctrPts.clear();
	ctrPts.push_back(p2);
	ctrPts.push_back(p4);
	ctrPts.push_back(p5);
	ctrPts.push_back(p7);
	ctrPts.push_back(p8);
	ctrPts.push_back(p10);
	ctrPts.push_back(p11);
	ctrPts.push_back(p13);
	ctrPts.push_back(p14);
	ctrPts.push_back(p16);
	ctrPts.push_back(p17);
	ctrPts.push_back(p19);
	ctrPts.push_back(p20);
	ctrPts.push_back(p22);
	ctrPts.push_back(p1);
	ctrPts.push_back(p23);
	c0 = new Curve(p0, p1, p2, p3);
	c1 = new Curve(p3, p4, p5, p6);
	c2 = new Curve(p6, p7, p8, p9);
	c3 = new Curve(p9, p10, p11, p12);
	c4 = new Curve(p12, p13, p14, p15);
	c5 = new Curve(p15, p16, p17, p18);
	c6 = new Curve(p18, p19, p20, p21);
	c7 = new Curve(p21, p22, p23, p0);
	curves.clear();
	curves.push_back(c0);
	curves.push_back(c1);
	curves.push_back(c2);
	curves.push_back(c3);
	curves.push_back(c4);
	curves.push_back(c5);
	curves.push_back(c6);
	curves.push_back(c7);
	anchor.clear();
	for (Curve *c : curves) {
		//push anchors
		anchor.push_back(c->p0);
	}
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
	

		//change the color
		if (key == GLFW_KEY_R) {
			if (nToggle) {
				rider = false;
				nToggle = !nToggle;
			}
			else {
				rider = true;
				nToggle = !nToggle;
			}
		}
		//color change
	
		//direction Light toggle
		if (key == GLFW_KEY_3) {
			
			currObj->ifDirSource = true;
			
			if (dirToggle) {
				currObj->dirOn = 1;
				dirToggle = !dirToggle;
			}
			else {
				currObj->dirOn = 0;
				dirToggle = !dirToggle;
			}
		}
		
		
		//make spot light narrow or wider
		if (key == GLFW_KEY_W) {

			if (mods == GLFW_MOD_SHIFT) {
				
				currObj->sptCutOff += 1.0f;
				currObj->sptCutOff2 += 1.0f;
			}
			else {
				currObj->sptCutOff -= 1.0f;
				currObj->sptCutOff2 -= 1.0f;
			}
		}
		//sharpen or make spot light blurrier
		if (key == GLFW_KEY_E) {

			if (mods == GLFW_MOD_SHIFT) {
				if (currObj->sptCutOff2-currObj->sptCutOff>=0.5f) {
					currObj->sptCutOff += 0.5f;
					//currObj->sptCutOff2 += 1.1f;
				}
			}
			else {
				if (currObj->sptCutOff >= 2.0f){
					currObj->sptCutOff -= 0.5f;
				//currObj->sptCutOff2 -= 1.0f;
			    }
		     }
		}
			//object rotation on/off
		if (key == GLFW_KEY_0) {

			if (currObj->objRotation) {
				currObj->objRotation = false;
					
			}
			else {
				currObj->objRotation = true;
					
			}
		}
		//point light rotation switch
		if (key == GLFW_KEY_1) {

			if (currObj->ptRot) {
				currObj->ptRot = false;

			}
			else {
				currObj->ptRot = true;

			}
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
		//debug mode switch
		if (key == GLFW_KEY_D) {

			if (nToggle) {
				debugMode = false;
				nToggle = !nToggle;
			}
			else {
				debugMode = true;
				nToggle = !nToggle;
			}


		}
		//cull mode switch
		if (key == GLFW_KEY_C) {

			if (nToggle) {
				cull = false;
				nToggle = !nToggle;
			}
			else {
				cull = true;
				nToggle = !nToggle;
			}

		}
		
		//render sphere
		if (key == GLFW_KEY_S) {

			if (nToggle) {
				selection = false;
				nToggle = !nToggle;
			}
			else {
				selection = true;
				nToggle = !nToggle;
			}

		}

		if (key == GLFW_KEY_X) {
			if (mods == GLFW_MOD_SHIFT) {
				selectPts[indexSelect].x -= 0.3f;
			}
			else {
				selectPts[indexSelect].x += 0.3f;
			}
		}
		if (key == GLFW_KEY_Y) {
			if (mods == GLFW_MOD_SHIFT) {
				selectPts[indexSelect].y -= 0.3f;
			}
			else {
				selectPts[indexSelect].y += 0.3f;
			}
		}
		if (key == GLFW_KEY_Z) {
			if (mods == GLFW_MOD_SHIFT) {
				selectPts[indexSelect].z -= 0.3f;
			}
			else {
				selectPts[indexSelect].z += 0.3f;
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
			if (indexSelect == selectPts.size() - 1) {
				indexSelect = 0;
			}
			else {
				indexSelect += 1;
			}
		
			//cout << "selection xyz:" << selectPts[indexSelect].x << endl;

		}
		//if release
		else if (action == GLFW_RELEASE) {
			left_release = true;
		}
		
	}
	else if (button == GLFW_MOUSE_BUTTON_2) {
		if (action == GLFW_PRESS) {
			if (indexSelect == 0) {
				indexSelect = selectPts.size() - 1;
			}
			else {
				indexSelect -= 1;
			}
			glUniform3f(glGetUniformLocation(sphereShader, "pickColor"), 0.5f, 0.1f, 0.1f);
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

