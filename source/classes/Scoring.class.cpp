#include "Scoring.class.hpp"
#include "glm/gtc/matrix_inverse.hpp"
Scoring::Scoring(Shader &shader, std::string model)
{
	std::cout << "Scoring - Constructor called " << std::endl;
	this->_shader = &shader;
	this->text = new Model(model);
	this->x_trans = -0.125f;
	this->z_trans = -0.15f;
	this->y_trans = 0.072f;
	this->rotate = 90.0f;
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
	glm::mat4 model = glm::affineInverse(camera.GetViewMatrix());
	model = glm::translate(model, glm::vec3(this->x_trans, this->y_trans, this->z_trans)); 	// Translate item
	model = glm::scale(model, glm::vec3(0.002f, 0.002f, 0.002f));									// scale item
	model = glm::rotate(model, glm::radians(this->rotate), glm::vec3(1, 0, 0)); 			// where x, y, z is axis of rotation (e.g. 0 1 0)
	// model = glm::rotate(model, glm::radians(front.y), glm::vec3(0, 1, 0)); 			// where x, y, z is axis of rotation (e.g. 0 1 0)
	// model = glm::rotate(model, glm::radians(camera.getFront().z * camera.getPossition().z), glm::vec3(1, 0, 0)); 			// where x, y, z is axis of rotation (e.g. 0 1 0)
	// model *= camera.GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	this->text->Draw(*this->_shader);
}
