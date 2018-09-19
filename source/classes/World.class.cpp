#include "World.class.hpp"
std::mutex mu;

World::World(Shader &shader, std::string model, float screen_x, float screen_y, GLFWwindow	*window)
{
	std::cout << "World - Parametric Constructor called " << std::endl;
mu.lock();
	glfwMakeContextCurrent(window);	
	this->WorldModel =  new Model(model);
	glfwMakeContextCurrent(NULL);
mu.unlock();

mu.lock();
	glfwMakeContextCurrent(window);	
	this->hud = HUD(shader, screen_x, screen_y);
	glfwMakeContextCurrent(NULL);
mu.unlock();

mu.lock();
	glfwMakeContextCurrent(window);	
	std::cout << "World: setting vars" << std::endl;
	this->_shader = &shader;
	this->x_trans = 0.0f;
	this->y_trans = 0.0f;
	this->z_trans = 0.0f;
	this->score = 0;
	this->time = 200;
	timeSinceNewFrame = 0.0f;
	this->wallCount = 0;
	this->enemyCount = 0;
	this->portalActive = false;
	this->player = new Player(shader, "resources/models/player/player_run_");
	this->lives = this->player->getLives();
	glfwMakeContextCurrent(NULL);
mu.unlock();

mu.lock();
	glfwMakeContextCurrent(window);	
	std::cout << "World: creating vectors" << std::endl;
	this->objects = new std::vector<Item*>();
	this->enemies = new std::vector<Enemy*>();
	this->powerups = new std::vector<Powerup*>();
	this->worldStatus = 0;

	this->bombRaduis_index = 0;
	this->bombCount_index = 0;
	this->speed_index = 0;

	// create breakable wall model
	this->wall_model = new Model("resources/models/wall.obj");
	glfwMakeContextCurrent(NULL);
mu.unlock();

mu.lock();
	glfwMakeContextCurrent(window);	
	// create coin models vectors
	std::cout << "World: creating Models Vectors" << std::endl;
	this->bombRaduis_model = new std::vector<Model*>();
	this->bombCount_model = new std::vector<Model*>();
	this->speed_model = new std::vector<Model*>();
	this->portal_model = new std::vector<Model*>();
	glfwMakeContextCurrent(NULL);
mu.unlock();

mu.lock();
	std::cout << "World: creating adding Models to bombRaduis Vector" << std::endl;
	glfwMakeContextCurrent(window);	
	// add moels to coin models vectors
	for (int i = 0; i < 24; i++)
		this->bombRaduis_model->push_back(new Model("resources/models/coin/bomb/coin" + std::to_string(i) + ".obj"));
	glfwMakeContextCurrent(NULL);
mu.unlock();

mu.lock();
	std::cout << "World: adding Models to BombCount Vector" << std::endl;
	glfwMakeContextCurrent(window);	
	for (int i = 0; i < 24; i++)
		this->bombCount_model->push_back(new Model("resources/models/coin/bombs/coin" + std::to_string(i) + ".obj"));
	glfwMakeContextCurrent(NULL);
mu.unlock();

mu.lock();
	std::cout << "World: adding Models to Speed Vector" << std::endl;
	glfwMakeContextCurrent(window);	
	for (int i = 0; i < 24; i++)
		this->speed_model->push_back(new Model("resources/models/coin/run/coin" + std::to_string(i) + ".obj"));
	glfwMakeContextCurrent(NULL);
mu.unlock();

mu.lock();
	std::cout << "World: adding Models to portal Vector" << std::endl;
	glfwMakeContextCurrent(window);
	// set models for portal
	for (int i = 0; i < 1; i++)
		this->portal_model->push_back(new Model("resources/models/portal/portal" + std::to_string(i) + ".obj"));
	glfwMakeContextCurrent(NULL);
mu.unlock();

mu.lock();
	std::cout << "World: adding new intital Powerups to their Vectors" << std::endl;
	glfwMakeContextCurrent(window);	
	// set initial powerups

	for (int i = 0; i < 3 ; i++)
	{
		this->speed.push_back( new Powerup(*this->_shader, "resources/models/coin/run/coin", 1, this->speed_model) );
		this->bombCount.push_back( new Powerup(*this->_shader, "resources/models/coin/bombs/coin", 2, this->bombCount_model) );
		this->bombRaduis.push_back( new Powerup(*this->_shader, "resources/models/coin/bomb/coin", 0, this->bombRaduis_model) );
	}
	this->portal = new Powerup(*this->_shader, "resources/models/portal/portal", 3, this->portal_model);
	std::cout << "World powerups initialized" << std::endl;

	glfwMakeContextCurrent(NULL);
mu.unlock();

mu.lock();
	std::cout << "World: Initializing Map" << std::endl;
	glfwMakeContextCurrent(window);	
	// initiliaze the map
	this->map = new char*[17] ;
	for (int z = 0; z < 17; z++)
	{
		this->map[z] = new char[17];
	}

	std::cout << "World map initialized" << std::endl;
	glfwMakeContextCurrent(NULL);
mu.unlock();

mu.lock();
	glfwMakeContextCurrent(window);	
	std::cout << "World: Initializing Breakable walls on Map" << std::endl;
	// randomly innitilize breakable walls to the world
	std::srand(std::time(NULL));
	for (int i = 0; i < 17; i++)
	{
		for(int j = 0; j < 17; j++)
		{
			if ((i % 2) != 0 && (j % 2) != 0)
			{
				this->map[i][j] = '#';
			}
			else
			{
				if (i < 2 && j <  2)
					this->map[i][j] = '\0';
				else
				{
					if ((std::rand() % 3 + 1) == 1)
					{
						this->wallCount++;
						this->map[i][j] = 'W';
						Item *temp = new Item(shader, this->wall_model);

						float x_transT = ((-168) - (j) * (-21));
						float z_transT = ((-168) - (i) * (-21));
						temp->setPos( x_transT, z_transT, i, j);
						this->objects->push_back(temp);
					}
					else
						this->map[i][j] = '\0';
				}
			}
		}
	}
	std::cout << "World walls initialized" << std::endl;

	glfwMakeContextCurrent(NULL);
mu.unlock();

mu.lock();
	glfwMakeContextCurrent(window);	

	std::cout << "World: Initializing Enemies on Map" << std::endl;
	// Initialize Enemies into the world
	int enemy_count = 5;
	while (enemy_count > 0)
	{
		int row = (rand() % 16);
		int col = (rand() % 16);
		if ( (row > 2) && (col > 2))
		{
			if (this->map[row][col] == '\0')
			{
				this->enemyCount++;
				this->map[row][col] = 'E';
				float x_transT = ((-168) - (col) * (-21));
				float z_transT = ((-168) - (row) * (-21));

				Enemy *temp = new Enemy(shader, "resources/models/enemy.obj");

				temp->setPos(x_transT, z_transT, row, col);
				temp->setMap(this->map);
				this->enemies->push_back(temp);
				enemy_count--;
			}
		}

	}
	std::cout << "World enemies initialized" << std::endl;
	glfwMakeContextCurrent(NULL);
mu.unlock();

}

