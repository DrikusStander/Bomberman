
#include "Character.class.hpp"


Character::Character( Shader &shader)
{
	std::cout << "Character - Parametric Constructor called " << std::endl;
	this->characterModel =  new Model("resources/models/nanosuit.obj");
	this->_shader = &shader;
	this->x_trans = 0.0f;
	this->y_trans = 18.25f;
	this->z_trans = 0.0f;
	this->rotate = 180.0f;
}

// Character( Character const & src);
Character::~Character( void )
{
	std::cout << "Character - Destructor called " << std::endl;
}
// Character const & operator=(Character const & rhs);



void Character::draw(void)
{
	glm::mat4 model(1);
	model = glm::translate( model, glm::vec3(this->x_trans, this->y_trans, this->z_trans)); 	// Translate it down a bit so it's at the center of the scene
	model = glm::scale( model, glm::vec3(1.25f, 1.25f, 1.25f));									// It's a bit too big for our scene, so scale it down
	model = glm::rotate(model, glm::radians(this->rotate), glm::vec3(0, 1, 0)); 				// where x, y, z is axis of rotation (e.g. 0 1 0)
	glUniformMatrix4fv( glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model ));
	this->characterModel->Draw(*this->_shader);
}
