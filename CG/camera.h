#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
/*enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};*/

// Default camera values
const float YAW        = -90.0f;
const float PITCH      =  0.0f;
//const float SPEED      =  2.5f;
const float SENSITIVTY =  0.1f;
const float ZOOM       =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
    public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Eular Angles
    float Yaw;
    float Pitch;
    // Camera options
    //float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    
    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

	Camera(Character c) : MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		Position = glm::vec3(c.GetPosition().x - c.GetDirection().x * 3.0f,c.GetPosition().y + 1.0f, c.GetPosition().z - c.GetDirection().z * 3.0f);
		WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		Front = glm::normalize(-Position + c.GetPosition() * 3.0f); //dst - src
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
		Yaw = -90.0f;
		//std::cout << Yaw;
		Pitch = -atan(1.0f / 3.0f) * 180.0f / Pi;
	}
    
    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }
    
    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
 /*   void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
        Position += Front * velocity;
        if (direction == BACKWARD)
        Position -= Front * velocity;
        if (direction == LEFT)
        Position -= Right * velocity;
        if (direction == RIGHT)
        Position += Right * velocity;
    }*/
    
    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, Character c, GLboolean constrainPitch = true)
    {
		//std::cout << Front.x << std::endl;
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;
        
        Yaw   += xoffset;
        Pitch += yoffset;


        
        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
            Pitch = 89.0f;
            if (Pitch < -89.0f)
            Pitch = -89.0f;
        }
        
        // Update Front, Right and Up Vectors using the updated Eular angles
		//Pitch = -atan(2.0f / 1.0f) * 180.0f / Pi;
		updateCameraVectors();
		float angle = atan(Front.z / Front.x);
		if (Front.x < 0) angle -= Pi;
		//else if (Direction.x < 0 && cameraDir.x > 0)angle += pi;
		float x = cos(angle);
		float z = sin(angle);
		Position = glm::vec3(c.GetPosition().x - x * 3.0f, c.GetPosition().y + 1.0f, c.GetPosition().z - z * 3.0f);
    }
    
    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
        Zoom -= yoffset;
        if (Zoom <= 1.0f)
        Zoom = 1.0f;
        if (Zoom >= 45.0f)
        Zoom = 45.0f;
    }

	void MoveCameraByCharacter(Character c) {
		//std::cout << c.GetDirection().x*c.GetDirection().x + c.GetDirection().z + c.GetDirection().z << std::endl;
		Position = glm::vec3(c.GetPosition().x - c.GetDirection().x * 3.0f, c.GetPosition().y + 1.0f, c.GetPosition().z - c.GetDirection().z * 3.0f);
		//double endAngle = atan((double)c.GetDirection().z / (double)c.GetDirection().x);
		//double startAngle = atan((double)Front.z / (double)Front.x);
		//std::cout << Xstart << " i " << Ystart << " j " << Xend << " x " << Yend << " y " << std::endl;
		//·ÀÖ¹180Ìø±ä
		//if (Front.x > 0 && c.GetDirection().x < 0) endAngle += pi;
		//else if (Front.x < 0 && c.GetDirection().x > 0)endAngle += pi;
		//Direction = cameraDir;
		//Yaw -= endAngle * 180.0f / Pi - startAngle * 180.0f / Pi;
		//updateCameraVectors();
		//std::cout << Yaw << std::endl;
		
		//Front = c.GetDirection();
		//WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		//Front = glm::normalize(-Position + c.GetPosition()); //dst - src
		//Right = glm::normalize(glm::cross(Front, WorldUp));
		//Up = glm::normalize(glm::cross(Right, Front));
		//Yaw = 0.0f;
		//std::cout << Yaw;
		//Pitch = -atan(2.0f / 1.0f) * 180.0f / Pi;
	}

	void RotateCameraByCharacter(Character c, bool isleft) {
		Position = glm::vec3(c.GetPosition().x - c.GetDirection().x * 3.0f, c.GetPosition().y + 1.0f, c.GetPosition().z - c.GetDirection().z * 3.0f);
		if(isleft)
			Yaw -= c.rotateAcce;
		else
			Yaw += c.rotateAcce;
		updateCameraVectors();
		Front = glm::vec3(c.GetDirection().x * sqrt(1 - Front.y * Front.y), Front.y, c.GetDirection().z * sqrt(1 - Front.y * Front.y));
		Right = glm::normalize(glm::cross(Front, glm::vec3(0.0f, 1.0f, 0.0f)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}

	glm::vec3 GetHorizDir() {
		return glm::vec3(Front.x* sqrt(1 - Front.y * Front.y), 0, Front.z* sqrt(1 - Front.y * Front.y));
	}
    
    private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif
