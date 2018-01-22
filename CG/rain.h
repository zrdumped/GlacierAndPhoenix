#pragma once
#include "particle.h"


class Rain {
public:
	Shader rainShader;
	Mesh rainMesh;
	ParticleGenerator rainPG;
	float scale = 1.0f;

	Rain(const char* path) {
		rainShader = Shader("rain.vs", "rain.fs");
		rainMesh = Mesh(path, true, false);
		rainPG = ParticleGenerator(rainShader, rainMesh, 1500, 10, glm::vec3(-10.0, 20.0, -10.0),0, glm::vec3(20.0, 0.0, 20.0),
			glm::vec4(1.0, 1.0, 1.0, 0.1), glm::vec4(0.0, 0.0, 0.0, 0.0),
			glm::vec4(1.0, 1.0, 1.0, 0.1), 1,
			15.0, 2.0, 0.0, 10.0, 2.0, glm::vec3(0.0, 1.0, 0.0),
			glm::vec3(0.0, -1, 0.0), glm::vec3(-0.00, -0.3, -0.00),
			0, scale);
	}

	void SetWeather(bool ing) {
		if (ing) rainPG.UpdateGrow(15);
		else rainPG.UpdateGrow(0);
	}

	void Draw(glm::mat4 view, glm::mat4 projection, glm::vec3 lightpos, Camera camera) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//fire.setShader(rainShader);
		//c.setShader(rainShader);
		rainPG.DrawParticle(view, projection, lightpos, camera.Position);
		rainPG.UpdateGenPos(glm::vec3(camera.Position.x - 10.0f, 20.0f, camera.Position.z - 10.0f));
		rainPG.Update(0.2);
		glDisable(GL_BLEND);
	}

};