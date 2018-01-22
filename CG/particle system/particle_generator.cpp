#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "particle_generator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Mesh mesh, GLuint totamount, GLuint initamount, glm::vec3 position, GLuint growPerUpdate, glm::vec3 posrange,
                                     glm::vec4 startcolor, glm::vec4 scrange,
                                     glm::vec4 endcolor, float shininess,
                                     float avglife, float liferange,
                                     float angle, float anglerange, float rotaterange, glm::vec3 axis,
                                     glm::vec3 velocity, glm::vec3 vrange,
                                     glm::vec3 size, glm::vec3 srange)
: shader(shader),mesh(mesh),amount(totamount),liveamount(initamount),lastUsedParticle(initamount),
    Position(position),grow(growPerUpdate),posRange(posrange),
    startColor(startcolor), startColorRange(scrange),
    endColor(endcolor), shininess(shininess), 
    avgLife(avglife), lifeRange(liferange),angle(angle), angleRange(anglerange), rotateRange(rotaterange), axis(axis),
    velocity(velocity), vRange(vrange),
    size(size), sRange(srange)
{
    
    this->init(initamount);
}

void ParticleGenerator::Update(GLfloat dt)
{
    // Add new particles
    GLint g = grow;
    GLint i;
    GLuint unusedParticle = lastUsedParticle;
    for (i = unusedParticle; i < amount; ++i)
    {
        if(particles[i].Life<=0.0f){
            this->respawnParticle(this->particles[i]);
            g--;
        }
        if(g<=0)break;
    }
    if(g<=0)lastUsedParticle = i;
    else {
        for (i = 0; i < unusedParticle; ++i)
        {
            if(particles[i].Life<=0.0f){
                this->respawnParticle(this->particles[i]);
                g--;
            }
            if(g<=0)break;
        }
        lastUsedParticle = i;
    }
    // Update all particles
   // glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glm::mat4 *m =  (glm::mat4 *)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    vector<int> mc;int j=0;
    for (GLuint i = 0; i < this->amount; ++i)
    {
        Particle &p = particles[i];
        // reduce life
        if (p.Life > 0.0f)
        {    // particle is alive, thus update
            //p.Position += p.Velocity * dt;
            p.model = glm::translate(p.model, p.Velocity*dt);
            float pp = myrand()*rotateRange*(((rand()%3)-1));
            p.model = glm::rotate(p.model, glm::radians(pp), axis);
            m[j] = p.model;
            j++;mc.push_back(i);
            p.Color += ((endColor - p.Color)/p.Life * dt);
            if(rand()%2)
            p.Velocity += glm::vec3(myrand()*vRange.x, 0,myrand()*vRange.z);
            else p.Velocity -= glm::vec3(myrand()*vRange.x, 0,myrand()*vRange.z);
            /*if(i==0)
                std::cout<<p.Velocity.x<<","<<p.Velocity.y<<","<<p.Velocity.z<<std::endl;*/
            p.Life -= dt;
            //p.Color.a -= dt * 1;
        }
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glm::vec4 *m2 =  (glm::vec4 *)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    for(int k=0;k<j;k++){
        m2[k] = particles[mc[k]].Color;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    liveamount = j;
    
}

// Render all particles
void ParticleGenerator::Draw(glm::mat4 view, glm::mat4 projection, glm::vec3 lightpos, glm::vec3 viewPos)
{
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.Use();
    this->shader.setMat4("projection", projection);
    this->shader.setMat4("view", view);
    this->shader.setVec3("lightPos",lightpos);
    this->shader.setVec3("viewPos", viewPos);
    this->shader.setFloat("shininess", shininess);
    /*or (Particle particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            this->shader.setMat4("model", particle.model);
            this->shader.setVec4("particlecolor", particle.Color);
           //this->texture.Bind();
            //std::cout<<"ptps"<<particle.Position.x<<","<<particle.Position.y<<","<<particle.Position.z<<std::endl;
           // glm::mat4 model(1.0f);
           // model = glm::mat4(1.0f);
            //model = glm::translate(model, particle.Position);
            //model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
            //this->shader.setMat4("model", model);
            //glBindVertexArray(this->VAO);
            
            //glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(mesh.VAO);
            glDrawArrays(GL_TRIANGLES, 0 ,mesh.indices.size()*3);
            // always good practice to set everything back to defaults once configured.
            //glActiveTexture(GL_TEXTURE0);
            //glBindVertexArray(0);
        }
    }*/

    //this->shader.setVec4("particlecolor", particles[0].Color);
    glBindVertexArray(mesh.VAO);
    //glDrawArraysInstanced(GL_TRIANGLES, mesh.indices.size()*3, GL_UNSIGNED_INT, 0, amount);
    glDrawArraysInstanced(GL_TRIANGLES, 0, mesh.indices.size()*3, liveamount);
    glBindVertexArray(0);
    
    glActiveTexture(GL_TEXTURE0);
    
}

void ParticleGenerator::init(GLuint aamount)
{
    // Set up mesh and attribute properties
    
    /*GLfloat particle_quad[] = {
        -0.01f, -0.01f, -0.01f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
        0.01f, -0.01f, -0.01f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
        0.01f,  0.01f, -0.01f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        0.01f,  0.01f, -0.01f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -0.01f,  0.01f, -0.01f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -0.01f, -0.01f, -0.01f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
        
        -0.01f, -0.01f,  0.01f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
        0.01f, -0.01f,  0.01f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
        0.01f,  0.01f,  0.01f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        0.01f,  0.01f,  0.01f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -0.01f,  0.01f,  0.01f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -0.01f, -0.01f,  0.01f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
        
        -0.01f,  0.01f,  0.01f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.01f,  0.01f, -0.01f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.01f, -0.01f, -0.01f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.01f, -0.01f, -0.01f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.01f, -0.01f,  0.01f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.01f,  0.01f,  0.01f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        
        0.01f,  0.01f,  0.01f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
        0.01f,  0.01f, -0.01f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
        0.01f, -0.01f, -0.01f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
        0.01f, -0.01f, -0.01f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
        0.01f, -0.01f,  0.01f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
        0.01f,  0.01f,  0.01f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
        
        -0.01f, -0.01f, -0.01f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
        0.01f, -0.01f, -0.01f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
        0.01f, -0.01f,  0.01f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        0.01f, -0.01f,  0.01f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -0.01f, -0.01f,  0.01f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -0.01f, -0.01f, -0.01f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
        
        -0.01f,  0.01f, -0.01f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
        0.01f,  0.01f, -0.01f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
        0.01f,  0.01f,  0.01f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        0.01f,  0.01f,  0.01f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.01f,  0.01f,  0.01f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.01f,  0.01f, -0.01f,  0.0f, 1.0f,   0.0f,  1.0f,  0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // Set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
    // Create initial amount default particle instances*/
    glm::mat4 *modelMatrices = new glm::mat4[amount];
    glm::vec4 *colorMatrices = new glm::vec4[amount];
    Particle p = Particle();
    for (GLuint i = 0; i < amount; ++i){
        //GLfloat random = ((rand() % 50) - 25) / 10.0f;
        //p.Position = glm::vec3(Position.x+((rand() % 100)/100.0)*posRange.x, Position.y+((rand() % 100)/100.0)*posRange.y , Position.z+((rand() % 100)/100.0)*posRange.z);
        
        p.model = glm::mat4(1.0);
        p.model = glm::translate(p.model, glm::vec3(Position.x+myrand()*posRange.x, Position.y+myrand()*posRange.y , Position.z+myrand()*posRange.z));
        float pp = angle+myrand()*angleRange;
        p.model = glm::rotate(p.model, glm::radians(pp), axis);
       
        p.Velocity = glm::vec3(velocity.x+myrand()*vRange.x, velocity.y+myrand()*vRange.y,velocity.z+myrand()*vRange.z);
        p.Color = glm::vec4(startColor.x + myrand()*startColorRange.x,
                            startColor.y + myrand()*startColorRange.y,
                            startColor.z + myrand()*startColorRange.z,
                            startColor.w + myrand()*startColorRange.w);
        
        p.Size = glm::vec3(size.x + myrand()*sRange.x,
                           size.y + myrand()*sRange.y,
                           size.z + myrand()*sRange.z);
        particles.push_back(p);
        
        //std::cout<<"initptps"<<particles[i].Position.x<<","<<particles[i].Position.y<<","<<particles[i].Position.z<<std::endl;
    }
    for (GLuint i = 0; i < aamount; ++i){
        p.Life = avgLife+myrand()*lifeRange;
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
    if(colorMatrices)free(colorMatrices);
    if(modelMatrices)free(modelMatrices);
    
    /*for (GLuint i = 0; i < aamount; ++i){
        p.model = glm::mat4(1.0);
        p.model = glm::translate(p.model, glm::vec3(Position.x+((rand() % 100)/100.0)*posRange.x, Position.y+((rand() % 100)/100.0)*posRange.y , Position.z+((rand() % 100)/100.0)*posRange.z));
        float pp = angle+((rand() % 100)/100.0)*angleRange;
        p.model = glm::rotate(p.model, glm::radians(pp), axis);
        p.Velocity = glm::vec3(velocity.x+((rand() % 100)/100.0)*vRange.x, velocity.y+((rand() % 100)/100.0)*vRange.y,velocity.z+((rand() % 100)/100.0)*vRange.z);
        p.Life = avgLife+avgLife+((rand() % 100)/100.0)*lifeRange;
        p.Color = glm::vec4(startColor.x + ((rand() % 100)/100.0)*startColorRange.x,
                            startColor.y + ((rand() % 100)/100.0)*startColorRange.y,
                            startColor.z + ((rand() % 100)/100.0)*startColorRange.z,
                            startColor.w + ((rand() % 100)/100.0)*startColorRange.w);
        p.Size = glm::vec3(size.x + ((rand() % 100)/100.0)*sRange.x,
                           size.y + ((rand() % 100)/100.0)*sRange.y,
                           size.z + ((rand() % 100)/100.0)*sRange.z);
        particles.push_back(p);
        //std::cout<<"initptps"<<particles[i].Position.x<<","<<particles[i].Position.y<<","<<particles[i].Position.z<<std::endl;
    }
    p.Life = 0;
    for (GLuint i = aamount; i < amount; ++i){
        //GLfloat random = ((rand() % 50) - 25) / 10.0f;
        //p.Position = glm::vec3(Position.x+((rand() % 100)/100.0)*posRange.x, Position.y+((rand() % 100)/100.0)*posRange.y , Position.z+((rand() % 100)/100.0)*posRange.z);
        p.model = glm::mat4(1.0);
        p.model = glm::translate(p.model, glm::vec3(Position.x+((rand() % 100)/100.0)*posRange.x, Position.y+((rand() % 100)/100.0)*posRange.y , Position.z+((rand() % 100)/100.0)*posRange.z));
        float pp = angle+((rand() % 100)/100.0)*angleRange;
        p.model = glm::rotate(p.model, glm::radians(pp), axis);
        p.Velocity = glm::vec3(velocity.x+((rand() % 100)/100.0)*vRange.x, velocity.y+((rand() % 100)/100.0)*vRange.y,velocity.z+((rand() % 100)/100.0)*vRange.z);
        p.Color = glm::vec4(startColor.x + ((rand() % 100)/100.0)*startColorRange.x,
                            startColor.y + ((rand() % 100)/100.0)*startColorRange.y,
                            startColor.z + ((rand() % 100)/100.0)*startColorRange.z,
                            startColor.w + ((rand() % 100)/100.0)*startColorRange.w);
        p.Size = glm::vec3(size.x + ((rand() % 100)/100.0)*sRange.x,
                           size.y + ((rand() % 100)/100.0)*sRange.y,
                           size.z + ((rand() % 100)/100.0)*sRange.z);
        particles.push_back(p);
        //std::cout<<"initptps"<<particles[i].Position.x<<","<<particles[i].Position.y<<","<<particles[i].Position.z<<std::endl;
    }*/
}

// Stores the index of the last particle used (for quick access to next dead particle)

GLuint ParticleGenerator::firstUnusedParticle()
{
    // First search from last used particle, this will usually return almost instantly
    for (GLuint i = lastUsedParticle; i < this->amount; ++i){
        if (this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for (GLuint i = 0; i < lastUsedParticle; ++i){
        if (this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    //lastUsedParticle = 0;
    return -1;
}

void ParticleGenerator::respawnParticle(Particle &particle)
{
    //particle.Position = glm::vec3(Position.x+((rand() % 100)/100.0)*posRange.x, Position.y+((rand() % 100)/100.0)*posRange.y , Position.z+((rand() % 100)/100.0)*posRange.z);
    particle.model = glm::mat4(1.0);
    particle.model = glm::translate(particle.model, glm::vec3(Position.x+myrand()*posRange.x, Position.y+myrand()*posRange.y , Position.z+myrand()*posRange.z));
    float pp = angle+myrand()*angleRange;
    particle.model = glm::rotate(particle.model, glm::radians(pp), axis);
    particle.Color = glm::vec4(startColor.x + myrand()*startColorRange.x,
                               startColor.y + myrand()*startColorRange.y,
                               startColor.z + myrand()*startColorRange.z,
                               startColor.w + myrand()*startColorRange.w);
    particle.Life = avgLife+((rand() % 100)/100.0)*lifeRange;
    particle.Velocity = glm::vec3(velocity.x+myrand()*vRange.x, velocity.y+myrand()*vRange.y,velocity.z+myrand()*vRange.z);
}

float ParticleGenerator::myrand()
{
    //your code here
    return (rand() % 100)/100.0;
}
