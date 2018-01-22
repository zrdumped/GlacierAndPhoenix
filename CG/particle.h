#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "mesh.h"

//extern glm::vec3 wind[10][10][10];
extern glm::vec3 wind;


double gaussrand()
{
	static double V1, V2, S;
	static int phase = 0;
	double X;

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

	return X;
}

// Represents a single particle and its state
struct Particle {
	glm::mat4 model;
	glm::vec3 Position;
	glm::vec3 Velocity, Accleration;
	//glm::mat4 model;
	glm::vec4 Color;
	GLfloat Life;

	Particle() : model(0.0f), Velocity(0.0f), Position(0.0f), Accleration(0.0f), Color(1.0f), Life(0.0f) { }
	//Particle(glm::vec3 p) : Position(p), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


// ParticleGenerator acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time.
class ParticleGenerator
{
public:
	ParticleGenerator() {}
	// Constructor
	ParticleGenerator(Shader newShader, Mesh newMesh, GLuint totamount, GLuint initamount, glm::vec3 position, float growPerUpdate, glm::vec3 newPosrange,
		glm::vec4 newStartColor, glm::vec4 newStartcolorrange,
		glm::vec4 newEndColor, float newShininess,
		float newAvglife, float newLiferange,
		float newAngle, float newAnglerange, float newRotaterange, glm::vec3 newAxis,
		glm::vec3 newVelocity, glm::vec3 newVelocityrange,
		float newSizerange, float newScale)
	{
		grow = growPerUpdate;
		shader = newShader;
		mesh = newMesh;
		amount = totamount;
		liveamount = lastUsedParticle = initamount;
		Position = position;
		posRange = newPosrange;
		startColor = newStartColor;
		startColorRange = newStartcolorrange;
		endColor = newEndColor;
		shininess = newShininess;
		avgLife = newAvglife;
		lifeRange = newLiferange;
		angle = newAngle;
		angleRange = newAnglerange;
		rotateRange = newRotaterange;
		axis = newAxis;
		velocity = newVelocity;
		vRange = newVelocityrange;
		sRange = newSizerange;
		scale = newScale;

		updateTime = 0;
		regularArea = true;
		this->init(initamount);
	}

	ParticleGenerator(Shader newShader, Mesh newMesh, GLuint totamount, GLuint initamount, glm::vec3 position, float growPerUpdate, glm::vec3 newGenWidth, glm::vec3 newGenDepth,
		glm::vec4 newStartColor, glm::vec4 newStartcolorrange,
		glm::vec4 newEndColor, float newShininess,
		float newAvglife, float newLiferange,
		float newAngle, float newAnglerange, float newRotaterange, glm::vec3 newAxis,
		glm::vec3 newVelocity, glm::vec3 newVelocityrange,
		float newSizerange, float newSize, float newTextureRadius, float newGrowAccelerate = 0)
	{
		shader = newShader;
		mesh = newMesh;
		amount = totamount;
		liveamount = lastUsedParticle = initamount;
		Position = position;
		genWidth = newGenWidth;
		genDepth = newGenDepth;
		startColor = newStartColor;
		startColorRange = newStartcolorrange;
		endColor = newEndColor;
		shininess = newShininess;
		avgLife = newAvglife;
		lifeRange = newLiferange;
		angle = newAngle;
		angleRange = newAnglerange;
		rotateRange = newRotaterange;
		axis = newAxis;
		velocity = newVelocity;
		vRange = newVelocityrange;
		sRange = newSizerange;
		size = newSize;
		textureRadius = newTextureRadius;
		growAccelerate = newGrowAccelerate;
		if (growAccelerate > 0) {
			grow = 0;
			growMax = growPerUpdate;
		}
		else grow = growMax = growPerUpdate;
		updateTime = 0;
		regularArea = false;
		textureRadius = textureRadius;
		this->init(initamount);
		posRange = genWidth + genDepth;
	}

