#include "Powerup.class.hpp"

Powerup::Powerup( Shader &shader, std::string model, int type, std::vector<Model*> *modelArr) : Item(shader, model + "0.obj")
{
	std::cout << "Powerup - Parametric Constructor called " << std::endl;
	this->itemModelarr = modelArr;
	this->_shader = &shader;
	this->x_trans = 0.0f;
	this->y_trans = 20.0f;
	this->z_trans = 0.0f;
	this->rotate = 180.0f;
	this->active = false;
	this->activeFrame = 0;
	this->powerupType = type;
}

Powerup::Powerup( Powerup const & src) : Item(src)
{
	*this = src;
}

Powerup::~Powerup( void )
{
	std::cout << "Powerup - Destructor called " << std::endl;
}

Powerup const & Powerup::operator=(Powerup const & rhs)
{
	this->itemModelarr =  rhs.itemModelarr;
	this->_shader = rhs._shader;
	this->x_trans = rhs.x_trans;
	this->y_trans = rhs.y_trans;
	this->z_trans = rhs.z_trans;
	this->rotate = rhs.rotate;
	this->active = rhs.active;
	this->powerupType = rhs.powerupType;
	return(*this);
}

void Powerup::draw(void)
{
	if (this->activeFrame == 23)
		this->activeFrame = 0;
	else
		this->activeFrame++;
	glm::mat4 model(1);
	model = glm::translate( model, glm::vec3(this->x_trans, this->y_trans, this->z_trans)); // Translate Powerup
	model = glm::scale(model, glm::vec3(4.25f, 2.25f, 4.25f));								// scale Powerup
	model = glm::rotate(model, glm::radians(this->rotate), glm::vec3(0, 1, 0)); 			// where x, y, z is axis of rotation (e.g. 0 1 0)
	glUniformMatrix4fv( glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model ));
	(*this->itemModelarr)[this->activeFrame]->Draw(*this->_shader);

}

int		Powerup::getType(void)
{
	return(this->powerupType);
}