World::World( World const & src)
{
	*this = src;
}

World::~World( void )
{
	std::cout << "World - Destructor called " << std::endl;
	delete this->WorldModel;
	delete this->player;
	delete this->wall_model;
	
	// clean up map
	for (int z = 0; z < 17; z++)
	{
		delete [] this->map[z];
	}
	delete [] this->map;

	
	// clean up objects
	for (std::vector<Item*>::iterator it = this->objects->begin() ; it != this->objects->end(); )
	{
		if (it != this->objects->end())
		{
			delete (*it);
			it = this->objects->erase(it);
		}
	}
	delete this->objects;

	// clean up enemies
	for (std::vector<Enemy*>::iterator it = this->enemies->begin() ; it != this->enemies->end(); )
	{
		delete (*it);
		it = this->enemies->erase(it);
	}
	delete this->enemies;

	// // clean up Powerups
	delete this->powerups;

	// clean up bombRaduis Powerups
	std::cout << "cleaning Bombraduis vector" << std::endl;
	for (std::vector<Powerup*>::iterator it = this->bombRaduis.begin() ; it != this->bombRaduis.end(); )
	{
		delete (*it);
		it = this->bombRaduis.erase(it);
	}
	// clean up bombRaduis_model Powerup Models
	for (std::vector<Model*>::iterator it = this->bombRaduis_model->begin() ; it != this->bombRaduis_model->end(); )
	{
		delete (*it);
		it = this->bombRaduis_model->erase(it);
	}
	delete this->bombRaduis_model;

	// clean up bombCount Powerups
	std::cout << "cleaning BomCount vector" << std::endl;
	for (std::vector<Powerup*>::iterator it = this->bombCount.begin() ; it != this->bombCount.end(); )
	{
		delete (*it);
		it = this->bombCount.erase(it);
	}
	// clean up bombCount_model Powerup Models
	for (std::vector<Model*>::iterator it = this->bombCount_model->begin() ; it != this->bombCount_model->end(); )
	{
		delete (*it);
		it = this->bombCount_model->erase(it);
	}
	delete this->bombCount_model;

	// clean up speed Powerups
	std::cout << "cleaning speed vector" << std::endl;
	for (std::vector<Powerup*>::iterator it = this->speed.begin() ; it != this->speed.end(); )
	{
		delete (*it);
		it = this->speed.erase(it);
	}
	// clean up speed Powerup Models
	for (std::vector<Model*>::iterator it = this->speed_model->begin() ; it != this->speed_model->end(); )
	{
		delete (*it);
		it = this->speed_model->erase(it);
	}
	delete this->speed_model;

}

