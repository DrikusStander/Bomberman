#include "MainMenu.class.hpp"

MainMenu::MainMenu(Shader &shader, std::string model)
{
	std::cout << "MainMenu - Constructor called " << std::endl;
	this->_shader = &shader;
	this->menu = new Model(model);
	this->x_trans = 0.0f;
	this->z_trans = 145.0f;
	this->y_trans = 200.0f;
	this->rotate = 30.0f;
}


MainMenu::MainMenu( MainMenu const & src)
{
	*this = src;
}

MainMenu::~MainMenu(void)
{
	std::cout << "MainMenu - Destructor called " << std::endl;
	delete this->menu;
}

MainMenu const & MainMenu::operator=(MainMenu const & rhs)
{
	this->menu = rhs.menu;
	return(*this);
}

void MainMenu::draw(void)
{
	glm::mat4 model(1);
	model = glm::translate(model, glm::vec3(this->x_trans, this->y_trans, this->z_trans)); 	// Translate item
	model = glm::scale(model, glm::vec3(15.0f, 10.0f, 12.0f));									// scale item
	model = glm::rotate(model, glm::radians(this->rotate), glm::vec3(1, 0, 0)); 			// where x, y, z is axis of rotation (e.g. 0 1 0)
	glUniformMatrix4fv(glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	this->menu->Draw(*this->_shader);
}
