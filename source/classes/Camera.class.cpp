/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnolte <cnolte@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 11:26:34 by cnolte            #+#    #+#             */
/*   Updated: 2018/09/01 17:20:35 by cnolte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.class.hpp"

//start canonical form
Camera::Camera(void)
{
	std::cout << "Camera - Default constructor called" << std::endl;
	this->movementSpeed = SPEED;
	this->mouseSensitivity = SENSITIVTY;
	this->zoom = ZOOM;
	this->position = glm::vec3(0.0f, 300.0f, 200.0f);
	this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->yaw = YAW;
	this->pitch = PITCH;
	this->updateCameraVectors( );
}

Camera::Camera(Camera const & src)
{
	std::cout << "Camera - Copy constructor called" << std::endl;
	*this = src;
}

Camera::~Camera(void)
{
	std::cout << "Camera - Destructor called" << std::endl;
}

Camera	&Camera::operator=(Camera const &rhs)
{
	std::cout << "Camera - Assignment operator called" << std::endl;
	if (this != &rhs)
	{
		this->position = rhs.position;
		this->front = rhs.front;
		this->up = rhs.up;
		this->right = rhs.right;
		this->worldUp = rhs.worldUp;
		this->yaw = rhs.yaw;
		this->pitch = rhs.pitch;
		this->movementSpeed = rhs.movementSpeed;
		this->mouseSensitivity = rhs.mouseSensitivity;
		this->zoom = rhs.zoom;
	}
	return (*this);
}
//end canonical form

// Constructor with scalar values
Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
{
	this->position = glm::vec3(posX, posY, posZ);
	this->worldUp = glm::vec3(upX, upY, upZ);
	this->yaw = yaw;
	this->pitch = pitch;
	this->updateCameraVectors();
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4	Camera::GetViewMatrix()
{
	return (glm::lookAt(this->position, this->position + this->front, this->up));
}

glm::vec3	Camera::getPossition(void)
{
	return (this->position);
}

glm::vec3	Camera::getFront(void)
{
	return (this->front);
}

glm::vec3	Camera::getUp(void)
{
	return (this->up);
}

GLfloat		Camera::getYaw(void)
{
	return (this->yaw);
}

GLfloat		Camera::getPitch(void)
{
	return (this->pitch);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void	Camera::ProcessKeyboard( Camera_Movement direction, GLfloat deltaTime )
{
	GLfloat	velocity = this->movementSpeed * deltaTime;
	
	if (direction == UP)
	{
		this->position += this->front * velocity;
	}

	if (direction == DOWN)
	{
		this->position -= this->front * velocity;
	}

	if (direction == FORWARD)
	{
		this->position += this->up * velocity;
	}

	if (direction == BACKWARD)
	{
		this->position -= this->up * velocity;
	}

	if (direction == LEFT)
	{
		this->position -= this->right * velocity;
	}
	
	if (direction == RIGHT)
	{
		this->position += this->right * velocity;
	}
}

GLfloat	Camera::GetZoom(void)
{
	return (this->zoom);
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void	Camera::ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset)
{
	GLboolean constrainPitch = true;
	xOffset *= this->mouseSensitivity;
	yOffset *= this->mouseSensitivity;

	this->yaw += xOffset;
	this->pitch += yOffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (this->pitch > 89.0f)
		{
			this->pitch = 89.0f;
		}

		if (this->pitch < -89.0f)
		{
			this->pitch = -89.0f;
		}
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	this->updateCameraVectors( );
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::updateCameraVectors(void)
{
	// Calculate the new Front vector
	glm::vec3	front;

	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->up = glm::normalize(glm::cross(this->right, this->front));
}
