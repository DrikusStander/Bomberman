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
Game::Game(void) : screen_x(100), screen_y(100)
{
	std::cout << "Game - Default Constructor Called" << std::endl;
}

Game::Game(const int width, const int height) : screen_x(width), screen_y(height)
{
	std::cout << "Game - Parametric Constructor called" << std::endl;

	lastX = 400;
	lastY = 300;
	firstMouse = true;
	deltaTime = 0.0;
	lastFrame = 0.0;
	menuVisible = true;
	loadVisible = false;
	this->WorldLoaded = false;

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
	// GLFWwindow	*window = glfwCreateWindow(this->screen_x, this->screen_y, "Bomberman", glfwGetPrimaryMonitor(), nullptr);
	// windowed
	GLFWwindow	*window = glfwCreateWindow(this->screen_x, this->screen_y, "Bomberman", nullptr, nullptr);
	this->window = window;
	if (window == nullptr)
	{
		glfwTerminate();
		throw Exceptions::CreateWindowFailed();
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	// std::cout << "x: " << SCREEN_WIDTH << " y: " << SCREEN_HEIGHT << std::endl;
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

	this->shader = &shader;
	// Load models
	// this->world = new World(shader, "resources/models/world.obj");
	for (int i = 0; i < 6; i++)
	{
		this->Menus.push_back(new MainMenu(shader, "resources/models/menu/Menu_" + std::to_string(i) + ".obj"));
	}

	Item temp(shader, "resources/models/portal/portal.obj");
	for (int i = 0; i < 3; i++)
	{
		this->load.push_back(new LoadingScreen(shader, "resources/models/menu/Loading_screen_" + std::to_string(i) + ".obj"));
	}
	this->loadActive = 0;

	// Item temp(shader, "resources/models/fire/fire.obj");
	// Item temp2(shader, "resources/models/fire/fire.obj");
	temp.setPos(168, 168, 0, 0);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
	this->projection = projection;

	this->menuActive = 0;
	GLfloat old_time = 0.0f;
	GLfloat old_time_key = 0.0f;
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		mu.lock();
		glfwMakeContextCurrent(window);

		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		// std::cout << "time: " << currentFrame << std::endl;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		if (currentFrame - old_time >= 1.0f)
		{
			old_time = currentFrame;
			std::cout << "FPS: " << std::to_string(1.0f / deltaTime) << std::endl;
		}
		
		// Check and call events
		glfwPollEvents();

		// Clear the colorbuffer
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();

		// set the camera view and projection
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
		if (menuVisible == true)
		{
			Menus[this->menuActive]->draw();
			if (currentFrame - old_time_key >= 0.07f)
			{
				old_time_key = currentFrame;

				MoveMenu();
			
				if (keys[GLFW_KEY_ENTER])
				{
					if (this->menuActive == 0)
					{
						menuVisible = false;
						loadVisible =true;
						std::thread *worldThread =  new std::thread(createWorld, this);
						worldThread->detach();
						
					}
					else if (this->menuActive == 3)
						this->menuActive = 5;
					else if (this->menuActive == 4)
						glfwSetWindowShouldClose(window, GL_TRUE);
					else if (this->menuActive == 5)
						this->menuActive = 0;
				}
				usleep(10000);
			}
		}
		else
		{

			if (loadVisible == true)
			{
				if (this->loadActive == 3)
					this->loadActive = 0;
				load[this->loadActive]->draw();
				usleep(400000);
				this->loadActive++;
			}
			else
			{	

				if (currentFrame - old_time_key >= 0.01f)
				{
					this->world->draw(camera.GetViewMatrix());
					old_time_key = currentFrame;
					DoMovement();
					if (world->getStatus() == 1)
					{
						delete this->world;
						this->menuActive = 0;
						menuVisible = true;
						// glfwSetWindowShouldClose(window, GL_TRUE);
					}
				}

			}
		}
		glfwSwapBuffers(window);
		glfwMakeContextCurrent(NULL);
		mu.unlock();
	}
	for (std::vector<MainMenu*>::iterator it = this->Menus.begin() ; it != this->Menus.end(); )
	{
		if (it != this->Menus.end())
		{
			delete (*it);
			it = this->Menus.erase(it);
		}
	}
	
	for (std::vector<LoadingScreen*>::iterator it = this->load.begin() ; it != this->load.end(); )
	{
		if (it != this->load.end())
		{
			delete (*it);
			it = this->load.erase(it);
		}
	}

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
		this->screen_x = rhs.screen_x;
		this->screen_y = rhs.screen_y;
		this->SCREEN_HEIGHT = rhs.SCREEN_HEIGHT;
		this->SCREEN_WIDTH = rhs.SCREEN_WIDTH;
		this->world = rhs.world;
	}
	return (*this);
}
//end canonical form

void Game::MoveMenu(void)
{
	// Player controls
	if (this->menuActive != 5)
	{
		if (keys[GLFW_KEY_UP])
		{
			if (this->menuActive > 0)
				this->menuActive--;
		}
		else if (keys[GLFW_KEY_DOWN])
		{
			if (this->menuActive < 4)
				this->menuActive++;
		}
	}

}
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
	{
		this->world->ProcessKeyboard(FWD);
	}
	else if (keys[GLFW_KEY_DOWN])
		this->world->ProcessKeyboard(BKW);
	else if (keys[GLFW_KEY_LEFT])
		this->world->ProcessKeyboard(LFT);
	else if (keys[GLFW_KEY_RIGHT])
		this->world->ProcessKeyboard(RGT);

	// Plant a bomb
	if (keys[GLFW_KEY_SPACE])
		this->world->ProcessKeyboard(SPC);
	usleep(10000);
}

void 	createWorld(Game *game)
{
	game->createWorld2();
}

void 	Game::createWorld2(void )
{
	this->world = new World(*(this->shader), "resources/models/world.obj", this->screen_x, this->screen_y, this->window);
	this->loadVisible = false;
}