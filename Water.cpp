#include "Water.h"
#include "Window.h"
#include "camera.h"


GLfloat flow = 0;
GLfloat reflectArray[] = { 1.0, 0.0, 0.0, 0.0,
0.0, -1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0 };

glm::mat4 reflectMat = glm::make_mat4(reflectArray);
Water::Water() {
	toWorld = glm::mat4(1.0f);
	
	
	//translate(glm::vec3(1, waterHeight, 1));
	startTime = glutGet(GLUT_ELAPSED_TIME);
	GLfloat vertices[] = { -1, -1, 0,
							-1, 1, 0,
							1, -1, 0,
							1, -1, 0,
							-1, 1, 0,
							1, 1, 0

	 };
	
	//glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f, size));
	scale(800);
	translate(glm::vec3(-500,0, -500));
	//toWorld = glm::translate(scale, glm::vec3(-10, -4, -10));
	//glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(100, 1.0f, 100));
	//toWorld = glm::translate(scale, glm::vec3(-25, -5, -20));
	texture[0] = loadTexture("../Water.jpg");
	dudvMap = loadTexture("../dudv2.png");
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);


	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array wi
	//glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//initialize frameBuffers
	//first reflection FB
	glGenFramebuffers(1, &reflectionFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, reflectionFrameBuffer);
	//glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glGenTextures(1, &reflectionTexture);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, reflec_width, reflec_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectionTexture, 0);
	//refraction FB
	glGenFramebuffers(1, &refractionFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, refractionFrameBuffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glGenTextures(1, &refractionTexture);
	glBindTexture(GL_TEXTURE_2D, refractionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, refrac_width, refrac_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, refractionTexture, 0);
    //depth FB
	//reflection
	glGenRenderbuffers(1, &reflectionDB);
	glBindRenderbuffer(GL_RENDERBUFFER, reflectionDB);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, reflec_width, reflec_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, reflectionDB);
	//reraction
	createDepthTexture();

}
Water::~Water()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VBO);
	glDeleteVertexArrays(1, &EBO);
	glDeleteFramebuffers(1, &reflectionFrameBuffer);
	glDeleteFramebuffers(1, &refractionFrameBuffer);
	glDeleteFramebuffers(1, &refractionDB);
	glDeleteFramebuffers(1, &reflectionDB);
}
void Water::createDepthTexture() {
	
	glGenTextures(1, &refracDepthTexture);
	glBindTexture(GL_TEXTURE_2D, refracDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, refrac_width, refrac_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		refracDepthTexture, 0);
}
void Water::draw(GLuint shaderProgram,glm::vec3 cam) {
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glm::mat4 modelview = Window::V * toWorld;
	glm::mat4 view = Window::V;
	glm::mat4 projection = Window::P;
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelview"), 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "reflection"), 1, GL_FALSE, &reflectMat[0][0]);
	glUniform3f(glGetUniformLocation(shaderProgram, "camPos"),cam.x, cam.y, cam.z );
	glBindVertexArray(VAO);
	curTime = glutGet(GLUT_ELAPSED_TIME);
	
	float d = curTime - startTime;
	startTime = curTime;
	flow += moveSpeed * d;
	flow = fmod(flow, 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "flow"), flow);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "reflectionTexture"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractionTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "refractionTexture"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dudvMap);
	glUniform1i(glGetUniformLocation(shaderProgram, "dudv"), 2);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

	glBindVertexArray(0);
}

void Water::bindReflectionFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, reflectionFrameBuffer);
	glViewport(0, 0, reflec_width, reflec_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Water::bindRefractionFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, refractionFrameBuffer);
	glViewport(0, 0, refrac_width, refrac_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Water::unbindCurrentFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Window::width, Window::height);
}
GLuint Water::loadTexture(std::string str) {
	GLuint texture;
	
	if (str.c_str() == NULL) {
		std::cout << "null" << std::endl;
		exit(1);
	}
	//unsigned char* tdata = loadPPM(str.c_str(), twidth, theight);
	//unsigned int textureID;
	//tetxure 1
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(str.c_str(), &width, &height, &nrChannels, 0);
	if (data) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	else std::cout << "Image failed to load at path: " << str.c_str() << std::endl;
	stbi_image_free(data);

	//glGenerateMipmap(GL_TEXTURE_2D);  // Generate mipmaps
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
	
}

GLfloat Water::getHeight() {
	return waterHeight;
}

void Water::scale(GLfloat scalor) {
	toWorld *= glm::scale(glm::mat4(1.0), glm::vec3(scalor,scalor,scalor));
}
void Water::translate(glm::vec3 v) {
	toWorld = glm::translate(glm::mat4(1.0f), v) * toWorld;
}