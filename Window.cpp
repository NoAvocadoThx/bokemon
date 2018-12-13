#include "window.h"

glm::vec3 direction;
GLfloat lastX = Window::width / 2.0f, lastY = Window::height / 2.0f;
bool firstMouseMove = true;
bool keyPressedStatus[1024]; 
GLfloat deltaTime = 0.0f; 
GLfloat lastFrame = 0.0f; 
bool fired = false;
bool walking = false;
bool debugMode = false;
const char* window_title = "GLFW Starter Project";
Cube * cube;
GLint toonShader;
GLint shaderProgram;
GLint skyboxShader;
GLint reflectShader;
GLint curveShader;
GLint sphereShader;
GLint terrainShader;
GLint waterShader;
GLint boundShader;
GLfloat fov = 45.0f;
GLint robotNum;
GLfloat distance;
bool isIn = false;
bool played = false;
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



BoundingBox *box;
BoundingBox *testbox, *testbox1, *testbox2;
ROBObject *body1,*body2,*body3, *ball2;
BALLObject *ball;

Curve *c0;
Curve *c1;
Curve *c2;
Curve *c3;
Curve *c4;
Curve *c5;
Curve *c6;
Curve *c7;
ROBObject * horse, *wolf, *cow, *sphere;
//Geometry * horse, *wolf, *cow, *sphere;
Transform * horsemtx, *wolfmtx, *cowmtx, *sphere_mtx, *tempmtx, *temp, *model_sphere;
Transform * modelMtx, *modelballMtx, *modelbody1Mtx, *modelbody2Mtx, *modelbody3Mtx;
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
Water *water;
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
bool cull;
bool renderSphere;
bool pause=false;
bool selection=true;
bool rider;
bool reachTop;
bool normal;
bool toggleWater;
bool toggleTerrain;
bool newHorse;

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
#define SOUND_PATH "../water.mp3"
#define SMASH_PATH "../smash.mp3"
#define WATER_VERT "../waterShader.vert"
#define WATER_FRAG "../waterShader.frag"
#define BOUND_VERT "../boundingShader.vert"
#define BOUND_FRAG "../boundingShader.frag"
int army_length = 1;
int Window::width;
int Window::height;
std::vector<GLfloat> Window::distanceVec;
glm::mat4 Window::P;
glm::mat4 Window::V;
bool Window::toggleSphere;
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
glm::vec3 camPos;
int seed=1;

// Sound System
irrklang::ISoundEngine * SoundEngine1;

