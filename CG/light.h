#pragma once
#define Pi 3.141
#include "shader_m.h"

float lightBox[] = {
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f, 
	0.5f,  0.5f, -0.5f, 
	0.5f,  0.5f, -0.5f, 
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f, 
	0.5f,  0.5f,  0.5f, 
	0.5f,  0.5f,  0.5f, 
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	0.5f,  0.5f,  0.5f, 
	0.5f,  0.5f, -0.5f, 
	0.5f, -0.5f, -0.5f, 
	0.5f, -0.5f, -0.5f, 
	0.5f, -0.5f,  0.5f, 
	0.5f,  0.5f,  0.5f, 

	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f, 
	0.5f, -0.5f,  0.5f, 
	0.5f, -0.5f,  0.5f, 
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	0.5f,  0.5f, -0.5f, 
	0.5f,  0.5f,  0.5f, 
	0.5f,  0.5f,  0.5f, 
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
};

class Light {
public:
	glm::vec3 lightPos;
	glm::vec3 lightColors[3] = { glm::vec3(1.0f, 0.5f, 0.1f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.1f, 0.1f, 0.3f) };
	glm::vec3 lightColor;
	glm::vec3 lightDir;
	Shader lampShader;
	float angle = 0.008f;
	float angle_total = 0;
	float y_offset = 0.0f;
	float constant = 1.0f;
	float linear = 0.009f;
	float quadratic = 0.00032f;
	//float linear = 0.0f;
	//float quadratic = 0.0f;
	float port;
	bool isDay;
	Light() {

	}

	Light(bool god) {
		lightPos = glm::vec3(95.0f, 0.0f, 0.0f);
		//lampShader = Shader("1.lamp.vs", "1.lamp.fs");
		lightColor = glm::vec3(1.0f, 0.5f, 0.1f);
		setupLight();
	}

	void drawLight(glm::mat4 view, glm::mat4 projection) {
		//if (angle_total > 3 * Pi / 2) return;
		//lightPos.y -= y_offset;
		float x = lightPos.x * cos(angle) - lightPos.y * sin(angle);
		float y = lightPos.x * sin(angle) + lightPos.y * cos(angle);
		if (angle_total < Pi / 2) y_offset += 0.01f;
		//else y_offset -= 0.01f;
		//y += y_offset;

		lightPos = glm::vec3(x, y, 25.0f);
		lightDir = glm::vec3(0.0f, 0.0f, 0.0f) - lightPos;
		angle_total += angle;
		//lightColor = glm::vec3(1.0f * sin(angle_total), 1.0f * sin(angle_total), 1.0f * sin(angle_total));
		if (angle_total >= 2 * Pi) angle_total -= 2 * Pi;

		if (0 <= angle_total && angle_total < Pi / 6.0) {
			port = angle_total / (Pi / 6.0);
			isDay = true;
		}
		else if (angle_total > 5 * Pi / 6 && angle_total <= Pi) {
			port = (Pi - angle_total) / (Pi / 6.0);
			isDay = true;
		}
		else if (angle_total > Pi && angle_total < 7 * Pi / 6) {
			port = -(angle_total - Pi) / (Pi / 6.0);
			isDay = false;
		}
		else if (angle_total > 11 * Pi / 6.0 && angle_total < 2 * Pi) {
			port = -(2 * Pi - angle_total) / (Pi / 6.0);
			isDay = false;
		}

		if (port > 0) {
		//	lightColor = glm::vec3(0.0f, 0.0f, 0.0f);
		//}
		//else {
			lightColor.x = lightColors[1].x * port + lightColors[0].x * (1 - port);
			lightColor.y = lightColors[1].y * port + lightColors[0].y * (1 - port);
			lightColor.z = lightColors[1].z * port + lightColors[0].z * (1 - port);
		}
		else {
			lightColor.x = lightColors[2].x * -port + lightColors[0].x * (1 + port);
			lightColor.y = lightColors[2].y * -port + lightColors[0].y * (1 + port);
			lightColor.z = lightColors[2].z * -port + lightColors[0].z * (1 + port);
		}
		//std::cout << lightColor.x << std::endl;
		// also draw the lamp object
	/*	lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		glm::mat4 model(1.0f);
		model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		model = glm::scale(model, glm::vec3(2.0f));
		lampShader.setMat4("model", model);
		lampShader.setVec3("color", lightColor);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);*/

		//lightColor.x -= 0.00001f;
	}

	void setupLight() {

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// first triangle setup
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(lightBox), lightBox, GL_STATIC_DRAW);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//glBindVertexArray(0);


	}

private:
	unsigned int VBO, VAO;


};