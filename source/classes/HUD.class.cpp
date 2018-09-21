#include "HUD.class.hpp"

int	numDigits(int number);

HUD::HUD(void)
{
	std::cout << "HUD - Default Constructor Called" << std::endl;
}

HUD::HUD(Shader shader, float screen_x, float screen_y)
{
	std::cout << "HUD - Parametric Constructor called" << std::endl;
	this->_shader = shader;
	this->HUD_Pos.left = -0.123f;
	this->HUD_Pos.center = -0.015f;
	this->HUD_Pos.right = 0.122f;
	this->HUD_Pos.none = 0.0f;
	this->lives = 0;
	this->time = 0;
	this->score = 0;
	for (int i = 0; i <= 9; i++)
		loadOBJ(std::to_string(i), HUD_Pos.none);
	loadOBJ("time_left", HUD_Pos.left);
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
		this->HUD_Pos = rhs.HUD_Pos;
	}
	return(*this);
}

void	HUD::loadOBJ(std::string objName, float pos)
{
	HUD_properties	tmp;
	tmp.obj_name = objName;
	tmp.model = Model("resources/models/HUD/" + objName + ".obj");
	tmp.pos.x = pos;
	tmp.pos.y = 0.077f;
	tmp.pos.z = -0.15f;
	tmp.rotate = 90.0f;
	this->HUD_item.push_back(tmp);
}

void	HUD::drawNumbers(glm::mat4 matCamera)
{
	std::string	strTime(std::to_string(this->time));
	for (int i = 0; i < numDigits(this->time); i++)
	{
		int j = strTime[i] - 48;

		glm::mat4 model = glm::affineInverse(matCamera);
		model = glm::translate(model, glm::vec3(this->HUD_Pos.left + 0.025f + (KERNING * (float)i), this->HUD_item[j].pos.y, this->HUD_item[j].pos.z)); 	// Translate item
		model = glm::scale(model, glm::vec3(TEXT_SCALE, TEXT_SCALE, TEXT_SCALE));				// scale item
		model = glm::rotate(model, glm::radians(this->HUD_item[j].rotate), glm::vec3(1, 0, 0)); 	// where x, y, z is axis of rotation (e.g. 0 1 0)
		glUniformMatrix4fv(glGetUniformLocation(this->_shader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
		this->HUD_item[j].model.Draw(this->_shader);
	}

	std::string	strScore(std::to_string(this->score));
	for (int i = 0; i < numDigits(this->score); i++)
	{
		int j = strScore[i] - 48;

		glm::mat4 model = glm::affineInverse(matCamera);
		model = glm::translate(model, glm::vec3(this->HUD_Pos.center + 0.018f + (0.006f * (float)i), this->HUD_item[j].pos.y, this->HUD_item[j].pos.z)); 	// Translate item
		model = glm::scale(model, glm::vec3(TEXT_SCALE, TEXT_SCALE, TEXT_SCALE));				// scale item
		model = glm::rotate(model, glm::radians(this->HUD_item[j].rotate), glm::vec3(1, 0, 0)); 	// where x, y, z is axis of rotation (e.g. 0 1 0)
		glUniformMatrix4fv(glGetUniformLocation(this->_shader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
		this->HUD_item[j].model.Draw(this->_shader);
	}

	std::string	strLives(std::to_string(this->lives));
	for (int i = 0; i < numDigits(this->lives); i++)
	{
		int j = strLives[i] - 48;

		glm::mat4 model = glm::affineInverse(matCamera);
		model = glm::translate(model, glm::vec3(this->HUD_Pos.right + 0.018f + (KERNING * (float)i), this->HUD_item[j].pos.y, this->HUD_item[j].pos.z)); 	// Translate item
		model = glm::scale(model, glm::vec3(TEXT_SCALE, TEXT_SCALE, TEXT_SCALE));				// scale item
		model = glm::rotate(model, glm::radians(this->HUD_item[j].rotate), glm::vec3(1, 0, 0)); 	// where x, y, z is axis of rotation (e.g. 0 1 0)
		glUniformMatrix4fv(glGetUniformLocation(this->_shader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
		this->HUD_item[j].model.Draw(this->_shader);
	}
}

void	HUD::draw(Camera &camera, const int time, const int score, const int lives)
{
	this->_shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(this->_shader.getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	this->time = time;
	this->score = score;
	this->lives = lives;
	for (int i = 10; i <= 12; i++)
	{
		glm::mat4 model = glm::affineInverse(camera.GetViewMatrix());
		model = glm::translate(model, glm::vec3(this->HUD_item[i].pos.x, this->HUD_item[i].pos.y, this->HUD_item[i].pos.z)); 	// Translate item
		model = glm::scale(model, glm::vec3(TEXT_SCALE, TEXT_SCALE, TEXT_SCALE));				// scale item
		model = glm::rotate(model, glm::radians(this->HUD_item[i].rotate), glm::vec3(1, 0, 0)); 	// where x, y, z is axis of rotation (e.g. 0 1 0)
		glUniformMatrix4fv(glGetUniformLocation(this->_shader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
		this->HUD_item[i].model.Draw(this->_shader);
	}
	drawNumbers(camera.GetViewMatrix());
}

int	numDigits(int number)
{
	int	digits = 0;
	if (number == 0)
		return (1);
	while (number)
	{
		number /= 10;
		digits++;
	}
	return (digits);
}
