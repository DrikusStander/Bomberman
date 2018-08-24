/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnolte <cnolte@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 11:20:47 by cnolte            #+#    #+#             */
/*   Updated: 2018/08/24 12:34:03 by cnolte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_CLASS_HPP
# define CAMERA_CLASS_HPP

// GL Includes
# include <GL/glew.h>

# include <iostream>

// Matrices includes
# include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const GLfloat	YAW = -90.0f;
const GLfloat	PITCH = 0.0f;
const GLfloat	SPEED = 60.0f;
const GLfloat	SENSITIVTY = 0.25f;
const GLfloat	ZOOM = 45.0f;

class Camera
{
	public:
		//start canonical form
		Camera(void);
		Camera(Camera const & src);
		~Camera(void);

		Camera	&operator=(Camera const &rhs);
		//end canonical form

		// Constructor with scalar values
		Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

		// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
		glm::mat4	GetViewMatrix(void);

		GLfloat		GetZoom(void);

		// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
		void		ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

		// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		void		ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset);

		// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
		void		ProcessMouseScroll(GLfloat yOffset);

	private:
		 // Camera Attributes
		glm::vec3	position;
		glm::vec3	front;
		glm::vec3	up;
		glm::vec3	right;
		glm::vec3	worldUp;
		
		// Eular Angles
		GLfloat		yaw;
		GLfloat		pitch;
		
		// Camera options
		GLfloat		movementSpeed;
		GLfloat		mouseSensitivity;
		GLfloat		zoom;

		// Calculates the front vector from the Camera's (updated) Eular Angles
		void		updateCameraVectors(void);
};

#endif
