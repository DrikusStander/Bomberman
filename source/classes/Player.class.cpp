
#include "Player.class.hpp"

Player::Player( Shader &shader, std::string model) : Character(shader, model)
{
	std::cout << "Player - Constructor called " << std::endl;
	this->bomb = new Bomb(shader, "resources/models/bom.obj");
}


Player::Player( Player const & src) : Character(src)
{

	*this = src;
}

Player::~Player( void )
{
	std::cout << "Player - Destructor called " << std::endl;
}

Player const & Player::operator=(Player const & rhs)
{
	this->bomb = rhs.bomb;
	return(*this);
}

void Player::draw(void)
{
	glm::mat4 model(1);
	model = glm::translate( model, glm::vec3(this->x_trans, this->y_trans, this->z_trans)); 	// Translate it down a bit so it's at the center of the scene
	model = glm::scale( model, glm::vec3(2.25f, 1.25f, 2.25f));									// It's a bit too big for our scene, so scale it down
	model = glm::rotate(model, glm::radians(this->rotate), glm::vec3(0, 1, 0)); 				// where x, y, z is axis of rotation (e.g. 0 1 0)
	glUniformMatrix4fv( glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model ));
	this->characterModel->Draw(*this->_shader);
	if (this->bomb->getActive() == true)
		this->bomb->draw();
}

void	Player::ProcessKeyboard(Direction direction)
{
	switch(direction)
	{
		case FWD:
			this->rotate = 180.0f;
			this->z_trans--;
			break;
		case BKW:
			this->z_trans++;
			this->rotate = 0.0f;
			break;
		case LFT:
			this->x_trans--;
			this->rotate = 270.0f;
			break;
		case RGT:
			this->x_trans++;
			this->rotate = 90.0f;
			break;
		case SPC:
		{
			if (this->bomb->getActive() == false)
			{
				this->bomb->setPos(this->x_trans, this->z_trans);
				this->bomb->setActive(true);
			}
			break;
		}
	}
}