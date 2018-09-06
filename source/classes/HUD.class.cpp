#include "HUD.class.hpp"

HUD::HUD(void)
{
	std::cout << "HUD - Default Constructor Called" << std::endl;
}

HUD::HUD(Shader &shader, float screen_x, float screen_y)
{
	std::cout << "HUD - Parametric Constructor called" << std::endl;
	this->_shader = &shader;
	HUD_Pos.left = -0.135f;
	HUD_Pos.center = 0.0f;
	HUD_Pos.right = 0.125f;
	loadOBJ("time", HUD_Pos.left);
	loadOBJ("score", HUD_Pos.center);
	loadOBJ("lives", HUD_Pos.right);
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

int	HUD::loadOBJ(std::string objName, float pos)
{
	HUD_properties	tmp;
	tmp.obj_name = objName;
	tmp.model = Model("resources/models/HUD/" + objName + ".obj");
	tmp.pos.x = pos;
	tmp.pos.y = 0.0775f;
	tmp.pos.z = -0.15f;
	tmp.rotate = 90.0f;
	this->HUD_item.push_back(tmp);
	return (1);
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
