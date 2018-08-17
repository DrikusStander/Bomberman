
#include "Player.class.hpp"

Player::Player( Shader &shader, std::string model) : Character(shader, model)
{
	std::cout << "Player - Constructor called " << std::endl;
	this->x_trans = -168.0f;
	this->z_trans = -168.0f;
	this->row = 0;
	this->col = 0;
	this->rowCounter = 0;
	this->colCounter = 0;
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
			// check collision func
			if (this->z_trans <= -168 || (*this->map)[this->row][this->col - 1] != NULL)
				break;
			this->colCounter++;
			if (colCounter == 336 / 17)
			{
				this->col--;
				if (this->col < 0)
					this->col = 0;
				this->colCounter = 0;
			}
			this->z_trans--;
			break;
		case BKW:
			this->rotate = 0.0f;
			if (this->z_trans >= 168 || (*this->map)[this->row][this->col + 1] != NULL)
				break;
			this->colCounter++;
			if (colCounter == 336 / 17)
			{
				this->col++;
				if (this->col > 16)
					this->col = 16;
				this->colCounter = 0;
			}
			this->z_trans++;
			break;
		case LFT:
		{
			// std::cout << "row : " << this->row << std::endl;
			this->rotate = 270.0f;
			if (this->x_trans <= -168 || (*this->map)[this->row + 1][this->col] != NULL )
				break;
			this->rowCounter++;
			if (rowCounter == 336 / 17)
			{
				this->row--;
				if (this->row < 0)
					this->row = 0;
				this->rowCounter = 0;
				std::cout << "Left row : " << this->row << std::endl;
			}
			this->x_trans--;
			break;
		}
		case RGT:
		{
			this->rotate = 90.0f;
			if (this->x_trans >= 168 || (*this->map)[this->row - 1][this->col] != NULL)
				break;
			this->rowCounter++;
			if (rowCounter == 336 / 17)
			{
				this->row++;
				if (this->row > 16)
					this->row = 16;
				std::cout << "Right row : " << this->row << std::endl;
				this->rowCounter = 0;
			}
			this->x_trans++;
			break;
		}
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