World const & World::operator=(World const & rhs)
{
	this->WorldModel = rhs.WorldModel;
	this->_shader = rhs._shader;
	this->x_trans = rhs.x_trans;
	this->y_trans = rhs.y_trans;
	this->z_trans = rhs.z_trans;
	return(*this);
}

void World::draw(glm::mat4 matCamera, const GLfloat glfwTime)
{
	this->player->setMap(this->map);
	glm::mat4 model(1);
	model = glm::translate( model, glm::vec3(this->x_trans, this->y_trans, this->z_trans)); 	// Translate it down a bit so it's at the center of the scene
	model = glm::scale( model, glm::vec3(0.2f, 0.2f, 0.2f));									// It's a bit too big for our scene, so scale it down
	glUniformMatrix4fv( glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr( model ));
	this->WorldModel->Draw(*this->_shader);
	for (Item *item : *this->objects)
	{
		item->draw();
	}
	this->player->draw();
	if (glfwTime - this->timeSinceNewFrame >= 1.0f)
	{
		this->timeSinceNewFrame = glfwTime;
		if (this->time > 0)
			this->time--;
		else
			this->worldStatus = 1;

	}

	this->hud.draw(matCamera, this->time, this->score, this->lives);

	for (Enemy *enemy : *this->enemies)
	{
		enemy->draw();
	}

	for (Powerup *powerup : *this->powerups)
	{
		powerup->draw();
	}

	//check what items the bomb affected
	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			if (this->map[i][j] == 'D')
			{
				// check if the player was hit
				if (this->player->getRow() == i && this->player->getCol() == j && this->player->getInvincible() == false)
				{
					/*
						handle this event diffrently
						check lives of the player
							-> if lives left respawn
							-> if no lives left Gameover splash screen and send to menu
					*/
					this->lives = this->player->subLife();
					this->player->setPos(-168, -168, 0, 0);
					// usleep(3000000);
					if (this->player->getLives() == 0)
						this->worldStatus = 1;
				}

				// check what powerup was affected
				for (std::vector<Powerup*>::iterator it = this->powerups->begin() ; it != this->powerups->end(); )
				{
					if ((*it)->getRow() == i && (*it)->getCol() == j)
					{
						if ((*it)->getType() != 3)
						{
							delete (*it);
							it = this->powerups->erase(it);
						}
						else
						{
							++it;
							this->map[i][j] = 'U';
						}
					}
					else
						++it;
				}

				// check what object was affected
				for (std::vector<Item*>::iterator it = this->objects->begin() ; it != this->objects->end(); )
				{
					int objRow = (*it)->getRow();
					int objCol = (*it)->getCol();
					if ((*it)->getRow() == i && (*it)->getCol() == j)
					{
						// generate a powerup based on random chance
						// if ((rand() % 2 + 1) == 1 && this->portalActive == false)
						if ((rand() % this->wallCount) == 0 && this->portalActive == false)
						{
							this->portalActive = true;
							std::cout << "-----------> portal " << std::endl;
							this->portal->setPos((*it)->getX(), (*it)->getZ(), (*it)->getRow(), (*it)->getCol());
							this->map[i][j] = 'U';
							this->powerups->push_back(this->portal);
						}
						else if ((rand() % 3) == 0)
						{
							int powerupOption = rand() % 3;
							Powerup *temp;
							if (powerupOption == 0)
							{
								if (this->bombRaduis_index < 3)
								{
									temp = this->bombRaduis[this->bombRaduis_index];
									this->bombRaduis_index++; 
								}
							}
							else if (powerupOption == 1)
							{
								if (this->speed_index < 3)
								{
									temp = this->speed[this->speed_index];
									this->speed_index++; 

								}
							}
							else if (powerupOption == 2)
							{
								if (this->bombCount_index < 3)
								{
									temp = this->bombCount[this->bombCount_index];
									this->bombCount_index++; 
								}
							}
							else 
							{
								temp = NULL;
							}
							if (temp != NULL)
							{
								temp->setPos((*it)->getX(), (*it)->getZ(), (*it)->getRow(), (*it)->getCol());
								this->powerups->push_back(temp);
								this->map[i][j] = 'U';
							}
						}
						this->wallCount--;
						std::cout << "delete object" << objCol << std::endl;
						delete (*it);
						it = this->objects->erase(it);
					}
					else
						++it;
				}
				// std::cout << "end of Oject Check" << std::endl;

				// check what enemy was affected
				for (std::vector<Enemy*>::iterator it = this->enemies->begin() ; it != this->enemies->end(); )
				{
					if ((*it)->getRow() == i && (*it)->getCol() == j)
					{
						this->enemyCount--;
						this->score += 100;
						delete (*it);
						it = this->enemies->erase(it);
					}
					else
						++it;
				}
				if (this->map[i][j] == 'D')
					this->map[i][j] = '\0';
			}
			if (this->map[i][j] == 'U')
			{
				// check if the player is on powerup
				if (this->player->getRow() == i && this->player->getCol() == j)
				{
					for (std::vector<Powerup*>::iterator it = this->powerups->begin() ; it != this->powerups->end(); )
					{
						if ((*it)->getRow() == i && (*it)->getCol() == j)
						{
							int type = (*it)->getType();
							this->player->handlePowerup(type);
							switch(type)
							{
								case 0:
									it = this->powerups->erase(it);
									this->bombRaduis_index--;
									break;
								case 1:
									it = this->powerups->erase(it);
									this->speed_index--;
									break;
								case 2:
									it = this->powerups->erase(it);
									this->bombCount_index--;
									break;
								case 3:
								{
									if (this->enemyCount == 0)
									{
										/*
											also load next level here
										*/
											it = this->powerups->erase(it);
											this->worldStatus = 2;
									}
									else
										++it;
									break;
								}
								default:
									break;
							}
							// delete (*it);
						}
						else
							++it;
					}
				}

			}
		}
		// std::cout << std::endl;
	}
	// std::cout << "end of world Draw" << std::endl;
}

