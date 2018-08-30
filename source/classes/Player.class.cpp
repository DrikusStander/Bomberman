
#include "Player.class.hpp"

Player::Player( Shader &shader, std::string model) : Character(shader, model + "0.obj")
{
	this->active = 0;
	for (int i = 0; i < 22; i++)
		this->characterModelarr.push_back(new Model(model + std::to_string(i) + ".obj"));
	std::cout << "Player - Constructor called " << std::endl;
	this->x_trans = -168.0f;
	this->z_trans = -168.0f;
	for (int i = 0; i < 3; i++)
		this->bomb.push_back(new Bomb(shader, "resources/models/bom.obj"));
	this->speedMult = 1;
	this->bombCount = 1;

}


Player::Player( Player const & src) : Character(src)
{

	*this = src;
}

Player::~Player( void )
{
	std::cout << "Player - Destructor called " << std::endl;
	// delete this->bomb;
}

Player const & Player::operator=(Player const & rhs)
{
	this->bomb = rhs.bomb;
	return(*this);
}

void Player::draw(void)
{
	glm::mat4 model(1);
	model = glm::translate( model, glm::vec3(this->x_trans, this->y_trans, this->z_trans)); 	// Translate item
	model = glm::scale( model, glm::vec3(1.2f, 1.2f, 1.2f));									// scale item
	model = glm::rotate(model, glm::radians(this->rotate), glm::vec3(0, 1, 0)); 				// where x, y, z is axis of rotation (e.g. 0 1 0)
	glUniformMatrix4fv( glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model ));
	this->characterModelarr[this->active]->Draw(*this->_shader);
	for (std::vector<Bomb*>::iterator it = this->bomb.begin() ; it != this->bomb.end(); it++)
	{
		if ((*it)->getActive() == true)
			(*it)->draw();
	}
}

float	Player::getX()
{
	return this->x_trans;
}

float	Player::getZ()
{
	return this->z_trans;
}

void	Player::clipX(float x_move)
{
	this->x_trans += x_move;
}

void	Player::clipZ(float z_move)
{
	this->z_trans += z_move;
}

void	Player::setPosOnMap()
{
	if (this->map[this->row][this->col] != 'B' && this->map[this->row][this->col] != 'D' && this->map[this->row][this->col] != 'U')
		this->map[this->row][this->col] = 'P';
}

void	Player::clearPosOnMap()
{
	if (this->map[this->row][this->col] != 'B' && this->map[this->row][this->col] != 'D' && this->map[this->row][this->col] != 'U')
		this->map[this->row][this->col] = '\0';
}

