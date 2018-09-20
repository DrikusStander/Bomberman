#include "Character.class.hpp"


Character::Character( Shader &shader, std::string model)
{
	std::cout << "Character - Parametric Constructor called " << std::endl;
	this->characterModel =  new Model(model);
	this->_shader = &shader;
	this->x_trans = 0.0f;
	this->y_trans = 20.0f;
	this->z_trans = 0.0f;
	this->rotate = 180.0f;
	this->row = 0;
	this->col = 0;
}

Character::Character( Character const & src)
{
	*this = src;
}

Character::~Character( void )
{
	std::cout << "Character - Destructor called " << std::endl;
	delete this->characterModel;
}

Character const & Character::operator=(Character const & rhs)
{
	this->characterModel = rhs.characterModel;
	this->_shader = rhs._shader;
	this->x_trans = rhs.x_trans;
	this->y_trans = rhs.y_trans;
	this->z_trans = rhs.z_trans;
	this->rotate = rhs.rotate;
	return(*this);
}

void Character::draw(void)
{
	glm::mat4 model(1);
	model = glm::translate( model, glm::vec3(this->x_trans, this->y_trans, this->z_trans)); 	// Translate item
	model = glm::scale( model, glm::vec3(1.25f, 1.25f, 1.25f));									// scale item
	model = glm::rotate(model, glm::radians(this->rotate), glm::vec3(0, 1, 0)); 				// where x, y, z is axis of rotation (e.g. 0 1 0)
	glUniformMatrix4fv( glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model ));
	this->characterModel->Draw(*this->_shader);
}

void Character::setMap( char **map)
{
	this->map = map;
}

void Character::setPos(float x, float z, int row, int col)
{
	this->row = row;
	this->col = col;
	this->x_trans = x;
	this->z_trans = z;
}

int		Character::getCol()
{
	return(this->col);
}

int		Character::getRow()
{
	return(this->row);
}

void	Character::setShader(Shader &shader)
{
	this->_shader = &shader;
}
