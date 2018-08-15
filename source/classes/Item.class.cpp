
#include "Item.class.hpp"


Item::Item( Shader &shader)
{
	std::cout << "Item - Parametric Constructor called " << std::endl;
	this->ItemModel =  new Model("resources/models/bom.obj");
	this->_shader = &shader;
	this->x_trans = 0.0f;
	this->y_trans = 18.25f;
	this->z_trans = 0.0f;
	this->rotate = 180.0f;
	this->active = false;
	this->PlantTimeLength = 3.0f;
}

// Item( Item const & src);
Item::~Item( void )
{
	std::cout << "Item - Destructor called " << std::endl;
}

// Item const & operator=(Item const & rhs)
// {

// 	return(*this);
// }

void Item::draw(void)
{
	if ((glfwGetTime() - this->PlantTime) >= this->PlantTimeLength)
		this->setActive(false);
	glm::mat4 model(1);
	model = glm::translate( model, glm::vec3(this->x_trans, this->y_trans, this->z_trans)); // Translate it down a bit so it's at the center of the scene
	model = glm::scale(model, glm::vec3(4.25f, 2.25f, 4.25f));	// It's a bit too big for our scene, so scale it down
	model = glm::rotate(model, glm::radians(this->rotate), glm::vec3(0, 1, 0)); // where x, y, z is axis of rotation (e.g. 0 1 0)
	glUniformMatrix4fv( glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model ));
	this->ItemModel->Draw(*this->_shader);
}

void Item::setPos(float x, float z)
{
	this->x_trans = x;
	this->z_trans = z;
}

void	Item::setActive(bool active)
{
	if (active == true)
		this->PlantTime = glfwGetTime();
	this->active = active;
}

bool	Item::getActive()
{
	return(this->active);
}