void	Player::ProcessKeyboard(Direction direction)
{
	if (this->active == 21)
		this->active = 0;
	switch(direction)
	{
		case FWD:
		{
			if (this->row > 0)
			{
				if (this->map[this->row - 1][this->col] == '\0' || this->map[this->row - 1][this->col] == 'E' || this->map[this->row - 1][this->col] == 'U')
				{
					if (fmod(((168) - (this->x_trans - 10.5)), -21) != 0.0f)
					{
						if ((fmod(((168) - (this->z_trans - 10.5)), -21) == 0) && (this->row >= 0))
						{
							if (fmod( (168 - (this->x_trans - 10.5)), -21 ) > 10.5)
								this->clipX(fmod(((168) - (this->x_trans)), -21));
							else if (fmod(((168) - (this->x_trans - 10.5)), -21) <= 10.5)
								this->clipX(fmod(((-168) - (this->x_trans)), -21));
						}
						this->rotate = 180.0f;
						this->z_trans -= 0.5f * this->speedMult;
						this->clearPosOnMap();
						this->row = static_cast<int>((-168 - (this->z_trans + 10.5)) /  -21);
						this->active++;
						this->setPosOnMap();
					}
				}
				else if (static_cast<int>((-168 - ((this->z_trans - 0.5) + 10.5)) /  -21) > this->row - 1) 
				{
					if (fmod(((168) - (this->z_trans - 10.5)), -21) < 10.5)
					{
						if (fmod( (168 - (this->x_trans - 10.5)), -21 ) > 10.5)
							this->clipX(fmod(((168) - (this->x_trans)), -21));
						else if (fmod(((168) - (this->x_trans - 10.5)), -21) <= 10.5)
							this->clipX(fmod(((-168) - (this->x_trans)), -21));
						this->rotate = 180.0f;
						this->z_trans -= 0.5f * this->speedMult;
						this->clearPosOnMap();
						this->row = static_cast<int>((-168 - (this->z_trans + 10.5)) /  -21);
						this->active++;
						this->setPosOnMap();
					}
				}
			}
			else if (this->z_trans > -168)
			{
				this->rotate = 180.0f;
				this->z_trans -= 0.5f * this->speedMult;
				this->clearPosOnMap();
				this->row = static_cast<int>((-168 - (this->z_trans + 10.5)) /  -21);
				this->active++;
				this->setPosOnMap();
			}
			break;
		}
		case BKW:
		{
			if (this->row < 16)
			{
				if (this->map[this->row + 1][this->col] == '\0' || this->map[this->row + 1][this->col] == 'E' || this->map[this->row + 1][this->col] == 'U')
				{
					if (fmod(((168) - (this->x_trans - 10.5)), -21) != 0.0f)
					{
						if ((fmod(((168) - (this->z_trans - 10.5)), -21) == 0) && (this->row < 16))
						{
							if (fmod(((168) - (this->x_trans) - 10.5), -21) > 10.5)
								this->clipX(fmod(((168) - (this->x_trans)), -21));
							else if (fmod(((168) - (this->x_trans - 10.5)), -21) <= 10.5)
								this->clipX(fmod(((-168) - (this->x_trans)), -21));
							else if (fmod(((168) - (this->x_trans - 10.5)), -21) > 10.5)
								this->clipX(fmod(((168) - (this->x_trans)), -21));
						}
						this->rotate = 0.0f;
						this->z_trans += 0.5f * this->speedMult;
						this->clearPosOnMap();
						this->row = static_cast<int>((-168 - (this->z_trans + 10.5)) /  -21);
						this->active++;
						this->setPosOnMap();
					}
				}
				else if (static_cast<int>((-168 - ((this->z_trans + 0.5) + 10.5)) /  -21) < this->row + 1) 
				{
					if (fmod(((168) - (this->z_trans - 10.5)), -21) > 10.5 ||  fmod(((168) - (this->z_trans - 10.5)), -21) == 0)
					{
						if (fmod(((168) - (this->x_trans) - 10.5), -21) > 10.5)
							this->clipX(fmod(((168) - (this->x_trans)), -21));
						else if (fmod(((168) - (this->x_trans - 10.5)), -21) <= 10.5)
							this->clipX(fmod(((-168) - (this->x_trans)), -21));
						else if (fmod(((168) - (this->x_trans - 10.5)), -21) > 10.5)
							this->clipX(fmod(((168) - (this->x_trans)), -21));
						this->rotate = 0.0f;
						this->z_trans += 0.5f * this->speedMult;
						this->clearPosOnMap();
						this->row = static_cast<int>((-168 - (this->z_trans + 10.5)) /  -21);
						this->active++;
						this->setPosOnMap();
					}
			}
			}
			else if (this->z_trans < 168)
			{
				this->rotate = 0.0f;
				this->z_trans += 0.5f * this->speedMult;
				this->clearPosOnMap();
				this->row = static_cast<int>((-168 - (this->z_trans + 10.5)) /  -21);
				this->active++;
				this->setPosOnMap();
			}
			break;
		}
		case LFT:
		{
			if (this->col > 0)
			{
				if (this->map[this->row][this->col - 1] == '\0' || this->map[this->row][this->col - 1] == 'E' || this->map[this->row][this->col - 1] == 'U')
				{
					if (fmod(((168) - (this->z_trans - 10.5)), -21) != 0.0f)
					{
						if ((fmod(((168) - (this->x_trans - 10.5)), -21) == 0) && (this->col >= 0) )
						{
							if (fmod(((168) - (this->z_trans - 10.5)), -21) > 10.5)
								this->clipZ(fmod(((168) - (this->z_trans)), -21));
							else if (fmod(((168) - (this->z_trans - 10.5)), -21) <= 10.5)
								this->clipZ(fmod(((-168) - (this->z_trans)), -21));
						}
						this->rotate = 270.0f;
						this->x_trans -= 0.5f * this->speedMult;
						this->clearPosOnMap();
						this->col = static_cast<int>((-168 - (this->x_trans + 10.5)) /  -21);
						this->active++;
						this->setPosOnMap();
					}
				}
				else if (static_cast<int>((-168 - ((this->x_trans - 0.5) + 10.5)) /  -21) > this->col - 1)
				{
					if (fmod(((168) - (this->x_trans - 10.5)), -21) < 10.5)
					{
						if (fmod(((168) - (this->z_trans - 10.5)), -21) > 10.5)
							this->clipZ(fmod(((168) - (this->z_trans)), -21));
						else if (fmod(((168) - (this->z_trans - 10.5)), -21) <= 10.5)
							this->clipZ(fmod(((-168) - (this->z_trans)), -21));
						this->rotate = 270.0f;
						this->x_trans -= 0.5f * this->speedMult;
						this->clearPosOnMap();
						this->col = static_cast<int>((-168 - (this->x_trans + 10.5)) /  -21);
						this->active++;
						this->setPosOnMap();
					}
				}
			}
			else if (this->x_trans > -168)
			{
				this->rotate = 270.0f;
				this->x_trans -= 0.5 * this->speedMult;
				this->clearPosOnMap();
				this->col = static_cast<int>((-168 - (this->x_trans + 10.5)) /  -21);
				this->active++;
				this->setPosOnMap();
			}
			break;
		}
		case RGT:
		{
			if (this->col < 16 )
			{
				if (this->map[this->row][this->col + 1] == '\0' || this->map[this->row][this->col + 1] == 'E' || this->map[this->row][this->col + 1] == 'U')
				{
					if (fmod( ( (168) - (this->z_trans - 10.5) ), -21 ) != 0.0f)
					{
						if ((fmod(((168) - (this->x_trans - 10.5)), -21) == 0) && (this->col < 16) )
						{
							if (fmod(((168) - (this->z_trans - 10.5)), -21) > 10.5)
								this->clipZ(fmod(((168) - (this->z_trans)), -21));
							else if (fmod(((168) - (this->z_trans - 10.5)), -21) <= 10.5)
								this->clipZ(fmod(((-168) - (this->z_trans)), -21));
						}
						this->rotate = 90.0f;
						this->x_trans += 0.5f * this->speedMult;
						this->clearPosOnMap();
						this->col = static_cast<int>((-168 - (this->x_trans + 10.5)) /  -21);
						this->active++;
						this->setPosOnMap();
					}
				}
				else if (static_cast<int>((-168 - ((this->x_trans + 0.5) + 10.5)) /  -21) < this->col + 1)
				{
					if (fmod(((168) - (this->x_trans - 10.5)), -21) > 10.5 || fmod(((168) - (this->x_trans - 10.5)), -21) == 0)
					{
						if (fmod(((168) - (this->z_trans - 10.5)), -21) > 10.5)
							this->clipZ(fmod(((168) - (this->z_trans)), -21));
						else if (fmod(((168) - (this->z_trans - 10.5)), -21) <= 10.5)
							this->clipZ(fmod(((-168) - (this->z_trans)), -21));
						this->rotate = 90.0f;
						this->x_trans += 0.5f * this->speedMult;
						this->clearPosOnMap();
						this->col = static_cast<int>((-168 - (this->x_trans + 10.5)) /  -21);
						this->active++;
						this->setPosOnMap();
					}
				}
			}
			else if (this->x_trans < 168)
			{
				this->rotate = 90.0f;
				this->x_trans += 0.5f * this->speedMult;
				this->clearPosOnMap();
				this->col = static_cast<int>((-168 - (this->x_trans + 10.5)) /  -21);
				this->active++;
				this->setPosOnMap();
			}
			break;
		}
		case SPC:
		{
			std::cout << "need to place bomb" << std::endl;
			for (int i = 0 ; i < this->bombCount; i++)
			{
				std::cout << "bomb: " << i << std::endl;
				if (this->map[this->row][this->col] == 'B')
					break;
				if (this->bomb[i]->getActive() == false)
				{
					float bomb_x;
					float bomb_z;
					if (fmod(((168) - (this->z_trans - 10.5)), -21) > 10.5)
						bomb_z = this->z_trans + fmod(((168) - (this->z_trans)), -21);
					else if (fmod(((168) - (this->z_trans - 10.5)), -21) <= 10.5)
						bomb_z = this->z_trans + fmod(((-168) - (this->z_trans)), -21);
					if (fmod(((168) - (this->x_trans) - 10.5), -21) > 10.5)
						bomb_x = this->x_trans + fmod(((168) - (this->x_trans)), -21);
					else if (fmod(((168) - (this->x_trans - 10.5)), -21) <= 10.5)
						bomb_x = this->x_trans + fmod(((-168) - (this->x_trans)), -21);
					this->bomb[i]->setPos(bomb_x, bomb_z, this->row, this->col);
					this->bomb[i]->setMap(this->map);
					this->bomb[i]->setActive(true);
					break;
				}
			}
			break;
		}
	}
	
	// std::cout << "* x_trans: " << this->x_trans << std::endl;
	// std::cout << "* z_trans: " << this->z_trans << std::endl;
	// std::cout << "x_trans: " << std::to_string((fmod(((168.0f) - (this->x_trans - 10.5f)), -21.0f))) << std::endl;
	// std::cout << "z_trans: "<< std::to_string((fmod(((168) - (this->z_trans - 10.5)), -21))) << std::endl;
	// std::cout << "array[" << this->row << "][" << this->col << "]" << std::endl;
}

void	Player::handlePowerup(int powerup)
{
	if (powerup == 0)
	{
		for (std::vector<Bomb*>::iterator it = this->bomb.begin() ; it != this->bomb.end(); it++)
		{
			(*it)->incBlastRaduis();
		}
	}
	else if (powerup == 1)
	{
		if (this->speedMult < 7)
			this->speedMult += 2;
	}
	else if (powerup == 2)
	{
		if (this->bombCount < 3)
			this->bombCount++;
	}
}