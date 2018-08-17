
#include "Bomb.class.hpp"


Bomb::Bomb( Shader &shader, std::string model) : Item(shader, model)
{
	std::cout << "Bomb - Parametric Constructor called " << std::endl;
	// this->PlantTime = 0.0f;
	this->PlantTimeLength = 3.0f;
}

// Bomb( Bomb const & src);
Bomb::~Bomb( void )
{
	std::cout << "Bomb - Destructor called " << std::endl;
}

// Bomb const & operator=(Bomb const & rhs)
// {

// 	return(*this);
// }

void Bomb::draw(void)
{
	if ((glfwGetTime() - this->PlantTime) >= this->PlantTimeLength)
		this->setActive(false);
	glm::mat4 model(1);
	model = glm::translate( model, glm::vec3(this->x_trans, this->y_trans, this->z_trans));		// Translate it down a bit so it's at the center of the scene
	model = glm::scale(model, glm::vec3(4.25f, 2.25f, 4.25f));									// It's a bit too big for our scene, so scale it down
	model = glm::rotate(model, glm::radians(this->rotate), glm::vec3(0, 1, 0));					// where x, y, z is axis of rotation (e.g. 0 1 0)
	glUniformMatrix4fv( glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model ));
	this->ItemModel->Draw(*this->_shader);
}

void	Bomb::setActive(bool active)
{
	if (active == true)
		this->PlantTime = glfwGetTime();
	this->active = active;
}
