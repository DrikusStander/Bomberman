#include "SoundMenu.class.hpp"

SoundMenu::SoundMenu(Shader &shader, std::string model)
{
	this->_shader = &shader;
	this->menu = new Model(model);
	this->x_trans = 0.0f;
	this->z_trans = 145.0f;
	this->y_trans = 200.0f;
	this->rotate = 30.0f;
}

SoundMenu::SoundMenu( SoundMenu const & src)
{
	*this = src;
}

SoundMenu::~SoundMenu(void)
{
	delete this->menu;
}

SoundMenu const & SoundMenu::operator=(SoundMenu const & rhs)
{
	this->menu = rhs.menu;
	return(*this);
}

void SoundMenu::draw(void)
{
	glm::mat4 model(1);
	model = glm::translate(model, glm::vec3(this->x_trans, this->y_trans, this->z_trans)); 	// Translate item
	model = glm::scale(model, glm::vec3(15.0f, 10.0f, 12.0f));									// scale item
	model = glm::rotate(model, glm::radians(this->rotate), glm::vec3(1, 0, 0)); 			// where x, y, z is axis of rotation (e.g. 0 1 0)
	glUniformMatrix4fv(glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	this->menu->Draw(*this->_shader);
}
