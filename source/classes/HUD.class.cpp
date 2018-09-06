#include "HUD.class.hpp"

int	numDigits(int number);

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
	HUD_Pos.none = 0.0f;
	this->lives = 19;
	this->time = 0;
	this->score = 0;
	loadOBJ("0", HUD_Pos.none);
	loadOBJ("1", HUD_Pos.none);
	loadOBJ("2", HUD_Pos.none);
	loadOBJ("3", HUD_Pos.none);
	loadOBJ("4", HUD_Pos.none);
	loadOBJ("5", HUD_Pos.none);
	loadOBJ("6", HUD_Pos.none);
	loadOBJ("7", HUD_Pos.none);
	loadOBJ("8", HUD_Pos.none);
	loadOBJ("9", HUD_Pos.none);
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
		this->lives = rhs.lives;
		this->score = rhs.score;
		this->time = rhs.time;
	}
	return(*this);
}

void	HUD::loadOBJ(std::string objName, float pos)
{
	HUD_properties	tmp;
	tmp.obj_name = objName;
	tmp.model = Model("resources/models/HUD/" + objName + ".obj");
	tmp.pos.x = pos;
	tmp.pos.y = 0.0775f;
	tmp.pos.z = -0.15f;
	tmp.rotate = 90.0f;
	this->HUD_item.push_back(tmp);
}

void	HUD::drawLives(glm::mat4 matCamera, int value)
{
	std::string	strValue(std::to_string(value));
	for (int i = 0; i < numDigits(value); i++)
	{
		
		int j = strValue[i] - 48;

		glm::mat4 model = glm::affineInverse(matCamera);
		model = glm::translate(model, glm::vec3(this->HUD_item[j].pos.x + 0.014f + (0.006f * (float)i), this->HUD_item[j].pos.y, this->HUD_item[j].pos.z)); 	// Translate item
		model = glm::scale(model, glm::vec3(0.00035f, 0.00035f, 0.00035f));				// scale item
		model = glm::rotate(model, glm::radians(this->HUD_item[j].rotate), glm::vec3(1, 0, 0)); 	// where x, y, z is axis of rotation (e.g. 0 1 0)
		glUniformMatrix4fv(glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
		this->HUD_item[j].model.Draw(*this->_shader);
	}
}

void	HUD::draw(glm::mat4 matCamera)
{
	for (int i = 10; i <= 12; i++)
	{
		glm::mat4 model = glm::affineInverse(matCamera);
		model = glm::translate(model, glm::vec3(this->HUD_item[i].pos.x, this->HUD_item[i].pos.y, this->HUD_item[i].pos.z)); 	// Translate item
		model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));				// scale item
		model = glm::rotate(model, glm::radians(this->HUD_item[i].rotate), glm::vec3(1, 0, 0)); 	// where x, y, z is axis of rotation (e.g. 0 1 0)
		glUniformMatrix4fv(glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
		this->HUD_item[i].model.Draw(*this->_shader);
	}
	drawLives(matCamera, 1234567890);
	// for (std::vector<HUD_properties>::iterator it = this->HUD_item.begin(); it != this->HUD_item.end(); it++)
	// {
	// 	if (it->is_num == true)
	// 	{
	// 		if (std::stoi(it->obj_name) <= 9)
	// 		{
	// 			if (std::stoi(it->obj_name) == this->lives)
	// 			{
	// 				glm::mat4 model = glm::affineInverse(matCamera);
	// 				model = glm::translate(model, glm::vec3(it->pos.x + 0.142f, it->pos.y - 0.001f, it->pos.z)); 	// Translate item
	// 				model = glm::scale(model, glm::vec3(0.00035f, 0.00035f, 0.00035f));			// scale item
	// 				model = glm::rotate(model, glm::radians(it->rotate), glm::vec3(1, 0, 0)); 	// where x, y, z is axis of rotation (e.g. 0 1 0)
	// 				glUniformMatrix4fv(glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	// 				it->model.Draw(*this->_shader);
	// 			}
	// 		}
	// 		else if ((std::stoi(it->obj_name) >= 10) && (std::stoi(it->obj_name) <= 99))
	// 		{

	// 		}
	// 	}
	// 	else
	// 	{
			// glm::mat4 model = glm::affineInverse(matCamera);
			// model = glm::translate(model, glm::vec3(it->pos.x, it->pos.y, it->pos.z)); 	// Translate item
			// model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));				// scale item
			// model = glm::rotate(model, glm::radians(it->rotate), glm::vec3(1, 0, 0)); 	// where x, y, z is axis of rotation (e.g. 0 1 0)
			// glUniformMatrix4fv(glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
			// it->model.Draw(*this->_shader);
	// 	}
	// }
}

int	numDigits(int number)
{
	int	digits = 0;
	while (number)
	{
		number /= 10;
		digits++;
	}
	return (digits);
}