void Window::initialize_objects()
{

	
	
	cube = new Cube();
	cube->scaleSize(1.0f);
	
		seed = randomInt();
		generator = new HeightGenerator(vertexCount, vertexCount, vertexCount, seed);
		terrain = new Terrain(generator);
		terrain->translate(glm::vec3(-terrain->vertexCount - 50, 0, -terrain->vertexCount - 100));
		terrain->scaleSize(3.0f, 1.0f, 3.0f);
	
	water = new Water();



	body1 = new ROBObject(HORSE_PATH);
	body2 = new ROBObject(HORSE_PATH);

	box = new BoundingBox(body1->boundingbox, body1->boxVertices);
	testbox = new BoundingBox(body2->boundingbox, body2->boxVertices);

	
	sphere = new ROBObject(BALL_PATH);
	sphere->isSphere = true;
	horse = new ROBObject(HORSE_PATH);
	
	testbox1 = new BoundingBox(sphere->boundingbox, sphere->boxVertices);
	testbox2 = new BoundingBox(horse->boundingbox, horse->boxVertices);
	sphere->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f));
	testbox1->toWorld = sphere->toWorld;
	horse->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f));
	horse->translate(glm::vec3(0, 15.0f, 0));
	testbox2->toWorld = horse->toWorld;


	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skyboxShader = LoadShaders(SKYBOX_VERT, SKYBOX_FRAG);
	reflectShader = LoadShaders(REFLECT_VERT, REFLECT_FRAG);
	curveShader = LoadShaders(CURVE_VERT, CURVE_FRAG);
    sphereShader = LoadShaders(SPHERE_VERT,SPHERE_FRAG);
	terrainShader = LoadShaders(TERRAIN_VERT, TERRAIN_FRAG);
	toonShader = LoadShaders(TOON_VERT, TOON_FRAG);
	waterShader = LoadShaders(WATER_VERT, WATER_FRAG);
	boxCP = glGetUniformLocation(skyboxShader, "clippingPlane");
	objCP = glGetUniformLocation(toonShader, "clippingPlane");
	terrainCP = glGetUniformLocation(terrainShader, "clippingPlane");
	boundShader = LoadShaders(BOUND_VERT, BOUND_FRAG);


	SoundEngine1 = irrklang::createIrrKlangDevice();
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
	glDeleteProgram(terrainShader);
	glDeleteProgram(toonShader);
	glDeleteProgram(waterShader);
	delete(SoundEngine1);
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
	checkcollision();
	//sphere->toWorld = camera.getViewMatrix();
	camPos = { camera.position.x, camera.position.y, camera.position.z };
	if (walking) {
		horse->walk();

	}
	//horse->toWorld[3].y = terrain->getHeightMove(horse->toWorld[3].x, horse->toWorld[3].z);
	if (!fired) {
		sphere->toWorld = glm::translate(glm::mat4(1.0f), camPos);
		sphere->viewdir = camera.forward;
	}
	if (sphere->finishedfire) {
		fired = false;
		sphere->firing = false;
		sphere->duration = 100;
	}
	testbox1->toWorld = sphere->toWorld;
	testbox2->toWorld = horse->toWorld;
}

