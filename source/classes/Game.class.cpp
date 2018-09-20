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
int		lastkeypressed;
int		tempKEY;

// Is called whenever a key is pressed/released via GLFW
void	KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		// glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			lastkeypressed = key;
			std::cout << "pressed key " << key << std::endl;
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
	// std::cout << "pressed key " << key << std::endl;
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

	int last_menu = 0;
	int temp22 = 2;
	this->stage = 1;
	lastX = 400;
	lastY = 300;
	firstMouse = true;
	deltaTime = 0.0;
	lastFrame = 0.0;
	menuVisible = true;
	loadVisible = false;
	soundMenuVisible = false;
	pauseVisible = false;
	this->WorldLoaded = false;
	this->menuActive = 0;
	this->loadActive = 0;
	this->soundActive = 0;
	this->pauseActive = 0;
	this->check = 0;
	this->key_change = 0;

	this->keyUP = GLFW_KEY_UP;
	this->keyDOWN = GLFW_KEY_DOWN;
	this->keyLEFT = GLFW_KEY_LEFT;
	this->keyRIGHT = GLFW_KEY_RIGHT;
	this->keyBOMB = GLFW_KEY_SPACE;

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
	//Shader	shader("resources/shaders/lighting.vs", "resources/shaders/lighting.frag");

	this->shader = &shader;
	// Load models
	for (int i = 0; i < 25; i++)
		this->Menus.push_back(new MainMenu(shader, "resources/models/menu/Menu_" + std::to_string(i) + ".obj"));

	for (int i = 0; i < 9; i++)
		this->load.push_back(new LoadingScreen(shader, "resources/models/menu/LoadingScreen/Loading_screen_" + std::to_string(i) + ".obj"));

	for (int i = 0; i < 11; i++)
		this->soundMenu.push_back(new SoundMenu(shader, "resources/models/menu/SoundScreen/SoundScreen_" + std::to_string(i) + ".obj"));
	
	for (int i = 0; i < 6; i++)
		this->pauseMenu.push_back(new PauseMenu(shader, "resources/models/menu/PauseMenu/pauseMenu_" + std::to_string(i) + ".obj"));

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
	this->projection = projection;

	GLfloat old_time = 0.0f;
	GLfloat old_time_key = 0.0f;

	// ---------Sound-----------
	World::sound->playMusic();
	// -------------------------

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
			// std::cout << "FPS: " << std::to_string(1.0f / deltaTime) << std::endl;
		}
		
		// Check and call events
		glfwPollEvents();

		// Clear the colorbuffer
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();
		this->placeSpotLight();
		// set the camera view and projection
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
		if (menuVisible == true || soundMenuVisible == true)
		{
			if (soundMenuVisible == true)
			{
				soundMenu[this->soundActive]->draw();
				MoveSoundMenu();
			}
			else
			{
				Menus[this->menuActive]->draw();
				MoveMenu();
			}
		
			if (keys[GLFW_KEY_ENTER])
			{
				keys[GLFW_KEY_ENTER] = false;
				if (menuVisible == true)
				{
					// std::cout << "active menu: " << this->menuActive << std::endl;
					if (this->menuActive == 0)
					{
						this->loadActive = 0;
						menuVisible = false;
						loadVisible =true;
						std::thread *worldThread =  new std::thread(createWorld, this);
						worldThread->detach();
					}
					else if (this->menuActive == 1)//load Game
					{
						if (exist("saveGame"))
						{
							std::ifstream file;
							std::string	line;
							std::vector<std::string> tokens;
							file.open("saveGame");
							getline(file, line);
							tokens = strsplit(line, ' ');
							if (tokens[0] == "stage:")
							{
								std::cout << tokens[0] << std::endl;

								std::istringstream(tokens[1]) >> this->stage;
							}
							file.close();
							switch(this->stage)
							{
								case 1:
									this->loadActive = 0;
									break;
								case 2:
									this->loadActive = 4;
									break;
								case 3:
									this->loadActive = 7;
									break;
								default:
									break;
							}
							// this->loadActive = 0;
							menuVisible = false;
							loadVisible =true;
							std::thread *worldThread =  new std::thread(loadGame, this);
							worldThread->detach();
						}
						else
							this->menuActive = 16;
					}
						// this->menuActive = 5;
					else if (this->menuActive == 4)
						glfwSetWindowShouldClose(window, GL_TRUE);
					else if (this->menuActive == 2)//Options
						this->menuActive = 6;
					else if (this->menuActive == 3)
						this->menuActive = 5;
					else if (this->menuActive == 4)
						glfwSetWindowShouldClose(window, GL_TRUE);
					else if (this->menuActive == 6)//Resolution
						this->menuActive = 10;
					else if (this->menuActive == 7) //Sound
					{
						menuVisible = false;
						this->soundActive = 0;
						soundMenuVisible = true;
					}
					else if (this->menuActive == 8) //Key Bindings
						this->menuActive = 17;
					else if (this->menuActive == 5) //Back to Main Menu
						this->menuActive = 0;
					else if (this->menuActive == 15) //Going back to Options
						this->menuActive = 6;
					else if (this->menuActive == 16)
						this->menuActive = 1;
					//------------------------------Changing Keys----------------------------------------------
					else if (this->menuActive == 17)//changing UP key
					{
						last_menu = this->menuActive;
						this->key_change = 0;
						this->menuActive = 24;
						keys[GLFW_KEY_ENTER] = false;
					}
					else if (this->menuActive == 18)//changing DOWN key
					{
						last_menu = this->menuActive;
						this->key_change = 1;
						this->menuActive = 24;
						keys[GLFW_KEY_ENTER] = false;
					}
					else if (this->menuActive == 19)//changing LEFT key
					{
						last_menu = this->menuActive;
						this->key_change = 2;
						this->menuActive = 24;
						keys[GLFW_KEY_ENTER] = false;
					}
					else if (this->menuActive == 20)//changing RIGHT key
					{
						last_menu = this->menuActive;
						this->key_change = 3;
						this->menuActive = 24;
						keys[GLFW_KEY_ENTER] = false;
					}
					else if (this->menuActive == 21)//changing BOMB key
					{
						last_menu = this->menuActive;
						this->key_change = 4;
						this->menuActive = 24;
						keys[GLFW_KEY_ENTER] = false;
					}
					else if (this->menuActive == 22)//changing 1st/3rd person View key
					{
						last_menu = this->menuActive;
						this->key_change = 5;
						this->menuActive = 24;
						keys[GLFW_KEY_ENTER] = false;
					}
					else if (this->menuActive == 23)
					{
						last_menu = 0;
						this->key_change = 0;
						this->menuActive = 8;
					}
					//---------------------------END Changing KEYS-------------------------------------------------
					else if (this->menuActive == 9)
					{
						if (this->check == 1)
						{
							menuVisible = false;
							pauseVisible = true;
							check = 0;
							this->pauseActive = 3;
						}
						else
							this->menuActive = 2;
					}
					//-------------------Changing Resolutions--------------------------------------------------------
					else if (this->menuActive == 10) //Resolution change
					{
						this->screen_x = 1024;
						this->screen_y = 576;
						glfwSetWindowSize(window, this->screen_x, this->screen_y);
					}
					else if (this->menuActive == 11) //Resolution change
					{
						this->screen_x = 1280;
						this->screen_y = 720;
						glfwSetWindowSize(window, this->screen_x, this->screen_y);
					}
					else if (this->menuActive == 12) //Resolution change
					{
						this->screen_x = 1920;
						this->screen_y = 1080;
						glfwSetWindowSize(window, this->screen_x, this->screen_y);
					}
					else if (this->menuActive == 13) //Window Mode
					{
						glfwSetWindowMonitor(window, nullptr, 0, 0, this->screen_x, this->screen_y, GLFW_DONT_CARE);
						keys[GLFW_KEY_ENTER] = true;
					}
					else if (this->menuActive == 14) //Full Screen Mode 
					{
						if (glfwGetWindowMonitor(window) == nullptr)
							temp22 = 0;
						if (temp22 < 2)
						{
							GLFWmonitor *monitor = glfwGetPrimaryMonitor();
							const GLFWvidmode *mode = glfwGetVideoMode(monitor);

							glfwSetWindowMonitor(window, monitor, 0, 0, 2300, 1920, mode->refreshRate);
							glfwSetWindowMonitor(window, monitor, 0, 0, this->screen_x, this->screen_y, mode->refreshRate);
							keys[GLFW_KEY_ENTER] = true;
							temp22++;
						}
					}
				}
				//--------------------Changing Sound--------------------------------------------------------------------
				else if (soundMenuVisible == true)
				{
					if (this->soundActive == 0)
						this->soundActive = 3;
					else if (this->soundActive == 1)
						this->soundActive = 7;
					else if (this->soundActive == 2)
					{
						this->menuActive = 7;
						soundMenuVisible = false;
						menuVisible = true;
					}
					else if ((this->soundActive >= 3) && (this->soundActive <= 6))
						this->soundActive = 0;
					else if ((this->soundActive >= 7) && (this->soundActive <= 10))
						this->soundActive = 1;
				}
			}
			//---------------------Selecting Keys---------------------------------------------------------------------------
			else if (this->menuActive == 24)
			{
				if (keys[lastkeypressed])
				{
					if (!(keys[GLFW_KEY_ENTER] && keys[GLFW_KEY_ESCAPE]))
					{
						tempKEY = lastkeypressed;
						if (this->key_change == 0)
						{
							if ((tempKEY != this->keyDOWN) && (tempKEY != this->keyLEFT) && (tempKEY != this->keyRIGHT) && (tempKEY != this->keyBOMB))
								this->keyUP = tempKEY;
							else
								std::cout << "key is already assigned" << std::endl;
						}
						if (this->key_change == 1)
						{
							if ((tempKEY != this->keyUP) && (tempKEY != this->keyLEFT) && (tempKEY != this->keyRIGHT) && (tempKEY != this->keyBOMB))
								this->keyDOWN = tempKEY;
							else
								std::cout << "key is already assigned" << std::endl;
						}
						if (this->key_change == 2)
						{
							if ((tempKEY != this->keyUP) && (tempKEY != this->keyDOWN) && (tempKEY != this->keyRIGHT) && (tempKEY != this->keyBOMB))
								this->keyLEFT = tempKEY;
							else
								std::cout << "key is already assigned" << std::endl;
						}
						if (this->key_change == 3)
						{
							if ((tempKEY != this->keyUP) && (tempKEY != this->keyDOWN) && (tempKEY != this->keyLEFT) && (tempKEY != this->keyBOMB))
								this->keyRIGHT = tempKEY;
							else
								std::cout << "key is already assigned" << std::endl;
						}
						if (this->key_change == 4)
						{
							if ((tempKEY != this->keyUP) && (tempKEY != this->keyDOWN) && (tempKEY != this->keyLEFT) && (tempKEY != this->keyRIGHT))
								this->keyBOMB = tempKEY;
							else
								std::cout << "key is already assigned" << std::endl;
						}
						// if (this->key_change == 5)
						// 	this->keyCHANGEVIEW = tempKEY;
						this->menuActive = last_menu;
					}
				}
			}
			//--------------------END Selecting Keys-------------------------------------------------------------------------
			else if (keys[GLFW_KEY_SPACE]) // Changing volume in-game
			{
				if (this->soundActive >= 3 && this->soundActive <= 10)
				{
					keys[GLFW_KEY_SPACE] = false;
					switch (this->soundActive)
					{
						case 3:
							World::sound->setVolMusic(0);
							break ;
						case 4:
							World::sound->setVolMusic(0.2);
							break ;
						case 5:
							World::sound->setVolMusic(0.5);
							break ;
						case 6:
							World::sound->setVolMusic(1.0);
							break ;
						case 7:
							World::sound->setVolEffects(0);
							break ;
						case 8:
							World::sound->setVolEffects(0.2);
							break ;
						case 9:
							World::sound->setVolEffects(0.5);
							break ;
						case 10:
							World::sound->setVolEffects(1.0);
							break ;
						default:
							break ;
					}
				}
			}
			usleep(10000);
		}
		else
		{
			if (loadVisible == true)
			{
				if (this->loadActive == 3)
				{
					this->loadActive = 0;
					this->stage = 1;
				}
				if (this->loadActive == 6)
				{
					this->loadActive = 4;
				}
				if (this->loadActive == 9)
				{
					this->loadActive = 7;
				}
				load[this->loadActive]->draw();
				usleep(400000);
				this->loadActive++;
			}
			//---------------------Pause Menu------------------------------------------
			else if (pauseVisible == true)
			{
				pauseMenu[this->pauseActive]->draw();
				MovePause();
				if (keys[GLFW_KEY_ENTER])
				{
					keys[GLFW_KEY_ENTER] = false;
					if (this->pauseActive >= 0 && this->pauseActive <= 5)
					{
						std::thread *worldThread;
						std::ifstream file;
						std::string	line;
						std::vector<std::string> tokens;
						switch (this->pauseActive)
						{
							case 0://Resume Game
								pauseVisible = false;
								break ;
							case 1:// Save Game
								this->world->saveWorld();
								pauseVisible = false;
								break ;
							case 2://Load Game
								if (exist("saveGame"))
								{
									delete this->world;
									
									file.open("saveGame");
									
									getline(file, line);
									tokens = strsplit(line, ' ');
									if (tokens[0] == "stage:")
									{
										std::cout << tokens[0] << std::endl;
										std::istringstream(tokens[1]) >> this->stage;
									}
									file.close();
									switch(this->stage)
									{
										case 1:
											this->loadActive = 0;
											break;
										case 2:
											this->loadActive = 4;
											break;
										case 3:
											this->loadActive = 7;
											break;
										default:
											break;
									}
									pauseVisible = false;
									menuVisible = false;
									pauseVisible = false;
									loadVisible = true;
									worldThread = new std::thread(loadGame, this);
									worldThread->detach();
								}
								else
									this->pauseActive = 5;
								break ;
							case 3://Options
								menuVisible = true;
								pauseVisible =false;
								this->check = 1;
								this->menuActive = 6;
								break ;
							case 4://Quit to Main menu
								delete this->world;
								this->menuActive = 0;
								menuVisible = true;
								pauseVisible = false;
								break ;
							case 5://Error_message
								this->pauseActive = 2;
								break ;
							default:
								break ;
						}
					}
				}
			}
			//--------------------End Pause Menu-------------------------------------
			else
			{	
				if (currentFrame - old_time_key >= 0.01f)
				{
					old_time_key = currentFrame;
					this->world->draw(camera.GetViewMatrix(), currentFrame);
					DoMovement();
					if (world->getStatus() == 1)
					{
						delete this->world;
						camera.moveCamForMenu();
						camera.ProcessMouseMovement(0, -250);
						this->menuActive = 0;
						menuVisible = true;
					}
					else if (world->getStatus() == 2)
					{
						if (this->stage == 3)
						{
							delete this->world;
							camera.moveCamForMenu();
							camera.ProcessMouseMovement(0, -250);
							this->menuActive = 0;
							this->stage = 4;

							menuVisible = true;
						}
						
						else if (this->stage == 1)
						{
							this->loadActive = 4;
							this->stage = 2;
						}
						else if (this->stage == 2)
						{
							this->loadActive = 7;
							this->stage = 3;
						}
						if (this->stage < 4)
						{
							loadVisible = true;
							camera.moveCamForMenu();
							camera.ProcessMouseMovement(0, -250);
							std::thread *worldThread =  new std::thread(loadStage, this);
							worldThread->detach();
						}
					}
				}
			}
		}
		glfwSwapBuffers(window);
		glfwMakeContextCurrent(NULL);
		mu.unlock();
	}
	mu.lock();
	glfwMakeContextCurrent(window);
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

	for (std::vector<SoundMenu*>::iterator it = this->soundMenu.begin() ; it != this->soundMenu.end(); )
	{
		if (it != this->soundMenu.end())
		{
			delete (*it);
			it = this->soundMenu.erase(it);
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


void Game::MovePause(void)
{
	// Pause Menu controls
	if (this->pauseActive != 5)
	{
		if (keys[GLFW_KEY_UP])
		{
			keys[GLFW_KEY_UP] = false;
			if (this->pauseActive > 0)
					this->pauseActive--;
		}
		else if (keys[GLFW_KEY_DOWN])
		{
			keys[GLFW_KEY_DOWN] = false;
			if (this->pauseActive < 4)
				this->pauseActive++;
		}
	}
}

void Game::MoveSoundMenu(void)
{
	//SoundMenu controls
	if (keys[GLFW_KEY_UP])
	{
		keys[GLFW_KEY_UP] = false;
		if ((this->soundActive > 0) && (this->soundActive < 3))
			this->soundActive--;
	}
	else if (keys[GLFW_KEY_DOWN])
	{
		keys[GLFW_KEY_DOWN] = false;
		if (this->soundActive < 2)
			this->soundActive++;
	}
	else if (keys[GLFW_KEY_LEFT])
	{
		keys[GLFW_KEY_LEFT] = false;
			if (this->soundActive > 3)
			{
				if (soundActive <= 6)
					this->soundActive--;
			}
			if (this->soundActive > 7)
			{
				if (this->soundActive <= 10)
					this->soundActive--;
			}
	}
	else if (this->soundActive >= 3)
	{
		if (keys[GLFW_KEY_RIGHT])
		{
			keys[GLFW_KEY_RIGHT] = false;
			if ((this->soundActive < 6) || (this->soundActive >= 7 && this->soundActive < 10))
				this->soundActive++;
		}
	}
}

void Game::MoveMenu(void)
{
	// Menu controls
	if (this->menuActive != 5 || this->menuActive != 16)
	{
		if (keys[GLFW_KEY_UP])
		{
			if (this->menuActive != 24)
				keys[GLFW_KEY_UP] = false;
			if (this->menuActive > 0)
			{
				if ((this->menuActive < 5) || (this->menuActive > 6 && this->menuActive <= 9) || (this->menuActive > 10 && this->menuActive < 16) || (this->menuActive > 17 && this->menuActive < 24))
					this->menuActive--;
			}
		}
		else if (keys[GLFW_KEY_DOWN])
		{
			if (this->menuActive != 24)
				keys[GLFW_KEY_DOWN] = false;
			if (this->menuActive < 4)
				this->menuActive++;
			if (this->menuActive >= 6)
			{
				if (this->menuActive < 9)
					this->menuActive++;
			}
			if (this->menuActive >= 10)
			{
				if (this->menuActive < 15)
					this->menuActive++;
			}
			if (this->menuActive >= 17 && this->menuActive < 23)
			{
				if (this->menuActive < 23)
					this->menuActive++;
			}
		}
		else if (check == 1)
		{
			if (keys[GLFW_KEY_ESCAPE])
			{
				keys[GLFW_KEY_ESCAPE] = false;
				if (this->menuActive >= 6 || this->menuActive <= 9)
				{
					menuVisible = false;
					pauseVisible = true;
					this->check = 1;
					this->pauseActive = 3;
				} 
			}
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
	if (keys[this->keyUP])
	{
		this->world->ProcessKeyboard(FWD, camera);
	}
	else if (keys[this->keyDOWN])
		this->world->ProcessKeyboard(BKW, camera);
	else if (keys[this->keyLEFT])
		this->world->ProcessKeyboard(LFT, camera);
	else if (keys[this->keyRIGHT])
		this->world->ProcessKeyboard(RGT, camera);

	// Plant a bomb
	if (keys[this->keyBOMB])
	{
		keys[this->keyBOMB] = false;
		this->world->ProcessKeyboard(SPC, camera);
	}
	else if (keys[GLFW_KEY_ESCAPE])
	{
		keys[GLFW_KEY_ESCAPE] = false;
		this->pauseActive = 0;
		pauseVisible = true;
	}
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

void 	loadStage(Game *game)
{
	std::cout << "in Load stage calling load stage from Game" << std::endl;
	game->loadStage1();
}

void 	Game::loadStage1(void )
{
	std::cout << "in Load stage1 calling load stage from World" << std::endl;

	this->world->loadStage(this->stage);
	std::cout << "set LoadVisible to false" << std::endl;
	this->loadVisible = false;
}

void 	loadGame(Game *game)
{
	game->loadGame1();
}

void 	Game::loadGame1(void )
{
	this->world = new World(*(this->shader), "resources/models/world.obj", this->screen_x, this->screen_y, this->window, "saveGame");
	this->stage = this->world->getStage();
	this->loadVisible = false;
}
void	Game::placeSpotLight(void)
{
	glUniform3f(glGetUniformLocation(this->shader->getProgram(), "spotLight.position"), camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
	glUniform3f(glGetUniformLocation(this->shader->getProgram(), "spotLight.direction"), camera.getFront().x, camera.getFront().y, camera.getFront().z);
	glUniform3f(glGetUniformLocation(this->shader->getProgram(), "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(this->shader->getProgram(), "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shader->getProgram(), "spotLight.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(this->shader->getProgram(), "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(this->shader->getProgram(), "spotLight.linear"), 0.0009f);
	glUniform1f(glGetUniformLocation(this->shader->getProgram(), "spotLight.quadratic"), 0.00032f);
	glUniform1f(glGetUniformLocation(this->shader->getProgram(), "spotLight.cutOff"), glm::cos(glm::radians(25.0f)));
	glUniform1f(glGetUniformLocation(this->shader->getProgram(), "spotLight.outerCutOff"), glm::cos(glm::radians(27.0f)));
}

inline bool Game::exist(const std::string& name)
{
	std::ifstream file(name);
	if(!file)				// If the file was not found, then file is 0, i.e. !file=1 or true.
		return false;		// The file was not found.
	else					// If the file was found, then file is non-0.
		return true;		// The file was found.
}