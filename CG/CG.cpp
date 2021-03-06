// CG.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <mmsystem.h>
enum WEATHER { SUN, RAIN, SNOW, STORM };
#include "character.h"
#include "fire.h"
#include "light.h"
#include "fog.h"
#include "camera.h"
#include "floor.h"
#include "skybox.h"
#include "sky.h"
#include "rain.h"
#include "snow.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

glm::vec3 wind;

Character c;
// camera
//Camera camera(glm::vec3(0.0f, 4.0f, 5.0f));
Camera camera;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

// lighting
Light sun_moon;
glm::mat4 view;
glm::mat4 projection;


WEATHER w;
//glm::mat4 rm = glm::mat4(1.0f); //relief map

int main()
{
	w = SUN;
	wind = glm::vec3(0);
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//PlaySound(_T("wind.mp3"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	//PlaySound(_T("burn.mp3"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	// glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	//Shader ourShader("6.3.coordinate_systems.vs", "6.3.coordinate_systems.fs");
	//c = Character("snow2.obj");
	//c = Character("snow.obj");
	camera = Camera(c);


	//Shader floorShader("floor.vs", "floor.fs");
	//Shader lampShder("1.lamp.vs", "1.lamp.fs");
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	// second triangle setup
	/*glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorPositions), floorPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal coord attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);*/

	/*// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);*/
	//SkyBox sb = SkyBox();
	Sky sky("sky.obj");
	//SkyBox sb = SkyBox();
	Floor floor("floorfinal.obj");//, &rm);
	c = Character("bird/0.obj","snow2.obj", floor.relief);
	sun_moon = Light(true);
	Rain rain("rain.obj");
	Snow snow("snow2.obj", MILD);
	Snow storm("snow2.obj", BLIZZARD);
	Fog fog;
	Fire fire(5, "snow2.obj");


	// load and create a texture 
	// -------------------------
	/*unsigned int texture1, texture2;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load("funny.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("123.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);*/
	// texture 3
	// ---------
	/*glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load("snow.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);*/

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	//ourShader.use();
	//ourShader.setInt("texture1", 0);
	//ourShader.setInt("texture2", 1);



	// render loop
	// -----------
	float angle = 0;
	int blendTime = 0;
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

															// bind textures on corresponding texture units

															// be sure to activate shader when setting uniforms/drawing objects

															// activate shader
															//ourShader.use();
		// create transformations
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);




		sun_moon.drawLight(view, projection);
		fog.update(sun_moon);
		//sb.drawSkybox(view, projection, sun_moon.lightColor);

		floor.Draw(sun_moon, view, projection, camera.Position, fog, fire, c);



		sky.Draw(sun_moon, view, projection, camera.Position, fog);


		c.DrawCharacter(view, projection, sun_moon, camera.Position);
		//camera.MoveCameraByCharacter(c);
		// render boxes

		/*for (unsigned int i = 1; i < 10; i++)
		{

			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			angle += 0.01f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);

	//		glm::mat4 floor(1.0f);
		//	floor = glm::translate(floor, floorPositions);
//floor = glm::rotate(floor, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//	ourShader.setMat4("floor", floor);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}*/
		// pass transformation matrices to the shader
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//glActiveTexture(GL_TEXTURE1);
	//	glBindTexture(GL_TEXTURE_2D, texture2);


	/*	glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3);
		floorShader.use();
		floorShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		floorShader.setMat4("view", view);
		floorShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		floorShader.setVec3("lightPos", lightPos);
		glBindVertexArray(VAOs[1]);
		glm::mat4 floor(1.0f);
		//model = glm::translate(model, floorPositions);
		floor = glm::rotate(floor, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//= glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		// model = glm::rotate(model, (float)glfwGetTime() * glm::radians(70.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		floorShader.setMat4("floor", floor);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/

		//sb.drawSkybox(view, projection, sun_moon.lightColor);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------

		rain.Draw(view, projection, sun_moon.lightPos, camera);
		snow.Draw(view, projection, sun_moon.lightPos, camera, fire, c, sun_moon);
		storm.Draw(view, projection, sun_moon.lightPos, camera, fire, c, sun_moon);
		if (w == RAIN) {
			rain.SetWeather(true);
			snow.SetWeather(false);
			storm.SetWeather(false);
		}
			else if (w == SNOW)
			{
				rain.SetWeather(false);
				snow.SetWeather(true);
				storm.SetWeather(false);
			}
			else if(w == STORM)
			{
				rain.SetWeather(false);
				snow.SetWeather(false);
				storm.SetWeather(true);
			}
			else if (w == SUN) {
				rain.SetWeather(false);
				snow.SetWeather(false);
				storm.SetWeather(false);
			}


		c.DrawWing(view, projection, sun_moon, camera.Position);
		fire.Draw(view, projection, sun_moon, camera.Position, c.GetPosition());

		blendTime += 1;
		//if (blendTime < 10) {

		//}
		/*else {
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
			c.DrawWing(view, projection, sun_moon, camera.Position);
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		}*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	exit(0);
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		c.ProcessKeyboard(FORWARD, camera.GetHorizDir());
		camera.MoveCameraByCharacter(c);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		c.ProcessKeyboard(BACKWARD, camera.GetHorizDir());
		camera.MoveCameraByCharacter(c);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		c.ProcessKeyboard(LEFT, camera.GetHorizDir());
		camera.RotateCameraByCharacter(c, true);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		c.ProcessKeyboard(RIGHT, camera.GetHorizDir());
		camera.RotateCameraByCharacter(c, false);
	}

	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
		if (sun_moon.angle > 0.001f) sun_moon.angle -= 0.001f;
		else sun_moon.angle = 0;
		cout << sun_moon.angle << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
		sun_moon.angle += 0.001f;
		//cout << sun_moon.angle << endl;
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		w = SUN;
		wind = glm::vec3(0);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		w = RAIN;
		wind = glm::vec3(0);
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		w = SNOW;
		wind = glm::vec3(0);
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		w = STORM;
		wind = glm::vec3(1.0, 0.0, 0.0);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	//c.ProcessMouseMovement(xoffset, yoffset);
	//camera.MoveCameraByCharacter(c);
	camera.ProcessMouseMovement(xoffset, yoffset, c);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
