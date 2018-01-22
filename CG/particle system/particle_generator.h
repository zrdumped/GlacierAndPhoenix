/*******************************************************************
 ** This code is part of Breakout.
 **
 ** Breakout is free software: you can redistribute it and/or modify
 ** it under the terms of the CC BY 4.0 license as published by
 ** Creative Commons, either version 4 of the License, or (at your
 ** option) any later version.
 ******************************************************************/
#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "loader.h"


// Represents a single particle and its state
struct Particle {
    glm::mat4 model;
    //Position,
    glm::vec3 Velocity, Size;
    //glm::mat4 model;
    glm::vec4 Color;
    GLfloat Life;
    
    Particle() : model(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f), Size(1.0f) { }
    //Particle(glm::vec3 p) : Position(p), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


// ParticleGenerator acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time.
class ParticleGenerator
{
public:
    // Constructor
    ParticleGenerator(Shader shader, Mesh mesh, GLuint totamount, GLuint initamount, glm::vec3 position, GLuint growPerUpdate, glm::vec3 posrange,
                      glm::vec4 startcolor, glm::vec4 scrange,
                      glm::vec4 endcolor, float shininess,
                      float avglife, float liferange,
                      float angle, float anglerange, float rotaterange, glm::vec3 axis,
                      glm::vec3 velocity, glm::vec3 vrange,
                      glm::vec3 size, glm::vec3 srange);
    // Update all particles
    void Update(GLfloat dt);
    // Render all particles
    void Draw(glm::mat4 view, glm::mat4 projection, glm::vec3 lightpos, glm::vec3 viewPos);
private:
    // State
    std::vector<Particle> particles;
    GLuint amount;
    GLuint grow;
    // Render state
    //glm::mat4* modelMatrices;
    GLuint liveamount;
    GLuint lastUsedParticle;
    Shader shader;
    Mesh mesh;
    GLuint VAO;
    GLuint VBO;
    GLuint VBO2;
    glm::vec3 Position;
    glm::vec3 posRange;
    glm::vec4 startColor;
    glm::vec4 startColorRange;
    glm::vec4 endColor;
    float shininess;
    float avgLife;
    float lifeRange;
    float angle;
    float angleRange;
    float rotateRange;
    glm::vec3 axis;
    glm::vec3 velocity;
    glm::vec3 vRange;
    glm::vec3 size;
    glm::vec3 sRange;
    // Initializes buffer and vertex attributes
    void init(GLuint initamount);
    // Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    GLuint firstUnusedParticle();
    // Respawns particle
    void respawnParticle(Particle &particle);
    float myrand();
};

#endif
