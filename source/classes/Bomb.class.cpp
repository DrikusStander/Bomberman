
#include "Bomb.class.hpp"


Bomb::Bomb( Shader &shader, std::string model) : Item(shader, model)
{
	std::cout << "Bomb - Parametric Constructor called " << std::endl;
	this->PlantTime = 0.0f;
	this->PlantTimeLength = 3.0f;
	this->blastRaduis = 1;
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

void Bomb::draw(void)
{
	if ((glfwGetTime() - this->PlantTime) >= this->PlantTimeLength)
	{
		if (this->row + this->blastRaduis < 17 && (this->map[this->row + this->blastRaduis][this->col] != '\0' && this->map[this->row + this->blastRaduis][this->col] != '#'))
		{
			this->map[this->row + this->blastRaduis][this->col] = 'D';
		}
		if (this->row - this->blastRaduis > -1 && (this->map[this->row - this->blastRaduis][this->col] != '\0' && this->map[this->row - this->blastRaduis][this->col] != '#'))
		{
			this->map[this->row - this->blastRaduis][this->col] = 'D';
		}
		if (this->col + this->blastRaduis < 17 && (this->map[this->row][this->col + this->blastRaduis] != '\0' && this->map[this->row][this->col + this->blastRaduis] != '#') )
		{
			this->map[this->row][this->col + this->blastRaduis] = 'D';
		}
		if (this->col - this->blastRaduis > -1 && (this->map[this->row][this->col - this->blastRaduis] != '\0' && this->map[this->row][this->col - this->blastRaduis] != '#' ))
		{
			this->map[this->row][this->col - this->blastRaduis] = 'D';
		}
		this->setActive(false);
		this->map[this->row][this->col] = 'D';
	}

	glm::mat4 model(1);
	model = glm::translate( model, glm::vec3(this->x_trans, this->y_trans, this->z_trans));		// Translate item
	model = glm::scale(model, glm::vec3(4.25f, 2.25f, 4.25f));									// scale item
	model = glm::rotate(model, glm::radians(this->rotate), glm::vec3(0, 1, 0));					// where x, y, z is axis of rotation (e.g. 0 1 0)
	glUniformMatrix4fv( glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model ));
	this->ItemModel->Draw(*this->_shader);
}

void	Bomb::setActive(bool active)
{
	if (active == true)
		this->PlantTime = glfwGetTime();
	this->map[this->row][this->col] = 'B';
	this->active = active;
}
