
#include "Bomb.class.hpp"


Bomb::Bomb( Shader &shader, std::string model) : Item(shader, model)
{
	std::cout << "Bomb - Parametric Constructor called " << std::endl;
	this->PlantTime = 0.0f;
	this->PlantTimeLength = 3.0f;
	this->ExplodeTimeLength = 1.0f;
	this->blastRaduis = 1;
	this->activeFlames = 0;
	this->destruction = false;
	for (int i = 0; i < 12; i++)
	{
		this->flames.push_back(new Item(shader, "resources/models/fire/fire.obj"));
	}
}

Bomb::Bomb( Bomb const & src) : Item(src)
{
	*this = src;
}

Bomb::~Bomb( void )
{
	std::cout << "Bomb - Destructor called " << std::endl;
}

Bomb const & Bomb::operator=(Bomb const & rhs)
{
	this->PlantTime = rhs.PlantTime;
	this->PlantTimeLength = rhs.PlantTimeLength;
	return(*this);
}

void	Bomb::checkDestruction()
{
	//check down
	for (int i = 1; i <= this->blastRaduis; i++)
	{
		// std::cout << "Checking bomb Down " << this->row + i << std::endl;
		if (this->row + i < 17)
		{
			if (this->map[this->row + i][this->col] == '#')
				break;
			if (this->map[this->row + i][this->col] != '\0' && this->map[this->row + i][this->col] != '#')
			{
				if (this->map[this->row + i][this->col] == 'W')
				{
					//do nothing
				}
				else 
					this->map[this->row + i][this->col] = 'D';
				
			}
			if (this->map[this->row + i][this->col] != 'U')
			{
				int tempX = -168 - (this->row + i) *  -21;
				int tempZ = -168 - (this->col) *  -21;
				this->flames[this->activeFlames]->setPos(tempZ, tempX, this->row + i, this->col);
				this->activeFlames++;
				if (this->map[this->row + i][this->col] == 'W')
				{
					this->map[this->row + i][this->col] = 'D';
					break;
				}
			}
		}
	}
	//check up
	for (int i = 1; i <= this->blastRaduis; i++)
	{
		// std::cout << "Checking bomb Up " << this->row  - i << std::endl;
		if (this->row - i > -1)
		{
			if (this->map[this->row - i][this->col] == '#')
				break;
			if (this->map[this->row - i][this->col] != '\0' && this->map[this->row - i][this->col] != '#')
			{
				if (this->map[this->row - i][this->col] == 'W')
				{
					//do nothing
				}
				else
					this->map[this->row - i][this->col] = 'D';
				
			}
			if (this->map[this->row - i][this->col] != 'U')
			{
				int tempX = -168 - (this->row - i) *  -21;
				int tempZ = -168 - (this->col) *  -21;
				this->flames[this->activeFlames]->setPos(tempZ, tempX, this->row - i, this->col);
				this->activeFlames++;
				if (this->map[this->row - i][this->col] == 'W')
				{
					this->map[this->row - i][this->col] = 'D';
					break;
				}
			}
		}
	}
	//check right
	for (int i = 1; i <= this->blastRaduis; i++)
	{
		// std::cout << "Checking bomb Right " << this->col  + i << std::endl;
		if (this->col + i < 17)
		{
			if (this->map[this->row][this->col + i] == '#')
				break;
			if (this->map[this->row][this->col  + i] != '\0' && this->map[this->row][this->col + i] != '#')
			{
				if (this->map[this->row][this->col + i] == 'W')
				{
					//do nothing
				}
				else
					this->map[this->row][this->col + i] = 'D';
				
			}
			if (this->map[this->row][this->col + i] != 'U')
			{
				int tempX = -168 - (this->row) *  -21;
				int tempZ = -168 - (this->col + i) *  -21;
				this->flames[this->activeFlames]->setPos(tempZ, tempX, this->row, this->col + i);
				this->activeFlames++;
				if (this->map[this->row][this->col + i] == 'W')
				{
					this->map[this->row][this->col + i] = 'D';
					break;
				}
			}
		}
	}
	//check left
	for (int i = 1; i <= this->blastRaduis; i++)
	{
		// std::cout << "Checking bomb Left " << this->col - i << std::endl;
		if (this->col - i > -1)
		{
			if (this->map[this->row][this->col - i] == '#')
				break;
			if (this->map[this->row][this->col - i] != '\0' && this->map[this->row][this->col - i] != '#')
			{
				if (this->map[this->row][this->col - i] == 'W')
				{
					//do nothing
				}
				else
					this->map[this->row][this->col - i] = 'D';
			}
			if (this->map[this->row][this->col - i] != 'U')
			{
				int tempX = -168 - (this->row) *  -21;
				int tempZ = -168 - (this->col - i) *  -21;
				this->flames[this->activeFlames]->setPos(tempZ, tempX, this->row, this->col - i);
				this->activeFlames++;
				if (this->map[this->row][this->col - i] == 'W')
				{
					this->map[this->row][this->col - i] = 'D';
					break;
				}
			}
		}
	}
}

void Bomb::draw(void)
{
	if ((glfwGetTime() - this->PlantTime) >= this->PlantTimeLength)
	{
		if (this->destruction == false)
		{
			this->destruction = true;
			this->checkDestruction();
		}
		for (int i = 0; i < this->activeFlames; i++)
		{
			if (this->map[this->flames[i]->getRow()][this->flames[i]->getCol()] != 'U')
			{
				this->map[this->flames[i]->getRow()][this->flames[i]->getCol()] = 'D';
				this->flames[i]->draw();
			}
		}
		if ((glfwGetTime() - this->PlantTime) >= this->ExplodeTimeLength + this->PlantTimeLength)
		{
			this->setActive(false);
			this->destruction = false;
			this->activeFlames = 0;
		}
		this->map[this->row][this->col] = 'D';
	}
	else
	{
		glm::mat4 model(1);
		model = glm::translate( model, glm::vec3(this->x_trans, this->y_trans, this->z_trans));		// Translate item
		model = glm::scale(model, glm::vec3(4.25f, 2.25f, 4.25f));									// scale item
		model = glm::rotate(model, glm::radians(this->rotate), glm::vec3(0, 1, 0));					// where x, y, z is axis of rotation (e.g. 0 1 0)
		glUniformMatrix4fv( glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model ));
		this->ItemModel->Draw(*this->_shader);
	}
}

void	Bomb::setActive(bool active)
{
	if (active == true)
		this->PlantTime = glfwGetTime();
	this->map[this->row][this->col] = 'B';
	this->active = active;
}

void	Bomb::incBlastRaduis()
{
	if (this->blastRaduis < 3)
		this->blastRaduis++;
}