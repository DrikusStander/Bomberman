
#include "World.class.hpp"


World::World( Shader &shader, std::string model)
{
	std::cout << "World - Parametric Constructor called " << std::endl;
	this->WorldModel =  new Model(model);
	this->_shader = &shader;
	this->x_trans = 0.0f;
	this->y_trans = 0.0f;
	this->z_trans = 0.0f;
	this->player = new Player(shader, "resources/models/nanosuit.obj");
	// this->map  = new std::vector<std::vector<Item*>>(17);
	// std::vector< std::vector<Item*> > map(17, std::vector<Item*>(17));
	// for (int i = 0; i < 17; i++)
	// 	(*this->map)[i].resize(17);

	// // (*this->map)[3][0] = new Item(shader, "resources/models/enemy.obj");
	// // std::vector<Item *> row = matrix[0][0];
	// // this->map[0][0].push_back(new Item(shader, "resources/models/bom.obj"));
	// // this->map[0][3].push_back(new Item(shader, "resources/models/nanosuit.obj"));
	// for (int i = 0; i < 17; i++)
	// {
	// 	// std::cout << "row :" << i << " constructing" << std::endl;
	// 	for (int j = 0; j < 17; j++)
	// 	{
	// 		// std::cout << "col :" << j << " constructing" << std::endl;
	// 		if ((i % 2) != 0 && (j % 2) != 0)
	// 		{
	// 			// std::cout << "col :" << j << " placing a pillar" << std::endl;
	// 			(*this->map)[i][j] = new Item(shader, "resources/models/enemy.obj");
	// 		}
	// 		else
	// 			(*this->map)[i][j] = NULL;
	// 	}
	// }


	// this->player_x_pos = 0;
	// this->player_z_pos = 0;
	
	// char tmpArray[17][17]=
	// {
	// 	{'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'},
	// 	{'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0'},
	// 	{'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'},
	// 	{'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0'},
	// 	{'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'},
	// 	{'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0'},
	// 	{'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'},
	// 	{'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0'},
	// 	{'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'},
	// 	{'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0'},
	// 	{'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'},
	// 	{'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0'},
	// 	{'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'},
	// 	{'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0'},
	// 	{'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'},
	// 	{'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0','#' ,'\0'},
	// 	{'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'}
	// };
	this->map = new char*[17] ;//&tmpArray;
	for (int z = 0; z < 17; z++)
	{
		this->map[z] = new char[17];
		// for (int x = 0; x < 17; x++)
		// {
		// 	this->mapArray[z][x] = tmpArray[z][x];
		// }
	}

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
				this->map[i][j] = '\0';
			}
		}
	}
	// this->player->setMap(this->map);

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
	this->player->setMap(this->map);
	if (this->map[0][0] != '\0')
		std::cout << "map[0][0] : " << this->map[0][0] << std::endl;

	glm::mat4 model(1);
	model = glm::translate( model, glm::vec3(this->x_trans, this->y_trans, this->z_trans)); 	// Translate it down a bit so it's at the center of the scene
	model = glm::scale( model, glm::vec3(0.2f, 0.2f, 0.2f));									// It's a bit too big for our scene, so scale it down
	glUniformMatrix4fv( glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr( model ));
	this->WorldModel->Draw(*this->_shader);
	this->player->draw();
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