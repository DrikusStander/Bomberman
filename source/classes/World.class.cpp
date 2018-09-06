#include "World.class.hpp"

World::World(Shader &shader, std::string model, float screen_x, float screen_y)
{
	std::cout << "World - Parametric Constructor called " << std::endl;
	this->WorldModel =  new Model(model);
	this->hud = HUD(shader, screen_x, screen_y);
	this->_shader = &shader;
	this->x_trans = 0.0f;
	this->y_trans = 0.0f;
	this->z_trans = 0.0f;
	this->player = new Player(shader, "resources/models/player/player_run_");
	this->objects = new std::vector<Item*>();
	this->enemies = new std::vector<Enemy*>();
	this->powerups = new std::vector<Powerup*>();
	this->worldStatus = 0;

	// initiliaze the map
	this->map = new char*[17] ;
	for (int z = 0; z < 17; z++)
	{
		this->map[z] = new char[17];
	}

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
						this->map[i][j] = 'W';
						Item *temp = new Item(shader, "resources/models/wall.obj");
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

	// clean up Powerups
	for (std::vector<Powerup*>::iterator it = this->powerups->begin() ; it != this->powerups->end(); )
	{
		delete (*it);
		it = this->powerups->erase(it);
	}
	delete this->powerups;
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

void World::draw(glm::mat4 matCamera)
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
	this->hud.draw(matCamera);

	for (Enemy *enemy : *this->enemies)
	{
		enemy->draw();
	}

	for (Powerup *powerup : *this->powerups)
	{
		powerup->draw();
	}

	//check what items the bomb affected
	// std::cout << std::endl;
	// std::cout << std::endl;

	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			// std::cout << this->map[i][j] << " " ;
			if (this->map[i][j] == 'D')
			{
				// check if the player was hit
				if (this->player->getRow() == i && this->player->getCol() == j)
				{
					/* 
						handle this event diffrently
						check lives of the player
							-> if lives left respawn
							-> if no lives left Gameover splash screen and send to menu
					*/
					this->worldStatus = 1;
				}

				// check what powerup was affected
				for (std::vector<Powerup*>::iterator it = this->powerups->begin() ; it != this->powerups->end(); )
				{
					if ((*it)->getRow() == i && (*it)->getCol() == j)
					{
						delete (*it);
						it = this->powerups->erase(it);
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
						if ((rand() % 3) == 0)
						{
							int powerupOption = rand() % 3;
							Powerup *temp;
							if (powerupOption == 0)
							{
								temp = new Powerup(*this->_shader, "resources/models/coin/bomb/coin", powerupOption);
							}
							else if (powerupOption == 1)
							{
								temp = new Powerup(*this->_shader, "resources/models/coin/run/coin", powerupOption);
							}
							else
							{
								temp =  new Powerup(*this->_shader, "resources/models/coin/bombs/coin", powerupOption);
							}
							temp->setPos((*it)->getX(), (*it)->getZ(), (*it)->getRow(), (*it)->getCol());
							this->powerups->push_back(temp);
							// std::cout << "placing newly created Powerup at i: " << i << " j: " << j << std::endl;
							this->map[i][j] = 'U';
						}
						std::cout << "delete object" << objCol << std::endl;
						delete (*it);
						it = this->objects->erase(it);
					}
					else
						++it;
				}

				// check what enemy was affected
				for (std::vector<Enemy*>::iterator it = this->enemies->begin() ; it != this->enemies->end(); )
				{
					if ((*it)->getRow() == i && (*it)->getCol() == j)
					{
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
							this->player->handlePowerup((*it)->getType());
							delete (*it);
							it = this->powerups->erase(it);
						}
						else
							++it;
					}
				}
			}
		}
		// std::cout << std::endl;
	}
	
}

void	World::ProcessKeyboard(Direction direction)
{
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