void Window::display_callback(GLFWwindow* window)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CLIP_DISTANCE0);
	GLfloat currentFrame = (GLfloat)glfwGetTime();
	
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	glm::vec3 rOrigin;
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderReflection();
	renderRefraction();
	renderAll();
	//ball->draw(toonShader);
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
void Window::renderReflection() {
	water->bindReflectionFrameBuffer();
	GLfloat distance = 2 * (camera.position.y-1);
	camera.position.y -= distance;

	camera.invertPitch();
	V = camera.getViewMatrix();
	glm::vec4 CP = glm::vec4(0, 1, 0, 0.5);
	glUseProgram(skyboxShader);
	glUniform4f(boxCP, CP.x, CP.y, CP.z, CP.w);
	cube->draw(skyboxShader);
	if (toggleTerrain) {
		glUseProgram(terrainShader);
		glUniform4f(terrainCP, CP.x, CP.y, CP.z, CP.w);
		terrain->draw(terrainShader);
	}
	//glUseProgram(shaderProgram);

	//->draw(shaderProgram);
	glUseProgram(toonShader);
	glUniform4f(objCP, CP.x, CP.y, CP.z, CP.w);
	//glUseProgram(toonShader);
	//body1->draw(shaderProgram);

	glm::vec3 pos = { camera.position.x, camera.position.y, camera.position.z };
	//camPos = { camera.position.x, camera.position.y, camera.position.z };
	glUniform3fv(glGetUniformLocation(toonShader, "cameraPosition"), 1, &(pos[0]));
	//army->draw(toonShader, glm::mat4(1.0f));
	horse->draw(toonShader);
	sphere->draw(toonShader);
	glUseProgram(boundShader);
	if (debugMode) {
		testbox1->draw(boundShader);
		testbox2->draw(boundShader);
	}
	water->unbindCurrentFrameBuffer();
	camera.position.y += distance;

	camera.invertPitch();
	V = camera.getViewMatrix();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
void Window::renderRefraction() {

	water->bindRefractionFrameBuffer();
	glm::vec4 CP = glm::vec4(0, -1, 0,1);
	
	glUseProgram(skyboxShader);
	glUniform4f(boxCP, CP.x, CP.y, CP.z, CP.w);
	cube->draw(skyboxShader);
	if (toggleTerrain) {
		glUseProgram(terrainShader);
		glUniform4f(terrainCP, CP.x, CP.y, CP.z, CP.w);
		terrain->draw(terrainShader);
	}
	//glUseProgram(shaderProgram);

	//->draw(shaderProgram);
	glUseProgram(toonShader);
	glUniform4f(objCP, CP.x, CP.y, CP.z, CP.w);
	//glUseProgram(toonShader);
	//body1->draw(shaderProgram);

	glm::vec3 pos = { camera.position.x, camera.position.y, camera.position.z };
	//camPos = { camera.position.x, camera.position.y, camera.position.z };
	glUniform3fv(glGetUniformLocation(toonShader, "cameraPosition"), 1, &(pos[0]));
	//army->draw(toonShader, glm::mat4(1.0f));
	horse->draw(toonShader);
	sphere->draw(toonShader);



	glUseProgram(boundShader);
	//testbox1->draw(boundingShader, false);
	if (debugMode) {
		testbox1->draw(boundShader);
		testbox2->draw(boundShader);
	}
	water->unbindCurrentFrameBuffer();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Window::renderAll() {
	glDisable(GL_CLIP_DISTANCE0);
  	glm::vec4 CP = glm::vec4(0, -1, 0, 1000.0f);
	//camPos = { camera.position.x, camera.position.y, camera.position.z };
	if (toggleTerrain) {
		glUseProgram(terrainShader);
		glUniform4f(terrainCP, CP.x, CP.y, CP.z, CP.w);
		terrain->draw(terrainShader);
	}
	glUseProgram(shaderProgram);

	//->draw(shaderProgram);
	glUseProgram(toonShader);
	glUniform4f(objCP, CP.x, CP.y, CP.z, CP.w);
	//glUseProgram(toonShader);
	//body1->draw(shaderProgram);
	glm::vec3 pos = { camera.position.x, camera.position.y, camera.position.z };
	glUniform3fv(glGetUniformLocation(toonShader, "cameraPosition"), 1, &(pos[0]));
	//army->draw(toonShader, glm::mat4(1.0f));
	horse->draw(toonShader);
	sphere->draw(toonShader);

	glUseProgram(boundShader);
	//testbox1->draw(boundingShader, false);
	if (debugMode) {
		testbox1->draw(boundShader);
		testbox2->draw(boundShader);
	}

	
	if (toggleWater) {
		glUseProgram(waterShader);
		water->draw(waterShader, camPos);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(skyboxShader);
	glUniform4f(boxCP, CP.x, CP.y, CP.z, CP.w);
	cube->draw(skyboxShader);
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
	

		if (key == GLFW_KEY_1) 
		{
			SoundEngine1->play2D(SOUND_PATH, GL_TRUE);
		}
		
		//spot light rotation switch
		if (key == GLFW_KEY_2) {

			SoundEngine1->stopAllSounds();
		}
		if (key == GLFW_KEY_3) {

			//SoundEngine->play2D(SMASH_PATH, GL_TRUE);
		}
		if (key == GLFW_KEY_5) {

			if (!debugMode) {
				debugMode = true;
			}
			else {
				debugMode = false;
			}

		}
		if (key == GLFW_KEY_T) {
			
			if (nToggle) {
				toggleTerrain = false;
				nToggle = !nToggle;
			}
			else {
				toggleTerrain = true;
				seed = randomInt();
				generator = new HeightGenerator(vertexCount, vertexCount, vertexCount, seed);
				terrain = new Terrain(generator);
				terrain->translate(glm::vec3(-terrain->vertexCount - 50, 0, -terrain->vertexCount - 100));
				terrain->scaleSize(3.0f, 1.0f, 3.0f);
				std::cout << "seed:" << seed << std::endl;
				nToggle = !nToggle;
			}

		}
		//water
		if (key == GLFW_KEY_N) {

			if (nToggle) {
				toggleWater = false;
				nToggle = !nToggle;
			}
			else {
				toggleWater = true;
				nToggle = !nToggle;
			}

		}
		if (key == GLFW_KEY_M) {
			if(walking) {
				walking = false;

			}
		else {
			walking = true;
			}
		}
		if (key == GLFW_KEY_P) {

			if (nToggle) {
				newHorse = false;
				nToggle = !nToggle;
			}
			else {
				newHorse = true;
				spawnHorse();
				nToggle = !nToggle;
			}

		}
	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{

		//Camera movement controls (FPS style)
		if (key == GLFW_KEY_W)	//Forward
		{
			//modelballMtx->translateZ(-0.2f);
			camera.handleKeyPress(FORWARD, deltaTime);
		}
		else if (key == GLFW_KEY_S) //Back
		{
			//modelballMtx->translateZ(0.2f);
			camera.handleKeyPress(BACKWARD, deltaTime);

		}
		//cout << "z" << endl;
		else if (key == GLFW_KEY_A) //Left
		{
			//modelballMtx->translateX(-0.2f);
			camera.handleKeyPress(LEFT, deltaTime);
		}
		else if (key == GLFW_KEY_D) //Right
		{
			//modelballMtx->translateX(0.2f);
			camera.handleKeyPress(RIGHT, deltaTime);
		}
		else if (key == GLFW_KEY_UP)
		{
			//ball->move(deltaTime);
			sphere->translateZ(-deltaTime*10);
			testbox1->toWorld = sphere->toWorld;
		}
		else if (key == GLFW_KEY_DOWN)
		{
			//ball->move(-deltaTime);
			sphere->translateZ(deltaTime*10);
			testbox1->toWorld = sphere->toWorld;
		}
		else if (key == GLFW_KEY_RIGHT)
		{
			//ball->spin(deltaTime);
			sphere->translateX(deltaTime*10);
			testbox1->toWorld = sphere->toWorld;
		}
		else if (key == GLFW_KEY_LEFT)
		{
			//ball->spin(-deltaTime);
			sphere->translateX(-deltaTime*10);
			testbox1->toWorld = sphere->toWorld;
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
			if (!fired) {
				sphere->firing = true;
				fired = true;
			}

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
	currPoint = trackBallMapping(glm::vec2(xpos, ypos));
	direction = currPoint - lastPoint;
	float velocity = glm::length(direction);
	if (velocity > 0.001)
	{
		glm::vec3 rotAxis;
		float rotAngle;
		rotAxis = glm::cross(lastPoint, currPoint);
		rotAngle = acos(glm::dot(lastPoint, currPoint) / (glm::length(lastPoint)*glm::length(currPoint)));
		sphere->rotAngle = rotAngle;
		sphere->rotAxis = rotAxis;
	}
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;
	lastPoint = currPoint;
	camera.handleMouseMove(xoffset, yoffset);
	V = camera.getViewMatrix();
}


void Window::checkcollision() {
	std::vector<float> bound1 = testbox1->getBoundary();
	std::vector<float> bound2 = testbox2->getBoundary();

		// check bound
		if (bound1[0] > bound2[1] && bound1[2] > bound2[3] && bound1[4] > bound2[5] &&
			bound1[1] < bound2[0] && bound1[3] < bound2[2] && bound1[5] < bound2[4]) {
			testbox1->collisionflag = true;
			testbox2->collisionflag = true;
			horse->dying = true;
			if (!played) {
				SoundEngine1->play2D(SMASH_PATH, GL_FALSE);
				played = true;
			}
		}
		else {
			testbox1->collisionflag = false;
			testbox2->collisionflag = false;
			played = false;
		}


	

}
void Window::spawnHorse() {
	horse = new ROBObject(HORSE_PATH);


	testbox2 = new BoundingBox(horse->boundingbox, horse->boxVertices);
	
	horse->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f));
	horse->translate(glm::vec3(0, 15.0f, 0));
	testbox2->toWorld = horse->toWorld;
}