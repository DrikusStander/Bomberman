#include "HUD.class.hpp"

HUD::HUD(Shader &shader, std::string model)
{
	std::cout << "HUD - Constructor called " << std::endl;
	this->_shader = &shader;
	HUD_properties	tmp;
	tmp.model = Model(model);
	tmp.pos.x = -0.135f;
	tmp.pos.y = 0.0775f;
	tmp.pos.z = -0.15f;
	tmp.rotate = 90.0f;
	HUD_item.push_back(tmp);
}


HUD::HUD(HUD const & src)
{
	*this = src;
}

HUD::~HUD(void)
{
	std::cout << "HUD - Destructor called " << std::endl;
}

HUD const & HUD::operator=(HUD const & rhs)
{
	if (this != &rhs)
	{
		for (int i = 0; i < rhs.HUD_item.size(); i++)
			this->HUD_item.push_back(rhs.HUD_item[i]);
		this->_shader = rhs._shader;
	}
	return(*this);
}

void HUD::draw(glm::mat4 matCamera)
{
	for (std::vector<HUD_properties>::iterator it = this->HUD_item.begin(); it != this->HUD_item.end(); it++)
	{
		glm::mat4 model = glm::affineInverse(matCamera);
		model = glm::translate(model, glm::vec3(it->pos.x, it->pos.y, it->pos.z)); 	// Translate item
		model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));									// scale item
		model = glm::rotate(model, glm::radians(it->rotate), glm::vec3(1, 0, 0)); 			// where x, y, z is axis of rotation (e.g. 0 1 0)
		glUniformMatrix4fv(glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
		it->model.Draw(*this->_shader);
	}
}