void	World::ProcessKeyboard(Direction direction, Camera &camera)
{
	camera.setPos(glm::vec3(this->player->getX(), 35.0f, this->player->getZ()));
	float	yaw = camera.getYaw();
	if (direction == FWD)
	{
		if ((yaw >= 315.0f) && (yaw <= 45.0f))
			direction = FWD;
		else if ((yaw >= 135.0f) && (yaw <= 225.0f))
			direction = BKW;
		else if ((yaw > 45.0f) && (yaw < 135.0f))
			direction = RGT;
		else if ((yaw > 225.0f) && (yaw < 315.0f))
			direction = LFT;
	}
	else if (direction == BKW)
	{
		if ((yaw >= 315.0f) && (yaw <= 45.0f))
			direction = BKW;
		else if ((yaw >= 135.0f) && (yaw <= 225.0f))
			direction = FWD;
		else if ((yaw > 45.0f) && (yaw < 135.0f))
			direction = LFT;
		else if ((yaw > 225.0f) && (yaw < 315.0f))
			direction = RGT;
	}
	else if (direction == LFT)
	{
		if ((yaw >= 315.0f) && (yaw <= 45.0f))
			direction = LFT;
		else if ((yaw >= 135.0f) && (yaw <= 225.0f))
			direction = RGT;
		else if ((yaw > 45.0f) && (yaw < 135.0f))
			direction = FWD;
		else if ((yaw > 225.0f) && (yaw < 315.0f))
			direction = BKW;
	}
	else if (direction == RGT)
	{
		if ((yaw >= 315.0f) && (yaw <= 45.0f))
			direction = RGT;
		else if ((yaw >= 135.0f) && (yaw <= 225.0f))
			direction = LFT;
		else if ((yaw > 45.0f) && (yaw < 135.0f))
			direction = BKW;
		else if ((yaw > 225.0f) && (yaw < 315.0f))
			direction = FWD;
	}
	this->player->ProcessKeyboard(direction);
}

float	World::player_getX(void)
{
	return this->player->getX();
}

float	World::player_getZ(void)
{
	return this->player->getZ();
}

void	World::player_clipX(float x_move)
{
	this->player->clipX(x_move);
}

void	World::player_clipZ(float z_move)
{
	this->player->clipZ(z_move);
}

int		World::getStatus( void )
{
	return (this->worldStatus);
}

int		World::getScore( void )
{
	return(this->score);
}

int		World::getLives( void )
{
	return(this->lives);
}