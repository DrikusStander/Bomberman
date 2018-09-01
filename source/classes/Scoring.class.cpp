#include "Scoring.class.hpp"

Scoring::Scoring(Shader &shader, std::string model)
{
	std::cout << "Scoring - Constructor called " << std::endl;
	this->_shader = &shader;
	this->text = new Model(model);
	this->x_trans = 0.0f;
	this->z_trans = -400.0f;
	this->y_trans = -100.0f;
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

void Scoring::draw(Camera &camera)
{
	// glm::vec3	front;

	// front.x = cos(glm::radians(camera.getYaw())) * cos(glm::radians(camera.getPitch()));
	// front.y = sin(glm::radians(camera.getPitch()));
	// front.z = sin(glm::radians(camera.getYaw())) * cos(glm::radians(camera.getPitch()));
	// std::cout << "X: " << std::to_string(camera.getUp().x) << " | Y: " << std::to_string(camera.getUp().y) << " | Z: " << std::to_string(camera.getUp().z) << std::endl;
	glm::mat4 model = camera.GetViewMatrix();
	model = glm::translate(model, glm::vec3(this->x_trans + (camera.getPossition().x * 2), this->y_trans + (camera.getPossition().y * 2), this->z_trans + (camera.getPossition().z * 2))); 	// Translate item
	model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));									// scale item
	// model = glm::rotate(model, glm::radians((front.y * -1) * 50), glm::vec3(0, 1, 0)); 			// where x, y, z is axis of rotation (e.g. 0 1 0)
	// model = glm::rotate(model, glm::radians(front.y), glm::vec3(0, 1, 0)); 			// where x, y, z is axis of rotation (e.g. 0 1 0)
	// model = glm::rotate(model, glm::radians(front.z), glm::vec3(0, 0, 1)); 			// where x, y, z is axis of rotation (e.g. 0 1 0)
	glUniformMatrix4fv(glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	this->text->Draw(*this->_shader);
}
