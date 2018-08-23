
#include "World.class.hpp"


World::World( Shader &shader, std::string model)
{
	std::cout << "World - Parametric Constructor called " << std::endl;
	this->active = 0;
	this->WorldModel =  new Model(model);
	this->_shader = &shader;
	this->x_trans = 0.0f;
	this->y_trans = 0.0f;
	this->z_trans = 0.0f;
	this->player[0] = new Player(shader, "resources/models/player/player_run_0.obj");
	this->player[1] = new Player(shader, "resources/models/player/player_run_1.obj");
	this->player[2] = new Player(shader, "resources/models/player/player_run_2.obj");
	this->player[3] = new Player(shader, "resources/models/player/player_run_3.obj");
	this->player[4] = new Player(shader, "resources/models/player/player_run_4.obj");
	this->player[5] = new Player(shader, "resources/models/player/player_run_5.obj");
	this->player[6] = new Player(shader, "resources/models/player/player_run_6.obj");
	this->player[7] = new Player(shader, "resources/models/player/player_run_7.obj");
	this->player[8] = new Player(shader, "resources/models/player/player_run_8.obj");
	this->player[9] = new Player(shader, "resources/models/player/player_run_9.obj");
	this->player[10] = new Player(shader, "resources/models/player/player_run_10.obj");
	this->player[11] = new Player(shader, "resources/models/player/player_run_11.obj");
	this->player[12] = new Player(shader, "resources/models/player/player_run_12.obj");
	this->player[13] = new Player(shader, "resources/models/player/player_run_13.obj");
	this->player[14] = new Player(shader, "resources/models/player/player_run_14.obj");
	this->player[15] = new Player(shader, "resources/models/player/player_run_15.obj");
	this->player[16] = new Player(shader, "resources/models/player/player_run_16.obj");
	this->player[17] = new Player(shader, "resources/models/player/player_run_17.obj");
	this->player[18] = new Player(shader, "resources/models/player/player_run_18.obj");
	this->player[19] = new Player(shader, "resources/models/player/player_run_19.obj");
	this->player[20] = new Player(shader, "resources/models/player/player_run_20.obj");
	this->player[21] = new Player(shader, "resources/models/player/player_run_21.obj");
	this->objects = new std::vector<Item*>();
	this->enemies = new std::vector<Enemy*>();
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
	delete this->player[0];
	delete this->player[1];
	delete this->player[2];
	delete this->player[3];
	delete this->player[4];
	delete this->player[5];
	delete this->player[6];
	delete this->player[7];
	delete this->player[8];
	delete this->player[9];
	delete this->player[10];
	delete this->player[11];
	delete this->player[12];
	delete this->player[13];
	delete this->player[14];
	delete this->player[15];
	delete this->player[16];
	delete this->player[17];
	delete this->player[18];
	delete this->player[19];
	delete this->player[20];
	delete this->player[21];

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

void World::draw(void)
{
	this->player[0]->setMap(this->map);
	this->player[1]->setMap(this->map);
	this->player[2]->setMap(this->map);
	this->player[3]->setMap(this->map);
	this->player[4]->setMap(this->map);
	this->player[5]->setMap(this->map);
	this->player[6]->setMap(this->map);
	this->player[7]->setMap(this->map);
	this->player[8]->setMap(this->map);
	this->player[9]->setMap(this->map);
	this->player[10]->setMap(this->map);
	this->player[11]->setMap(this->map);
	this->player[12]->setMap(this->map);
	this->player[13]->setMap(this->map);
	this->player[14]->setMap(this->map);
	this->player[15]->setMap(this->map);
	this->player[16]->setMap(this->map);
	this->player[17]->setMap(this->map);
	this->player[18]->setMap(this->map);
	this->player[19]->setMap(this->map);
	this->player[20]->setMap(this->map);
	this->player[21]->setMap(this->map);
	if (this->map[0][0] != '\0')
		std::cout << "map[0][0] : " << this->map[0][0] << std::endl;

	glm::mat4 model(1);
	model = glm::translate( model, glm::vec3(this->x_trans, this->y_trans, this->z_trans)); 	// Translate it down a bit so it's at the center of the scene
	model = glm::scale( model, glm::vec3(0.2f, 0.2f, 0.2f));									// It's a bit too big for our scene, so scale it down
	glUniformMatrix4fv( glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr( model ));
	this->WorldModel->Draw(*this->_shader);
	this->player[this->active]->draw();

	//check what items the bomb affected
	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			if (this->map[i][j] == 'D')
			{
				// check if the player was hit
				if (this->player[this->active]->getRow() == i && this->player[this->active]->getCol() == j)
				{
					/* 
						handle this event diffrently
						check lives of the player 
							-> if lives left respawn
							-> if no lives left lose splash screen and send to menu
					*/
					this->worldStatus = 1;
				}

				// check what object was affected
				for (std::vector<Item*>::iterator it = this->objects->begin() ; it != this->objects->end(); )
				{
					if ((*it)->getRow() == i && (*it)->getCol() == j)
					{
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
				this->map[i][j] = '\0';
			}
		}
	}
	for (Enemy *enemy : *this->enemies)
	{
		enemy->draw();
	}
	for (Item *item : *this->objects)
	{
		item->draw();
	}
}

void	World::ProcessKeyboard(Direction direction)
{
	this->player[this->active]->ProcessKeyboard(direction);
	// if (this->active < 21)
	// 	this->active++;
	// else if (this->active == 21)
	// 	this->active = 0;
}

float	World::player_getX(void)
{
	return this->player[this->active-1]->getX();
}

float	World::player_getZ(void)
{
	return this->player[this->active-1]->getZ();
}

void	World::player_clipX(float x_move)
{
	this->player[this->active]->clipX(x_move);
}

void	World::player_clipZ(float z_move)
{
	this->player[this->active]->clipZ(z_move);
}

int		World::getStatus( void )
{
	return (this->worldStatus);
}
