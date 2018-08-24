#include "Scoring.class.hpp"

Scoring::Scoring(Shader &shader, std::string model)
{
	std::cout << "Scoring - Constructor called " << std::endl;
	this->_shader = &shader;
	this->text = new Model(model);
	this->x_trans = -268.0f;
	this->z_trans = -268.0f;
	this->y_trans = 100.0f;
	this->rotate = 180.0f;
}


Scoring::Scoring( Scoring const & src)
{
	*this = src;
}

Scoring::~Scoring(void)
{
	std::cout << "Scoring - Destructor called " << std::endl;
	delete this->text;
}

Scoring const & Scoring::operator=(Scoring const & rhs)
{
	this->text = rhs.text;
	return(*this);
}

void Scoring::draw(void)
{
	glm::mat4 model(1);
	model = glm::translate(model, glm::vec3(this->x_trans, this->y_trans, this->z_trans)); 	// Translate item
	model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));									// scale item
	model = glm::rotate(model, glm::radians(this->rotate), glm::vec3(0, 1, 0)); 			// where x, y, z is axis of rotation (e.g. 0 1 0)
	glUniformMatrix4fv(glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	this->text->Draw(*this->_shader);
}
