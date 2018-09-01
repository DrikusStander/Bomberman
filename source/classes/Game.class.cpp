/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnolte <cnolte@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 11:26:34 by cnolte            #+#    #+#             */
/*   Updated: 2018/08/13 17:26:06 by cnolte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.class.hpp"

Camera	camera;

bool	keys[1024];
bool	firstMouse;
GLfloat	lastX;
GLfloat	lastY;

// Is called whenever a key is pressed/released via GLFW
void	KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

void	MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat	xOffset = xPos - lastX;
	GLfloat	yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

//start canonical form
Game::Game(void) : s_WIDTH(100), s_HEIGHT(100)
{
	std::cout << "Game - Default Constructor Called" << std::endl;
}

Game::Game(const int width, const int height) : s_WIDTH(width), s_HEIGHT(height)
{
	std::cout << "Game - Parametric Constructor called" << std::endl;

	lastX = 400;
	lastY = 300;
	firstMouse = true;
	deltaTime = 0.0;
	lastFrame = 0.0;

	camera.ProcessMouseMovement(0, -250);

	// Init GLFW
	glfwInit();

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	// fullscreen
	// GLFWwindow	*window = glfwCreateWindow(this->s_WIDTH, this->s_HEIGHT, "Bomberman", glfwGetPrimaryMonitor(), nullptr);
	// windowed
	GLFWwindow	*window = glfwCreateWindow(this->s_WIDTH, this->s_HEIGHT, "Bomberman", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		throw Exceptions::CreateWindowFailed();
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
		throw Exceptions::InitializeGlewFailed();

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Setup and compile our shaders
	Shader	shader("resources/shaders/modelLoading.vert", "resources/shaders/modelLoading.frag");

	// Load models
	this->world = new World(shader, "resources/models/world.obj");
	
	// Item temp(shader, "resources/models/fire/fire.obj");
	// Item temp2(shader, "resources/models/fire/fire.obj");
	// temp2.setPos(168, 168, 0, 0);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		// std::cout << "time: " << currentFrame << std::endl;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();

		// set the camera view and projection
		glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
		world->draw(camera);
		int worldStatus = world->getStatus();
		if (worldStatus == 1)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		// temp.draw();
		// temp2.draw();
		glfwSwapBuffers(window);
	}
	delete this->world;
	glfwTerminate();
}

Game::Game(Game const & src)
{
	std::cout << "Game - Copy constructor called" << std::endl;
	*this = src;
}

Game::~Game(void)
{
	std::cout << "Game - Destructor called" << std::endl;
}

Game	&Game::operator=(Game const &rhs)
{
	std::cout << "Game - Assignment operator called" << std::endl;
	if (this != &rhs)
	{
		this->deltaTime = rhs.deltaTime;
		this->lastFrame = rhs.lastFrame;
		this->s_WIDTH = rhs.s_WIDTH;
		this->s_HEIGHT = rhs.s_HEIGHT;
		this->SCREEN_HEIGHT = rhs.SCREEN_HEIGHT;
		this->SCREEN_WIDTH = rhs.SCREEN_WIDTH;
		this->world = rhs.world;
	}
	return (*this);
}
//end canonical form

// Moves/alters the camera positions based on user input
void	Game::DoMovement(void)
{
	// Camera controls
	if (keys[GLFW_KEY_Q])
		camera.ProcessKeyboard(UP, deltaTime);
	else if (keys[GLFW_KEY_E])
		camera.ProcessKeyboard(DOWN, deltaTime);
	else if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	else if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	else if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	else if (keys[GLFW_KEY_D] )
		camera.ProcessKeyboard(RIGHT, deltaTime);

	// Player controls
	if (keys[GLFW_KEY_UP])
		this->world->ProcessKeyboard(FWD);
	else if (keys[GLFW_KEY_DOWN])
		this->world->ProcessKeyboard(BKW);
	else if (keys[GLFW_KEY_LEFT])
		this->world->ProcessKeyboard(LFT);
	else if (keys[GLFW_KEY_RIGHT])
		this->world->ProcessKeyboard(RGT);

	// Plant a bomb
	if (keys[GLFW_KEY_SPACE])
		this->world->ProcessKeyboard(SPC);
}
