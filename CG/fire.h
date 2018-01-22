#include "particle.h"

class Fire {
public:
	Shader fireShader;
	Mesh* fireMesh;

	Fire(int num, const char* firePath) {
		fireNum = num;
		fireMesh = new Mesh[fireNum];
		firePos = new glm::vec3[fireNum];
		firePG = new ParticleGenerator[fireNum];
		onFire = new int[fireNum];
		fireTime = new int[fireNum];

		for (int i = 0; i < fireNum; i++) {
			firePos[i] = glm::vec3(myrand() * 20, 4.5, myrand() * 20);
			onFire[i] = 1;
			fireTime[i] = 0;
		}
		onFire[3] = 0;

		fireShader = Shader("wing.vs", "wing.fs");
		for (int i = 0; i < fireNum; i++) {
			fireMesh[i] = Mesh(firePath, true, false);
			//wingMesh[i] = Mesh(true);
			if (i == 0)
				fireMesh[i].setupTexture("flame.png");
			else
				fireMesh[i].texture = fireMesh[0].texture;
		}
		fireShader.use();
		fireShader.setInt("texture0", 0);

		//glm::vec4 startColor ;
		for (int i = 0; i < fireNum; i++) {
			glm::vec4 startColor;
			if (onFire[i])  startColor = glm::vec4(0.7569f, 0.2471f, 0.1176f, 1.0f);
			else startColor = glm::vec4(0.1176f, 0.2471f, 0.7569f, 1.0f);
			firePG[i] = ParticleGenerator(fireShader, fireMesh[i], 500, 500, firePos[i], 15, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
				startColor, glm::vec4(0.1, 0.1, 0.1, 0.0),
				glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), 1,
				0.0, 2.0,
				0.0, 0.0, 0.0, glm::vec3(0.0, 1.0, 0.0),
				glm::vec3(0.0, 1.5, 0.0), glm::vec3(1.0, 1.0, 1.0),
				0.01, 0.1, 0);
			//sizerange , size
		}
	}

	void Draw(glm::mat4 view, glm::mat4 projection, Light sun_moon, glm::vec3 cameraPos, glm::vec3 chaPos) {
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glActiveTexture(GL_TEXTURE0);
		for (int i = 0; i < fireNum; i++) {
			glBindTexture(GL_TEXTURE_2D, fireMesh[i].texture);
				firePG[i].DrawParticle(view, projection, sun_moon.lightPos, cameraPos);
				firePG[i].Update(0.1);
				if (onFire[i] == 1) {
					fireTime[i] += 1;
					if (fireTime[i] > 500) {
						fireTime[i] = 0;
						onFire[i] = 0;
						firePG[i].UpdateColor(glm::vec4(0.1176f, 0.2471f, 0.7569f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
					}
				}
				if (onFire[i] == 0 && distance(chaPos, firePos[i]) < 0.5) {
						//PlaySound(_T("wind.mp3"), NULL, SND_ASYNC | SND_FILENAME);
						Explode(i);
						onFire[i] = true;
						onFire[i] = 2;
				}
				if (firePG[i].isStopped()) {
					onFire[i] = 1;
					firePG[i].UpdateColor(glm::vec4(0.7569f, 0.2471f, 0.1176f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
					firePG[i].UpdateVelocity(glm::vec3(0.0, 1.5, 0.0), glm::vec3(1.0, 1.0, 1.0));
					firePG[i].UpdateLife(0);
					firePG[i].UpdateGrow(15);
				}
		}
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

	void setShader(Shader target) {
		char varName[40];
		for (int i = 0; i < fireNum; i++) {
			sprintf(varName, "lights[%d].position", i);
			target.setVec3(varName,firePos[i] + glm::vec3(myrand(), myrand(), myrand()));
			sprintf(varName, "lights[%d].lightColor", i);
			if(onFire[i] == 1)
				target.setVec3(varName, glm::vec3(0.7569f, 0.2471f, 0.1176f));
			else
				target.setVec3(varName, glm::vec3(0.1176f, 0.2471f, 0.7569f));
			if (onFire[i] != 2) {
				sprintf(varName, "lights[%d].constant", i);
				target.setFloat(varName, 5.0);
				sprintf(varName, "lights[%d].linear", i);
				target.setFloat(varName, 0.49);
				sprintf(varName, "lights[%d].quadratic", i);
				target.setFloat(varName, 0.22);
			}
			else {
				sprintf(varName, "lights[%d].constant", i);
				target.setFloat(varName, 3.0);
				sprintf(varName, "lights[%d].linear", i);
				target.setFloat(varName, 0.3);
				sprintf(varName, "lights[%d].quadratic", i);
				target.setFloat(varName, 0.1);
			}
			sprintf(varName, "lights[%d].ambient", i);
			target.setVec3(varName, glm::vec3(0.05));
			sprintf(varName, "lights[%d].diffuse", i);
			target.setVec3(varName, glm::vec3(0.8));
			sprintf(varName, "lights[%d].specular", i);
			target.setVec3(varName, glm::vec3(1.0));

		}
	}

private:
	int fireNum = 0;
	glm::vec3* firePos;
	int* onFire;
	ParticleGenerator* firePG;
	int* fireTime;

	void Explode(int i) {
		firePG[i].UpdateVelocity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(15.0, 15.0, 15.0));
		firePG[i].Explode();
	}

	float myrand() {
		static double V1, V2, S;
		static int phase = 0;
		float X;

		if (phase == 0) {
			do {
				double U1 = (double)rand() / RAND_MAX;
				double U2 = (double)rand() / RAND_MAX;

				V1 = 2 * U1 - 1;
				V2 = 2 * U2 - 1;
				S = V1 * V1 + V2 * V2;
			} while (S >= 1 || S == 0);

			X = V1 * sqrt(-2 * log(S) / S);
		}
		else
			X = V2 * sqrt(-2 * log(S) / S);

		phase = 1 - phase;

		return X * 0.1 + 0.4;

	}
};