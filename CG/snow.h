#pragma once
//#include "particle_generator.h"
#include "particle.h"

#define MILD 0 
#define BLIZZARD 1
class Snow {
public:
	Shader snowShader;
	Mesh snowMesh;
	ParticleGenerator snowPG;
	int snowMode;
	//float scale = 0.1f;

	Snow(const char* path, int mode, const char* smogPath = NULL) {
		snowShader = Shader("snow.vs", "snow.fs");
		snowMesh = Mesh(path, true, false);
		snowMode = mode;
		if(mode == MILD)
		snowPG = ParticleGenerator(snowShader, snowMesh, 15000, 10, glm::vec3(-10.0, 20.0, -10.0), 0, glm::vec3(20.0, 0.0, 20.0),
			glm::vec4(1.0, 1.0, 1.0, 1), glm::vec4(0.0, 0.0, 0.0, 0.0),
			glm::vec4(1.0, 1.0, 1.0, 1), 1,
			80.0, 2.0,
			0.0, 10.0, 2.0, glm::vec3(0.0, 1.0, 0.0),
			glm::vec3(0.0, -0.2, 0.0), glm::vec3(-0.00, -0.03, -0.00),
			0.3, 0.1);
		else if (mode == BLIZZARD) 
			snowPG = ParticleGenerator(snowShader, snowMesh, 20000, 10, glm::vec3(-20.0, 20.0, -20.0), 0, glm::vec3(0.0, -20.0, 40.0),
				glm::vec4(1.0, 1.0, 1.0, 1), glm::vec4(0.0, 0.0, 0.0, 0.0),
				glm::vec4(1.0, 1.0, 1.0, 1), 1,
				20.0, 2.0,
				0.0, 10.0, 2.0, glm::vec3(0.0, 1.0, 0.0),
				glm::vec3(0.5, -0.3, 0.0), glm::vec3(-0.00, -0.15, -0.00),
				0.3, 0.2);
		snowMesh.setupTexture("SnowFlake.png");
		snowShader.use();
		snowShader.setInt("texture0", 0);
	}

	void SetWeather(bool ing) {
		if (ing && snowMode == BLIZZARD) snowPG.UpdateGrow(20);
		else if (ing && snowMode == MILD) snowPG.UpdateGrow(5);
		else if (!ing) snowPG.UpdateGrow(0);
	}

	void Draw(glm::mat4 view, glm::mat4 projection, glm::vec3 lightpos, Camera camera, Fire fire, Character c, Light light) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, snowMesh.texture);
		snowShader.use();
		fire.setShader(snowShader);
		c.setShader(snowShader, true);
		snowShader.setBool("isDay", light.isDay);
		//cout << light.isDay << endl;
		snowPG.DrawParticle(view, projection, lightpos, camera.Position);
		if(snowMode == 1)
			snowPG.UpdateGenPos(glm::vec3(camera.Position.x - 20.0f, 20.0f, camera.Position.z - 20.0f));
		else
			snowPG.UpdateGenPos(glm::vec3(camera.Position.x - 10.0f, 20.0f, camera.Position.z - 10.0f));
		snowPG.Update(0.2);
		glDisable(GL_BLEND);
	}

};