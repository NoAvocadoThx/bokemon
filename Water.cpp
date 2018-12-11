#include "Water.h"
#include "Window.h"
#include "camera.h"


Water::Water() {
	toWorld = glm::mat4(1.0f);
	scale(1000.0f);
	translate(glm::vec3(-size/2 , -8.5f, - size/2 ));
	//startTime = glfwGetTime();
	GLfloat vertices[] = {-1,-1,-1,1,1,-1,1,-1,-1,1,1,1};
	texture[0] = loadTexture("../Water.jpg");
	dudvMap = loadTexture("../WaterDUDV");
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
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//initialize frameBuffers
	//first reflection FB
	glGenFramebuffers(1, &reflectionFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, reflectionFrameBuffer);
	glGenTextures(1, &reflectionTexture);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, reflec_width, reflec_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectionTexture, 0);
	//refraction FB
	glGenFramebuffers(1, &refractionFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, refractionFrameBuffer);
	glGenTextures(1, &refractionTexture);
	glBindTexture(GL_TEXTURE_2D, refractionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, refrac_width, refrac_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, refractionTexture, 0);
    //depth FB
	//reflection
	glGenRenderbuffers(1, &reflectionDB);
	glBindRenderbuffer(GL_RENDERBUFFER, reflectionDB);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, reflec_width, reflec_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, reflectionDB);
	//reraction
	glGenRenderbuffers(1, &refractionDB);
	glBindRenderbuffer(GL_RENDERBUFFER, refractionDB);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, refrac_width, refrac_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, refractionDB);

}


void Water::draw(GLuint shaderProgram) {
	glm::mat4 modelview = Window::V * toWorld;
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);
	//glUniform3f(glGetUniformLocation(shaderProgram, "cameraPos"), Camera::position.x, Camera::position.y, Camera::position.z);
	glBindVertexArray(VAO);
	curTime = glfwGetTime();
	GLfloat flow=0;
	flow+= 0.2*(curTime - startTime);
	glUniform1f(glGetUniformLocation(shaderProgram, "flow"), flow);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "reflectionTexture"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractionTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "refractionTexture"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dudvMap);
	glUniform1i(glGetUniformLocation(shaderProgram, "dudv"), 3);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertices));

	glBindVertexArray(0);
}

void Water::bindReflectionFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, reflectionFrameBuffer);
	glViewport(0, 0, reflec_width, reflec_height);
}
void Water::bindRefractionFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, refractionFrameBuffer);
	glViewport(0, 0, refrac_width, refrac_height);
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

	glGenerateMipmap(GL_TEXTURE_2D);  // Generate mipmaps
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);
	
}

GLfloat Water::getHeight() {
	return waterHeight;
}

void Water::scale(GLfloat scalor) {
	toWorld *= glm::scale(glm::mat4(1.0), glm::vec3(scalor));
}
void Water::translate(glm::vec3 v) {
	toWorld = glm::translate(glm::mat4(1.0f), v) * toWorld;
}