	// Update all particles
	void Update(GLfloat dt) {
		if (growAccelerate > 0 && grow < growMax) grow += growAccelerate;
		// Add new particles
		unsigned int g = grow;
		if (g > 0) {
			unsigned int  i;
			unsigned int  unusedParticle = lastUsedParticle;
			for (i = unusedParticle; i < amount; ++i)
			{
				if (particles[i].Life <= 0.0f) {
					this->respawnParticle(this->particles[i]);
					g--;
				}
				if (g <= 0)break;
			}
			if (g <= 0)lastUsedParticle = i;
			else {
				for (i = 0; i < unusedParticle; ++i)
				{
					if (particles[i].Life <= 0.0f) {
						this->respawnParticle(this->particles[i]);
						g--;
					}
					if (g <= 0)break;
				}
				lastUsedParticle = i;
			}
		}
		// Update all particles
		// glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glm::mat4 *m = (glm::mat4 *)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
		vector<int> mc; int j = 0;
		for (unsigned int i = 0; i < this->amount; ++i)
		{
			Particle &p = particles[i];
			// reduce life
			if (p.Life > 0.0f)
			{    // particle is alive, thus update
				p.Position += p.Velocity * dt;
				//cout << p.Position.x << " " << p.Position.y << " " << p.Position.z << " " << endl;
				p.model = glm::translate(p.model, p.Velocity*dt);
				float pp = myrand()*rotateRange*(((rand() % 3) - 1));
				p.model = glm::rotate(p.model, glm::radians(pp), axis);
				m[j] = p.model;
				j++; mc.push_back(i);
				p.Color += ((endColor - p.Color) / p.Life * dt);
				p.Accleration = wind;
				p.Velocity += p.Accleration * dt;
				/*if(i==0)
					 std::cout<<p.Velocity.x<<","<<p.Velocity.y<<","<<p.Velocity.z<<std::endl;*/
				p.Life -= dt;
				//p.Color.a -= dt * 1;
			}
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glm::vec4 *m2 = (glm::vec4 *)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
		for (int k = 0; k < j; k++) {
			m2[k] = particles[mc[k]].Color;
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		liveamount = j;

	}

	void Explode() {
		grow = 0;
		avgLife = 6;
		randMode = 1;
		for (int i = 0; i < amount; ++i) {
				this->respawnParticle(this->particles[i]);
		}
		//cout << "hi" << endl;
	}

	void UpdateGrow(int newGrow) {
		grow = newGrow;
		randMode = 0;
	}

	bool isStopped() {
		for (int i = 0; i < amount; ++i) {
			if (particles[i].Life > 0.0f) return false;
		}
		return true;
	}

	void UpdateGenPos(glm::vec3 genpos) {
		Position = genpos;
	}

	void UpdateGenPos(glm::vec3 genpos, glm::vec3 width, glm::vec3 depth) {
		Position = genpos;
		genWidth = width;
		genDepth = depth;
	}

	void UpdateVelocity(glm::vec3 newV, glm::vec3 newVRange) {
		velocity = newV;
		vRange = newVRange;
	}

	void UpdateColor(glm::vec4 sColor, glm::vec4 eColor) {
		startColor = sColor;
		endColor = eColor;
	}

	void UpdateLife(int newLife) {
		avgLife = newLife;
	}

	// Render all particles
	void DrawParticle(glm::mat4 view, glm::mat4 projection, glm::vec3 lightpos, glm::vec3 viewPos) {
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		this->shader.use();
		this->shader.setMat4("projection", projection);
		this->shader.setMat4("view", view);
		this->shader.setVec3("lightPos", lightpos);
		this->shader.setVec3("viewPos", viewPos);
		this->shader.setFloat("shininess", shininess);
		this->shader.setFloat("scale", scale);

		//fire.setShader(this->shader);
		//c.setShader(this->shader);

		//if (!regularArea) {
		//	this->shader.setFloat("textureRadius", textureRadius);
			//this->shader.setFloat("colorPort", p.Life / );
			//this->shader.setVec4("startColor", startColor);
			//this->shader.setVec4("endColor", endColor);
		//}

		//this->shader.setVec4("particlecolor", particles[0].Color);
		glBindVertexArray(mesh.VAO);
		if(mesh.vertices.size() != 0)
			glDrawArraysInstanced(GL_TRIANGLES, 0, mesh.vertices.size(), liveamount);
		else 
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, liveamount);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);

	}


private:
	// State
	std::vector<Particle> particles;
	unsigned int amount;
	float grow;
	float growMax;
	float growAccelerate;
	// Render state
	//glm::mat4* modelMatrices;
	unsigned int liveamount;
	unsigned int lastUsedParticle;
	Shader shader;
	Mesh mesh;
	unsigned int  VAO, VBO, VBO2;
	glm::vec3 Position;
	glm::vec3 posRange;
	glm::vec3 genWidth;
	glm::vec3 genDepth;
	glm::vec4 startColor;
	glm::vec4 startColorRange;
	glm::vec4 endColor;
	float shininess;
	float avgLife;
	float lifeRange;
	float angle;
	float angleRange;
	float rotateRange;
	float size;
	glm::vec3 axis;
	glm::vec3 velocity;
	glm::vec3 vRange;
	float sRange;
	float scale;
	int mode;
	unsigned int updateTime;
	float textureRadius;
	bool regularArea = true;
	int randMode = 0;
	// Initializes buffer and vertex attributes
	void init(GLuint aamount) {
		// Set up mesh and attribute properties
		glm::mat4 *modelMatrices = new glm::mat4[amount];
		glm::vec4 *colorMatrices = new glm::vec4[amount];
		Particle p = Particle();
		for (GLuint i = 0; i < amount; ++i) {
			//GLfloat random = ((rand() % 50) - 25) / 10.0f;
			if (regularArea || textureRadius == 0)
				p.Position = glm::vec3(Position.x + myrand()*posRange.x, Position.y + myrand()*posRange.y, Position.z + myrand()*posRange.z);
			else {
				float width = myrand(), depth = myrand();
				p.Position = glm::vec3(Position.x + width * genWidth.x + depth * genDepth.x,
					Position.y + width * genWidth.y + depth * genDepth.y,
					Position.z + width * genWidth.z + depth * genDepth.z);
			}


			p.model = glm::mat4(1.0);
			p.model = glm::translate(p.model, p.Position);
			//if (i == 0)cout << size.x << endl;
			float pp = angle + myrand()*angleRange;
			p.model = glm::rotate(p.model, glm::radians(pp), axis);
			if (regularArea) {
				float sp = 1 + myrand() * (2.0 * sRange) - sRange;
				p.model = glm::scale(p.model, glm::vec3(sp));
			}
			else {
				float sp = size + myrand() * sRange;
				p.model = glm::scale(p.model, glm::vec3(sp));
			}
			if (regularArea)
				p.Velocity = glm::vec3(velocity.x + myrand()*vRange.x, velocity.y + myrand()*vRange.y, velocity.z + myrand()*vRange.z);
			else
				p.Velocity = glm::vec3(velocity.x + (myrand() - 0.5)*vRange.x, velocity.y + (myrand() - 0.5)*vRange.y, velocity.z + (myrand() - 0.5)*vRange.z);
			p.Color = glm::vec4(startColor.x + myrand()*startColorRange.x,
				startColor.y + myrand()*startColorRange.y,
				startColor.z + myrand()*startColorRange.z,
				startColor.w + myrand()*startColorRange.w);

			particles.push_back(p);

		}
		for (GLuint i = 0; i < aamount; ++i) {
			particles[i].Life = avgLife + avgLife + myrand()*lifeRange;
			modelMatrices[i] = p.model;
			colorMatrices[i] = p.Color;
		}
		lastUsedParticle = liveamount;
		//unsigned int buffer;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_DYNAMIC_DRAW);
		glBindVertexArray(mesh.VAO);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glGenBuffers(1, &VBO2);
		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::vec4), &colorMatrices[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
		glVertexAttribDivisor(7, 1);
		glBindVertexArray(0);
		if (colorMatrices)free(colorMatrices);
		if (modelMatrices)free(modelMatrices);

	
	}
	// Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
	GLuint firstUnusedParticle() {
		// First search from last used particle, this will usually return almost instantly
		for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
			if (this->particles[i].Life <= 0.0f) {
				lastUsedParticle = i;
				return i;
			}
		}
		// Otherwise, do a linear search
		for (GLuint i = 0; i < lastUsedParticle; ++i) {
			if (this->particles[i].Life <= 0.0f) {
				lastUsedParticle = i;
				return i;
			}
		}
		// All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
		//lastUsedParticle = 0;
		return -1;
	}
	// Respawns particle
	void respawnParticle(Particle &particle) {
		if (regularArea || textureRadius == 0)
			particle.Position = glm::vec3(Position.x + myrand()*posRange.x, Position.y + myrand()*posRange.y, Position.z + myrand()*posRange.z);
		else {
			float width = myrand(), depth = myrand();
			particle.Position = glm::vec3(Position.x + width * genWidth.x + depth * genDepth.x,
				Position.y + width * genWidth.y + depth * genDepth.y,
				Position.z + width * genWidth.z + depth * genDepth.z);
		}
		particle.model = glm::mat4(1.0);
		particle.model = glm::translate(particle.model, particle.Position);
		float pp = angle + myrand()*angleRange;
		particle.model = glm::rotate(particle.model, glm::radians(pp), axis);
		if (regularArea) {
			float sp = 1 + myrand() * (2.0 * sRange) - sRange;
			particle.model = glm::scale(particle.model, glm::vec3(sp));
		}
		else {
			float sp = size + myrand() * sRange;
			particle.model = glm::scale(particle.model, glm::vec3(sp));
		}
		//particle.model = glm::translate(particle.model, glm::vec3(sp, sp, sp));
		particle.Color = glm::vec4(startColor.x + myrand()*startColorRange.x,
			startColor.y + myrand()*startColorRange.y,
			startColor.z + myrand()*startColorRange.z,
			startColor.w + myrand()*startColorRange.w);
		particle.Life = avgLife + ((rand() % 100) / 100.0)*lifeRange;
		if (regularArea)
			particle.Velocity = glm::vec3(velocity.x + myrand()*vRange.x, velocity.y + myrand()*vRange.y, velocity.z + myrand()*vRange.z);
		else
			particle.Velocity = glm::vec3(velocity.x + (myrand() - 0.5)*vRange.x, velocity.y + (myrand() - 0.5)*vRange.y, velocity.z + (myrand() - 0.5)*vRange.z);

	}

	float myrand()
	{

		//return gaussrand() + 0.5;
		if (randMode == 0)
			return (rand() % 100) / 100.0;
		else
			return gaussrand() * 0.1  + 0.5;
			//return (rand() % 100) / 100.0;
	}




};

