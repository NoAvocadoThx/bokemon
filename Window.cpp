#include "window.h"


GLfloat lastX = Window::width / 2.0f, lastY = Window::height / 2.0f;
bool firstMouseMove = true;
bool keyPressedStatus[1024]; 
GLfloat deltaTime = 0.0f; 
GLfloat lastFrame = 0.0f; 


const char* window_title = "GLFW Starter Project";
Cube * cube;
GLint toonShader;
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
GLuint boxCP;
GLuint objCP;
GLuint terrainCP;



ROBObject *ball;
ROBObject *body1,*body2,*body3;


Curve *c0;
Curve *c1;
Curve *c2;
Curve *c3;
Curve *c4;
Curve *c5;
Curve *c6;
Curve *c7;

Geometry * horse, *wolf, *cow, *sphere;
Transform * horsemtx, *wolfmtx, *cowmtx, *sphere_mtx, *tempmtx, *temp;
Transform * modelMtx, * modelballMtx, *modelbody1Mtx, *modelbody2Mtx, *modelbody3Mtx;
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

glm::mat4 mat =glm::mat4(1.0f);
glm::vec3 top;

vector<Curve*> curves;
vector<glm::vec3> anchor;
vector<glm::vec3> ctrPts;
vector<glm::vec3> selectPts;
glm::vec3 currPt;
glm::vec3 selectColor= glm::vec3(0.4f, 0.3f, 0.1f);

Terrain *terrain;
HeightGenerator *generator;
GLint vertexCount = 128;
bool Window::SPHERE_SHOW = false;
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
#define TERRAIN_VERT "../terrainShader.vert"
#define TERRAIN_FRAG "../terrainShader.frag"
#define BALL_PATH "../ball.obj"
#define BODY_PATH "../body.obj"
#define HORSE_PATH "../horse.obj"
#define COW_PATH "../Irex_obj.obj"
#define WOLF_PATH "../wolf.obj"
#define TOON_VERT "../toonShader.vert"
#define TOON_FRAG "../toonShader.frag"
int army_length = 1;
int Window::width;
int Window::height;
std::vector<GLfloat> Window::distanceVec;
glm::mat4 Window::P;
glm::mat4 Window::V;
bool Window::toggleSphere;
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
void Window::initialize_objects()
{
	glEnable(GL_CLIP_DISTANCE0);
	body1 = new ROBObject(HORSE_PATH);
	sphere = new Geometry(BALL_PATH);
	horse = new Geometry(HORSE_PATH);
	wolf = new Geometry(WOLF_PATH);
	cow = new Geometry(COW_PATH);
	horsemtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -8.0f, 0.0f)));
	horsemtx->rotate(90);
	horsemtx->addChild(horse);

	wolfmtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(8.0f, -8.0f, -6.0f)));
	wolfmtx->rotate(90);
	wolfmtx->addChild(wolf);

	cowmtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, -8.0f, -9.0f)));
	cowmtx->rotate(90);
	cowmtx->addChild(cow);
	sphere_mtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	sphere_mtx->addChild(sphere);

	modelMtx = new Transform(glm::mat4(1.0f));
	modelMtx->addChild(horsemtx);
	modelMtx->addChild(wolfmtx);
	modelMtx->addChild(cowmtx);
	//modelMtx->addChild(sphere_mtx);

	temp = new Transform(glm::mat4(1.0f));
	temp->addChild(horsemtx);
	temp->addChild(wolfmtx);
	temp->addChild(cowmtx);

	army = new Transform(glm::mat4(1.0f));
	float space = 80.0f; // draws 100 robots
	for (int x = 0; x < army_length; x++) {
		for (int z = 0; z < army_length; z++) {

			tempmtx = new Transform(glm::translate(glm::mat4(1.0f), glm::vec3((float)x*space, 0.0f, (float)z*space)));

			tempmtx->children = modelMtx->children;
			army->addChild(tempmtx);
		}
	}

	
	int seed = rand();
	generator = new HeightGenerator(vertexCount, vertexCount, vertexCount, seed);



	cube = new Cube();

	terrain = new Terrain(generator);
	terrain->translate(glm::vec3(-terrain->vertexCount-50 , -10, -terrain->vertexCount-100));
	terrain->scaleSize(2.0f,1.0f,2.0f);

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skyboxShader = LoadShaders(SKYBOX_VERT, SKYBOX_FRAG);
	reflectShader = LoadShaders(REFLECT_VERT, REFLECT_FRAG);
	curveShader = LoadShaders(CURVE_VERT, CURVE_FRAG);
    sphereShader = LoadShaders(SPHERE_VERT,SPHERE_FRAG);
	terrainShader = LoadShaders(TERRAIN_VERT, TERRAIN_FRAG);
	toonShader = LoadShaders(TOON_VERT, TOON_FRAG);
	boxCP = glGetUniformLocation(skyboxShader, "clippingPlane");
	objCP = glGetUniformLocation(toonShader, "clippingPlane");
	terrainCP = glGetUniformLocation(terrainShader, "clippingPlane");
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
	glDeleteProgram(toonShader);
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
		P = glm::perspective(camera.mouse_zoom,
			(GLfloat)(Window::width) / Window::height, 1.0f, 1000.0f);
		V = camera.getViewMatrix();
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
	GLfloat currentFrame = (GLfloat)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	glm::vec3 rOrigin;
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(skyboxShader);

	cube->draw(skyboxShader);

	glUseProgram(terrainShader);
	terrain->draw(terrainShader);
	
	glUseProgram(shaderProgram);
	//->draw(shaderProgram);
	//glUseProgram(toonShader);
	body1->draw(shaderProgram);
	glm::vec3 pos = { camera.position.x, camera.position.y, camera.position.z };
	//glUniform3fv(glGetUniformLocation(toonShader, "eye_position"), 1, &(pos[0]));

	//army->draw(toonShader, glm::mat4(1.0f));
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
			camera.handleKeyPress(FORWARD, deltaTime);
		}
		else if (key == GLFW_KEY_S) //Back
		{

			camera.handleKeyPress(BACKWARD, deltaTime);

		}
		//cout << "z" << endl;
		else if (key == GLFW_KEY_A) //Left
		{
			camera.handleKeyPress(LEFT, deltaTime);
		}
		else if (key == GLFW_KEY_D) //Right
		{
			camera.handleKeyPress(RIGHT, deltaTime);
		}
		V = camera.getViewMatrix();
	}

}
void Window::translateCamera(glm::vec3 transVec) {
	V = glm::translate(glm::mat4(1.0f), transVec) * V;
}
void Window::scroll_callback(GLFWwindow* window, double x, double y) {
	

	camera.handleMouseScroll(y);
	V = camera.getViewMatrix();
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
		if (action == GLFW_RELEASE) {
			left_release = false;
			glfwGetCursorPos(window, &c_initX, &c_initY);
			lastPoint = trackBallMapping(glm::vec2(c_initX, c_initY));
			
		
			//cout << "selection xyz:" << selectPts[indexSelect].x << endl;

		}
		//if release
		else if (action = GLFW_PRESS) {
			left_release = true;
		}
		
	}
	else if (button == GLFW_MOUSE_BUTTON_2) {
		if (action == GLFW_PRESS) {
			
			//cout << "selection xyz:" << selectPts[indexSelect].x << endl;
			
		}
	}
}
void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouseMove)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouseMove = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.handleMouseMove(xoffset, yoffset);
	V = camera.getViewMatrix();
}
