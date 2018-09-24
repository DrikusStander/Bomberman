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
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			lastkeypressed = key;
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
}

Game::Game(const int width, const int height, const bool debug) : screen_x(width), screen_y(height), debug(debug)
{
	std::cout << "Game: Loading..." << std::endl;
	bool	died_msg = false;
	this->last_menu = 0;
	this->fullScreenItteration = 2;
	this->stage = 1;
	lastX = 400;
	lastY = 300;
	firstMouse = true;
	this->deltaTime = 0.0;
	this->lastFrame = 0.0;
	this->menuVisible = true;
	this->loadVisible = false;
	this->soundMenuVisible = false;
	this->pauseVisible = false;
	this->WorldLoaded = false;
	this->menuActive = 0;
	this->loadActive = 0;
	this->soundActive = 0;
	this->pauseActive = 0;
	this->check = 0;
	this->key_change = 0;
	this->pause_died = 0.0f;
	this->you_won = 0.0f;
	this->you_died = 0.0f;
	this->lives = 3;
	// this->wtclogo_animate = 0.0f;
	this->readConfig();

	

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
	GLFWwindow	*window = glfwCreateWindow(this->screen_x, this->screen_y, "Bomberman", nullptr, nullptr);
	this->window = window;
	if (window == nullptr)
	{
		glfwTerminate();
		throw Exceptions::CreateWindowFailed();
	}
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	if (this->debug == false)
		glfwSetCursorPosCallback(window, nullptr);
	else
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
	this->shaderNormal = &shader;
	Shader	shaderFlash("resources/shaders/lighting.vs", "resources/shaders/lighting.frag");
	this->shaderFlash = &shaderFlash;
	this->shaderActive = this->shaderNormal;

	for (int i = 0; i <= 60; i++)
		this->wtc.push_back(new wtcLogo(shader, "resources/models/wtc_logo/wtc_" + std::to_string(i) + ".obj"));

	std::thread *modelsThread =  new std::thread(loadmodels, this);
	modelsThread->detach();

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
	this->projection = projection;
	GLfloat old_time = 0.0f;
	GLfloat old_time_key = 0.0f;
	this->wtc_logo = true;
	bool	wtc_last = false;
	int		wtc_active = 0;
	int		wtc_count = 0;

	// ---------Sound-----------
	World::sound->playMusic();
	World::sound->setVolMusic(this->volMusic);
	World::sound->setVolEffects(this->volEffect);
	// -------------------------
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		mu.lock();
		glfwMakeContextCurrent(window);
		wtcAnimation:
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		this->deltaTime = currentFrame - this->lastFrame;
		this->lastFrame = currentFrame;
		if (currentFrame - old_time >= 1.0f)
			old_time = currentFrame;
		// Check and call events
		glfwPollEvents();
		// Clear the colorbuffer
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if ((this->menuVisible == true) || (this->pauseActive == true))
		{
			this->toggleFlash = false;
			this->shaderActive = this->shaderNormal;
		}
		else
		{
			if (this->toggleFlash == true)
			{
				this->shaderActive = this->shaderFlash;
				this->placeSpotLight();
			}
			else
				this->shaderActive = this->shaderNormal;
		}
		this->shaderActive->Use();
		// set the camera view and projection
		glUniformMatrix4fv(glGetUniformLocation(this->shaderActive->getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(this->shaderActive->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
		if (wtc_logo == true)
		{
			wtc_count++;
			this->wtc[wtc_active]->draw();
			if (wtc_last == false)
			{
				if (wtc_active < 60)
					wtc_active++;
				else
					wtc_last = true;
			}
			else if (wtc_last == true)
			{
				if (wtc_active > 0)
					wtc_active--;
				else
					wtc_last = false;
			}
			if (wtc_count < 4)
			{
				glfwSwapBuffers(window);
				goto wtcAnimation;
			}
			else
				wtc_count = 0;

			usleep(30000);
		}
		if (this->menuVisible == true || this->soundMenuVisible == true)
			this->menuIsVisible();
		else
		{
			if (this->loadVisible == true)
				this->loadIsVisible();
			else if (this->pauseVisible == true)
				this->pauseIsVisible();
			else
			{
				if (currentFrame - old_time_key >= 0.01f)
				{
					old_time_key = currentFrame;
					this->world->setShader((*this->shaderActive));
					if (this->lives != this->world->getLives())
					{
						this->toggleFlash = false;
						this->you_died = this->lastFrame;
						this->lives = this->world->getLives();
						died_msg = true;
					}
					if (died_msg == true)
					{
						if (this->lastFrame - this->you_died >= 1.0f)
						{
							this->you_died = this->lastFrame;
							died_msg = false;
						}
						else
						{
							camera.moveCamForMenu();
							camera.ProcessMouseMovement(0, -250);
							this->Menus[27]->draw();
						}
					}
					else
						this->world->draw(camera, currentFrame);
					DoMovement();
					if (world->getStatus() == 1)
					{
						delete this->world;
						this->menuVisible = true;
						this->menuActive = 26;
						this->pause_died = this->lastFrame;
					}
					else if (world->getStatus() == 2)
					{
						switch(this->stage)
						{
							case 3:
								delete this->world;
								this->stage = 4;
								menuVisible = true;
								this->menuActive = 28;
								this->you_won = this->lastFrame;
								break;
							case 1:
								this->loadActive = 4;
								this->stage = 2;
								break;
							case 2:
								this->loadActive = 7;
								this->stage = 3;
								break;
							default:
								break;
						}
						if (this->stage < 4)
						{
							loadVisible = true;
							if (this->debug == false)
							{
								camera.moveCamForMenu();
								camera.ProcessMouseMovement(0, -250);
							}
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
	glfwTerminate();
}

Game::Game(Game const & src)
{
	*this = src;
}

Game::~Game(void)
{
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
}

Game	&Game::operator=(Game const &rhs)
{
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

void Game::MovePause(void)
{
	// Debugging
	if (this->debug == true)
	{
		// Camera controls
		if (keys[GLFW_KEY_Q])
			camera.ProcessKeyboard(UP, this->deltaTime);
		else if (keys[GLFW_KEY_E])
			camera.ProcessKeyboard(DOWN, this->deltaTime);
		else if (keys[GLFW_KEY_W])
			camera.ProcessKeyboard(FORWARD, this->deltaTime);
		else if (keys[GLFW_KEY_S])
			camera.ProcessKeyboard(BACKWARD, this->deltaTime);
		else if (keys[GLFW_KEY_A])
			camera.ProcessKeyboard(LEFT, this->deltaTime);
		else if (keys[GLFW_KEY_D] )
			camera.ProcessKeyboard(RIGHT, this->deltaTime);
	}

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
	// Debugging
	if (this->debug == true)
	{
		// Camera controls
		if (keys[GLFW_KEY_Q])
			camera.ProcessKeyboard(UP, this->deltaTime);
		else if (keys[GLFW_KEY_E])
			camera.ProcessKeyboard(DOWN, this->deltaTime);
		else if (keys[GLFW_KEY_W])
			camera.ProcessKeyboard(FORWARD, this->deltaTime);
		else if (keys[GLFW_KEY_S])
			camera.ProcessKeyboard(BACKWARD, this->deltaTime);
		else if (keys[GLFW_KEY_A])
			camera.ProcessKeyboard(LEFT, this->deltaTime);
		else if (keys[GLFW_KEY_D] )
			camera.ProcessKeyboard(RIGHT, this->deltaTime);
	}

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
	// Debugging
	if (this->debug == true)
	{
		// Camera controls
		if (keys[GLFW_KEY_Q])
			camera.ProcessKeyboard(UP, this->deltaTime);
		else if (keys[GLFW_KEY_E])
			camera.ProcessKeyboard(DOWN, this->deltaTime);
		else if (keys[GLFW_KEY_W])
			camera.ProcessKeyboard(FORWARD, this->deltaTime);
		else if (keys[GLFW_KEY_S])
			camera.ProcessKeyboard(BACKWARD, this->deltaTime);
		else if (keys[GLFW_KEY_A])
			camera.ProcessKeyboard(LEFT, this->deltaTime);
		else if (keys[GLFW_KEY_D] )
			camera.ProcessKeyboard(RIGHT, this->deltaTime);
	}

	// Menu controls
	if (this->menuActive != 5 || this->menuActive != 16 || this->menuActive != 25 || this->menuActive != 26 || this->menuActive != 27 || this->menuActive != 28)
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
					this->menuVisible = false;
					this->pauseVisible = true;
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
	// Debugging
	if (this->debug == true)
	{
		this->keyUP = GLFW_KEY_UP;
		this->keyDOWN = GLFW_KEY_DOWN;
		this->keyLEFT = GLFW_KEY_LEFT;
		this->keyRIGHT = GLFW_KEY_RIGHT;
		this->keyBOMB = GLFW_KEY_SPACE;
		this->keyFLASH = GLFW_KEY_F;

		// Camera controls
		if (keys[GLFW_KEY_Q])
			camera.ProcessKeyboard(UP, this->deltaTime);
		else if (keys[GLFW_KEY_E])
			camera.ProcessKeyboard(DOWN, this->deltaTime);
		else if (keys[GLFW_KEY_W])
			camera.ProcessKeyboard(FORWARD, this->deltaTime);
		else if (keys[GLFW_KEY_S])
			camera.ProcessKeyboard(BACKWARD, this->deltaTime);
		else if (keys[GLFW_KEY_A])
			camera.ProcessKeyboard(LEFT, this->deltaTime);
		else if (keys[GLFW_KEY_D] )
			camera.ProcessKeyboard(RIGHT, this->deltaTime);
	}

	// Player controls
	if (keys[this->keyUP])
		this->world->ProcessKeyboard(FWD, camera, this->toggleFlash);
	else if (keys[this->keyDOWN])
		this->world->ProcessKeyboard(BKW, camera, this->toggleFlash);
	else if (keys[this->keyLEFT])
		this->world->ProcessKeyboard(LFT, camera, this->toggleFlash);
	else if (keys[this->keyRIGHT])
		this->world->ProcessKeyboard(RGT, camera, this->toggleFlash);
	// Plant a bomb
	if (keys[this->keyBOMB])
	{
		keys[this->keyBOMB] = false;
		this->world->ProcessKeyboard(SPC, camera, this->toggleFlash);
	}
	else if (keys[GLFW_KEY_ESCAPE])
	{
		keys[GLFW_KEY_ESCAPE] = false;
		this->pauseActive = 0;
		this->toggleFlash = false;
		this->pauseVisible = true;
		if (this->debug == false)
			glfwSetCursorPosCallback(window, nullptr);
	}
	else if (keys[this->keyFLASH])
	{
		keys[this->keyFLASH] = false;
		if (this->toggleFlash == true)
		{
			this->toggleFlash = false;
			if (this->debug == false)
				glfwSetCursorPosCallback(window, nullptr);
		}
		else if (this->toggleFlash == false)
		{
			this->toggleFlash = true;
			camera.resetPitch();
			if (this->debug == false)
				glfwSetCursorPosCallback(window, MouseCallback);
			switch (this->world->getDirLast())
			{
				case FWD:
					camera.setYaw(-90.0f);
					break ;
				case RGT:
					camera.setYaw(0.0f);
					break ;
				case BKW:
					camera.setYaw(90.0f);
					break ;
				case LFT:
					camera.setYaw(180.0f);
					break ;
				default:
					camera.setYaw(-90.0f);
					break ;
			}
			this->world->moveCameraFp(camera);
		}
	}
	usleep(10000);
}

void 	createWorld(Game *game)
{
	game->createWorld2();
}

void 	Game::createWorld2(void )
{
	this->world = new World(*(this->shaderActive), "resources/models/world.obj", this->screen_x, this->screen_y, this->window, this->debug);
	this->loadVisible = false;
}

void 	loadStage(Game *game)
{
	game->loadStage1();
}

void 	Game::loadStage1(void )
{
	this->world->loadStage(this->stage);
	this->loadVisible = false;
}

void 	loadGame(Game *game)
{
	game->loadGame1();
}

void 	Game::loadGame1(void )
{
	this->world = new World(*(this->shaderNormal), "resources/models/world.obj", this->screen_x, this->screen_y, this->window, "saveGame", this->debug);
	this->stage = this->world->getStage();
	this->loadVisible = false;
	this->lives = this->world->getLives();
}
void	Game::placeSpotLight(void)
{
	glUniform3f(glGetUniformLocation(this->shaderFlash->getProgram(), "spotLight.position"), camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
	glUniform3f(glGetUniformLocation(this->shaderFlash->getProgram(), "spotLight.direction"), camera.getFront().x, camera.getFront().y, camera.getFront().z);
	glUniform3f(glGetUniformLocation(this->shaderFlash->getProgram(), "spotLight.ambient"), 0.1f, 0.1f, 0.1f);
	glUniform3f(glGetUniformLocation(this->shaderFlash->getProgram(), "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(this->shaderFlash->getProgram(), "spotLight.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(this->shaderFlash->getProgram(), "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(this->shaderFlash->getProgram(), "spotLight.linear"), 0.00009f);
	glUniform1f(glGetUniformLocation(this->shaderFlash->getProgram(), "spotLight.quadratic"), 0.000092f);
	glUniform1f(glGetUniformLocation(this->shaderFlash->getProgram(), "spotLight.cutOff"), glm::cos(glm::radians(23.0f)));
	glUniform1f(glGetUniformLocation(this->shaderFlash->getProgram(), "spotLight.outerCutOff"), glm::cos(glm::radians(27.0f)));
}

inline bool Game::exist(const std::string& name)
{
	std::ifstream file(name);
	if(!file)				// If the file was not found, then file is 0, i.e. !file=1 or true.
		return false;		// The file was not found.
	else					// If the file was found, then file is non-0.
		return true;		// The file was found.
}

void	Game::writeConfig(void)
{
	std::stringstream sstr;
	std::ofstream file;
	file.open("gameConfig");
	sstr << this->volMusic;
	file << "volMusic: " + sstr.str() + "\n";
	sstr.str("");
	sstr << this->volEffect;
	file << "volEffect: " + sstr.str() + "\n";
	sstr.str("");
	sstr << this->keyUP;
	file << "keyUp: " + sstr.str() + "\n";
	sstr.str("");
	sstr << this->keyDOWN;
	file << "keyDown: " + sstr.str() + "\n";
	sstr.str("");
	sstr << this->keyLEFT;
	file << "keyLeft: " + sstr.str() + "\n";
	sstr.str("");
	sstr << this->keyRIGHT;
	file << "keyRight: " + sstr.str() + "\n";
	sstr.str("");
	sstr << this->keyBOMB;
	file << "keyBomb: " + sstr.str() + "\n";
	sstr.str("");
	sstr << this->keyFLASH;
	file << "keyFlash: " + sstr.str() + "\n";
	sstr.str("");
	sstr << this->screen_x;
	file << "screenX: " + sstr.str() + "\n";
	sstr.str("");
	sstr << this->screen_y;
	file << "screenY: " + sstr.str() + "\n";
	sstr.str("");
	file.close();
}

void	Game::readConfig(void)
{
	std::ifstream file;
	file.open("gameConfig");
	std::string	line;
	std::vector<std::string> tokens;
	//read volMusic
	getline(file, line);
	tokens = strsplit(line, ' ');
	if (tokens[0] == "volMusic:")
		std::istringstream(tokens[1]) >> this->volMusic;
	tokens.clear();
	//read volEffect
	getline(file, line);
	tokens = strsplit(line, ' ');
	if (tokens[0] == "volEffect:")
		std::istringstream(tokens[1]) >> this->volEffect;
	tokens.clear();
	//read keyUp
	getline(file, line);
	tokens = strsplit(line, ' ');
	if (tokens[0] == "keyUp:")
		std::istringstream(tokens[1]) >> this->keyUP;
	tokens.clear();
	//read keyDown
	getline(file, line);
	tokens = strsplit(line, ' ');
	if (tokens[0] == "keyDown:")
		std::istringstream(tokens[1]) >> this->keyDOWN;
	tokens.clear();
	//read keyLeft
	getline(file, line);
	tokens = strsplit(line, ' ');
	if (tokens[0] == "keyLeft:")
		std::istringstream(tokens[1]) >> this->keyLEFT;
	tokens.clear();
	//read keyRight
	getline(file, line);
	tokens = strsplit(line, ' ');
	if (tokens[0] == "keyRight:")
		std::istringstream(tokens[1]) >> this->keyRIGHT;
	tokens.clear();
	//read keyBomb
	getline(file, line);
	tokens = strsplit(line, ' ');
	if (tokens[0] == "keyBomb:")
		std::istringstream(tokens[1]) >> this->keyBOMB;
	tokens.clear();
	//read keyFlashLight
	getline(file, line);
	tokens = strsplit(line, ' ');
	if (tokens[0] == "keyFlash:")
		std::istringstream(tokens[1]) >> this->keyFLASH;
	tokens.clear();
	//read screenX
	getline(file, line);
	tokens = strsplit(line, ' ');
	if (tokens[0] == "screenX:")
		std::istringstream(tokens[1]) >> this->screen_x;
	tokens.clear();
	//read screenY
	getline(file, line);
	tokens = strsplit(line, ' ');
	if (tokens[0] == "screenY:")
		std::istringstream(tokens[1]) >> this->screen_y;
	tokens.clear();
	file.close();
}

void	Game::menuIsVisible( void )
{
	if (this->debug == false)
	{
		camera.moveCamForMenu();
		camera.ProcessMouseMovement(0, -250);
	}
	if (this->menuActive == 26)
	{
		this->Menus[26]->draw();
		if (this->lastFrame - this->pause_died >= 3.0f)
		{
			this->pause_died = this->lastFrame;
			this->menuActive = 0;
		}
	}
	else if (this->menuActive == 28)
	{
		this->Menus[28]->draw();
		if (this->lastFrame - this->you_won >= 3.0f)
		{
			this->you_won = this->lastFrame;
			this->menuActive = 0;
		}
	}
	else
	{
		if (this->soundMenuVisible == true)
		{
			this->soundMenu[this->soundActive]->draw();
			MoveSoundMenu();
		}
		else
		{
			this->Menus[this->menuActive]->draw();
			MoveMenu();
		}
		
		if (keys[GLFW_KEY_ENTER])
		{
			keys[GLFW_KEY_ENTER] = false;
			if (this->menuVisible == true)
			{
				if (this->menuActive == 0)
				{
					this->lives = 3;
					this->loadActive = 0;
					this->menuVisible = false;
					this->loadVisible =true;
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
						menuVisible = false;
						loadVisible =true;
						std::thread *worldThread =  new std::thread(loadGame, this);
						worldThread->detach();
					}
					else
						this->menuActive = 16;
				}
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
						this->menuVisible = false;
						this->pauseVisible = true;
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
					this->writeConfig();
				}
				else if (this->menuActive == 11) //Resolution change
				{
					this->screen_x = 1280;
					this->screen_y = 720;
					glfwSetWindowSize(window, this->screen_x, this->screen_y);
					this->writeConfig();
				}
				else if (this->menuActive == 12) //Resolution change
				{
					this->screen_x = 1920;
					this->screen_y = 1080;
					glfwSetWindowSize(window, this->screen_x, this->screen_y);
					this->writeConfig();
				}
				else if (this->menuActive == 13) //Window Mode
				{
					glfwSetWindowMonitor(window, nullptr, 0, 0, this->screen_x, this->screen_y, GLFW_DONT_CARE);
					keys[GLFW_KEY_ENTER] = true;
				}
				else if (this->menuActive == 14) //Full Screen Mode 
				{
					if (glfwGetWindowMonitor(window) == nullptr)
						this->fullScreenItteration = 0;
					if (this->fullScreenItteration < 2)
					{
						GLFWmonitor *monitor = glfwGetPrimaryMonitor();
						const GLFWvidmode *mode = glfwGetVideoMode(monitor);
						glfwSetWindowMonitor(window, monitor, 0, 0, 2300, 1920, mode->refreshRate);
						glfwSetWindowMonitor(window, monitor, 0, 0, this->screen_x, this->screen_y, mode->refreshRate);
						keys[GLFW_KEY_ENTER] = true;
						this->fullScreenItteration++;
					}
				}
			}
			//--------------------Changing Sound--------------------------------------------------------------------
			else if (this->soundMenuVisible == true)
			{
				if (this->soundActive == 0)
				{
					if (this->volMusic == 0.0f)
						this->soundActive = 3;
					else if (this->volMusic == 0.2f)
						this->soundActive = 4;
					else if (this->volMusic == 0.5f)
						this->soundActive = 5;
					else if(this->volMusic == 1.0f)
						this->soundActive = 6;
				}
				else if (this->soundActive == 1)
				{
					if (this->volEffect == 0.0f)
						this->soundActive = 7;
					else if (this->volEffect == 0.2f)
						this->soundActive = 8;
					else if (this->volEffect == 0.5f)
						this->soundActive = 9;
					else if (this->volEffect == 1.0f)
						this->soundActive = 10;
				}
				else if (this->soundActive == 2)
				{
					this->menuActive = 7;
					this->soundMenuVisible = false;
					this->menuVisible = true;
				}
				else if ((this->soundActive >= 3) && (this->soundActive <= 6))
					this->soundActive = 0;
				else if ((this->soundActive >= 7) && (this->soundActive <= 10))
					this->soundActive = 1;
			}
		}
		//---------------------Selecting Keys---------------------------------------------------------------------------
		else if (this->menuActive == 24 || this->menuActive == 25)
		{
			if (this->menuActive == 25)
			{
				usleep(1000000);
				this->menuActive = last_menu;
			}
			if (keys[lastkeypressed])
			{
				if (!(keys[GLFW_KEY_ENTER] && keys[GLFW_KEY_ESCAPE]))
				{
					tempKEY = lastkeypressed;
					if (this->key_change == 0)
					{
						if ((tempKEY != this->keyDOWN) && (tempKEY != this->keyLEFT) && (tempKEY != this->keyRIGHT) && (tempKEY != this->keyBOMB) && (tempKEY != this->keyFLASH))
						{
							this->keyUP = tempKEY;
							this->menuActive = last_menu;
						}
						else
							this->menuActive = 25;
					}
					if (this->key_change == 1)
					{
						if ((tempKEY != this->keyUP) && (tempKEY != this->keyLEFT) && (tempKEY != this->keyRIGHT) && (tempKEY != this->keyBOMB) && (tempKEY != this->keyFLASH))
						{
							this->keyDOWN = tempKEY;
							this->menuActive = last_menu;
						}
						else
							this->menuActive = 25;
					}
					if (this->key_change == 2)
					{
						if ((tempKEY != this->keyUP) && (tempKEY != this->keyDOWN) && (tempKEY != this->keyRIGHT) && (tempKEY != this->keyBOMB) && (tempKEY != this->keyFLASH))
						{
							this->keyLEFT = tempKEY;
							this->menuActive = last_menu;
						}
						else
							this->menuActive = 25;
					}
					if (this->key_change == 3)
					{
						if ((tempKEY != this->keyUP) && (tempKEY != this->keyDOWN) && (tempKEY != this->keyLEFT) && (tempKEY != this->keyBOMB) && (tempKEY != this->keyFLASH))
						{
							this->keyRIGHT = tempKEY;
							this->menuActive = last_menu;
						}
						else
							this->menuActive = 25;
					}
					if (this->key_change == 4)
					{
						if ((tempKEY != this->keyUP) && (tempKEY != this->keyDOWN) && (tempKEY != this->keyLEFT) && (tempKEY != this->keyRIGHT) && (tempKEY != this->keyFLASH))
						{
							this->keyBOMB = tempKEY;
							this->menuActive = last_menu;
						}
						else
							this->menuActive = 25;
					}
					if (this->key_change == 5)
					{
						if ((tempKEY != this->keyUP) && (tempKEY != this->keyDOWN) && (tempKEY != this->keyLEFT) && (tempKEY != this->keyRIGHT) && (tempKEY != this->keyBOMB))
						{
							this->keyFLASH = tempKEY;
							this->menuActive = last_menu;
						}
						else
							this->menuActive = 25;
					}
					this->writeConfig();
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
						this->volMusic = 0;
						World::sound->setVolMusic(this->volMusic);
						break ;
					case 4:
						this->volMusic = 0.2;
						World::sound->setVolMusic(this->volMusic);
						break ;
					case 5:
						this->volMusic = 0.5;
						World::sound->setVolMusic(this->volMusic);
						break ;
					case 6:
						this->volMusic = 1.0;
						World::sound->setVolMusic(this->volMusic);
						break ;
					case 7:
						this->volEffect = 0;
						World::sound->setVolEffects(this->volEffect);
						break ;
					case 8:
						this->volEffect = 0.2;
						World::sound->setVolEffects(this->volEffect);
						break ;
					case 9:
						this->volEffect = 0.5;
						World::sound->setVolEffects(this->volEffect);
						break ;
					case 10:
						this->volEffect = 1.0;
						World::sound->setVolEffects(this->volEffect);
						break ;
					default:
						break ;
				}
				this->writeConfig();
			}
		}
		usleep(10000);
	}
}

void	Game::loadIsVisible( void )
{
	if (this->debug == false)
	{
		camera.moveCamForMenu();
		camera.ProcessMouseMovement(0, -250);
	}
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
	this->load[this->loadActive]->draw();
	usleep(400000);
	this->loadActive++;
}

void	Game::pauseIsVisible( void )
{
	if (this->debug == false)
	{
		camera.moveCamForMenu();
		camera.ProcessMouseMovement(0, -250);
	}
	this->pauseMenu[this->pauseActive]->draw();
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
					this->pauseVisible = false;
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
							std::istringstream(tokens[1]) >> this->stage;
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
						this->pauseVisible = false;
						this->menuVisible = false;
						this->pauseVisible = false;
						this->loadVisible = true;
						worldThread = new std::thread(loadGame, this);
						worldThread->detach();
					}
					else
						this->pauseActive = 5;
					break ;
				case 3://Options
					this->menuVisible = true;
					this->pauseVisible =false;
					this->check = 1;
					this->menuActive = 6;
					break ;
				case 4://Quit to Main menu
					delete this->world;
					this->menuActive = 0;
					this->menuVisible = true;
					this->pauseVisible = false;
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

void	loadmodels(Game *game)
{
	game->loadmodels1();
}

void	Game::loadmodels1()
{
	// Load models
	for (int i = 0; i < 29; i++)
	{
		mu.lock();
		glfwMakeContextCurrent(window);
		this->Menus.push_back(new MainMenu((*this->shaderNormal), "resources/models/menu/Menu_" + std::to_string(i) + ".obj"));
		glfwMakeContextCurrent(NULL);
		mu.unlock();
	}
	for (int i = 0; i < 9; i++)
	{
		mu.lock();
		glfwMakeContextCurrent(window);
		this->load.push_back(new LoadingScreen((*this->shaderNormal), "resources/models/menu/LoadingScreen/Loading_screen_" + std::to_string(i) + ".obj"));
		glfwMakeContextCurrent(NULL);
		mu.unlock();
	}
	for (int i = 0; i < 11; i++)
	{
		mu.lock();
		glfwMakeContextCurrent(window);
		this->soundMenu.push_back(new SoundMenu((*this->shaderNormal), "resources/models/menu/SoundScreen/SoundScreen_" + std::to_string(i) + ".obj"));
		glfwMakeContextCurrent(NULL);
		mu.unlock();
	}
	for (int i = 0; i < 6; i++)
	{
		mu.lock();
		glfwMakeContextCurrent(window);
		this->pauseMenu.push_back(new PauseMenu((*this->shaderNormal), "resources/models/menu/PauseMenu/pauseMenu_" + std::to_string(i) + ".obj"));
		glfwMakeContextCurrent(NULL);
		mu.unlock();
	}
	this->wtc_logo = false;
}
