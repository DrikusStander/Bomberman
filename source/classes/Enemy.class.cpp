#include "Enemy.class.hpp"

Enemy::Enemy( Shader &shader, std::string model) : Character(shader, model)
{
	std::cout << "Enemy - Constructor called " << std::endl;
	this->dir = FWD;
	this->changedir = 0;
}


Enemy::Enemy( Enemy const & src) : Character(src)
{
	*this = src;
}

Enemy::~Enemy( void )
{
	std::cout << "Enemy - Destructor called " << std::endl;
}

Enemy const & Enemy::operator=(Enemy const & rhs)
{
	this->dir = rhs.dir;
	return(*this);
}

void Enemy::draw(void)
{
	this->ProcessKeyboard(this->dir);
	glm::mat4 model(1);
	model = glm::translate( model, glm::vec3(this->x_trans, this->y_trans, this->z_trans)); 	// Translate item
	model = glm::scale( model, glm::vec3(6.25f, 4.25f, 6.25f));									// so scale item
	model = glm::rotate(model, glm::radians(this->rotate), glm::vec3(0, 1, 0)); 				// where x, y, z is axis of rotation (e.g. 0 1 0)
	glUniformMatrix4fv( glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model ));
	this->characterModel->Draw(*this->_shader);
	if (this->changedir == 87 * 3)
	{
		this->dir = static_cast<Direction>(rand() % 4);
		this->changedir = 0;
	}
}

float	Enemy::getX()
{
	return this->x_trans;
}

float	Enemy::getZ()
{
	return this->z_trans;
}

void	Enemy::clipX(float x_move)

{
	this->x_trans += x_move;
}

void	Enemy::clipZ(float z_move)
{
	this->z_trans += z_move;
}

void	Enemy::setPosOnMap()
{
	if (this->map[this->row][this->col] == 'P')
		this->map[this->row][this->col] = 'D';
	else if (this->map[this->row][this->col] != 'B')
		this->map[this->row][this->col] = 'E';
}

void	Enemy::clearPosOnMap()
{
	if (this->map[this->row][this->col] == 'P')
		this->map[this->row][this->col] = 'P';
	else if (this->map[this->row][this->col] != 'B' && this->map[this->row][this->col] != 'D')
		this->map[this->row][this->col] = '\0';
}

void	Enemy::moveFwd()
{
	if (this->row > 0)
	{
		if (this->map[this->row - 1][this->col] == '\0' || this->map[this->row - 1][this->col] == 'P' )
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
				this->z_trans -= 0.5f;
				this->clearPosOnMap();
				// this->map[this->row][this->col] = '\0';
				this->row = static_cast<int>((-168 - (this->z_trans + 10.5)) /  -21);
				// this->active++;
				this->setPosOnMap();
				// this->map[this->row][this->col] = 'E';
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
				this->z_trans -= 0.5f;
				this->clearPosOnMap();
				// this->map[this->row][this->col] = '\0';
				this->row = static_cast<int>((-168 - (this->z_trans + 10.5)) /  -21);
				// this->active++;
				this->setPosOnMap();
				// this->map[this->row][this->col] = 'E';
			}
			else
				this->changeDir();
		}
		
	}
	else if (this->z_trans > -168)
	{
		this->rotate = 180.0f;
		this->z_trans -= 0.5f;
				this->clearPosOnMap();
		// this->map[this->row][this->col] = '\0';
		this->row = static_cast<int>((-168 - (this->z_trans + 10.5)) /  -21);
		// this->active++;
				this->setPosOnMap();
		// this->map[this->row][this->col] = 'E';
	}
	else
		this->changeDir();
}

void	Enemy::moveBkw()
{
	if (this->row < 16)
	{
		if (this->map[this->row + 1][this->col] == '\0' || this->map[this->row + 1][this->col] == 'P' )
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
				this->z_trans += 0.5f;
				this->clearPosOnMap();
				// this->map[this->row][this->col] = '\0';
				this->row = static_cast<int>((-168 - (this->z_trans + 10.5)) /  -21);
				// this->active++;
				this->setPosOnMap();
				// this->map[this->row][this->col] = 'E';
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
				this->z_trans += 0.5f;
				this->clearPosOnMap();
				// this->map[this->row][this->col] = '\0';
				this->row = static_cast<int>((-168 - (this->z_trans + 10.5)) /  -21);
				// this->active++;
				this->setPosOnMap();
				// this->map[this->row][this->col] = 'E';
			}
			else
				this->changeDir();
		}
		
	}
	else if (this->z_trans < 168)
	{
		this->rotate = 0.0f;
		this->z_trans += 0.5f;
				this->clearPosOnMap();
		// this->map[this->row][this->col] = '\0';
		this->row = static_cast<int>((-168 - (this->z_trans + 10.5)) /  -21);
		// this->active++;
				this->setPosOnMap();
		// this->map[this->row][this->col] = 'E';
	}
	else
		this->changeDir();
}

void	Enemy::moveLft()
{
	if (this->col > 0)
	{
		if (this->map[this->row][this->col - 1] == '\0' || this->map[this->row][this->col - 1] == 'P' )
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
				this->x_trans -= 0.5f;
				this->clearPosOnMap();
				// this->map[this->row][this->col] = '\0';
				this->col = static_cast<int>((-168 - (this->x_trans + 10.5)) /  -21);
				// this->active++;
				this->setPosOnMap();
				// this->map[this->row][this->col] = 'E';
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
				this->x_trans -= 0.5f;
				this->clearPosOnMap();
				// this->map[this->row][this->col] = '\0';
				this->col = static_cast<int>((-168 - (this->x_trans + 10.5)) /  -21);
				// this->active++;
				this->setPosOnMap();
				// this->map[this->row][this->col] = 'E';
			}
			else
				this->changeDir();
		}
		
	}
	else if (this->x_trans > -168)
	{
		this->rotate = 270.0f;
		this->x_trans -= 0.5;
				this->clearPosOnMap();
		// this->map[this->row][this->col] = '\0';
		this->col = static_cast<int>((-168 - (this->x_trans + 10.5)) /  -21);
		// this->active++;
				this->setPosOnMap();
		// this->map[this->row][this->col] = 'E';
	}
	else
		this->changeDir();
}

void	Enemy::moveRgt()
{
	if (this->col < 16 )
	{
		if (this->map[this->row][this->col + 1] == '\0' || this->map[this->row][this->col + 1] == 'P')
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
				this->x_trans += 0.5f;
				this->clearPosOnMap();
				// this->map[this->row][this->col] = '\0';
				this->col = static_cast<int>((-168 - (this->x_trans + 10.5)) /  -21);
				// this->active++;
				this->setPosOnMap();
				// this->map[this->row][this->col] = 'E';
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
				this->x_trans += 0.5f;
				this->clearPosOnMap();
				// this->map[this->row][this->col] = '\0';
				this->col = static_cast<int>((-168 - (this->x_trans + 10.5)) /  -21);
				// this->active++;
				this->setPosOnMap();
				// this->map[this->row][this->col] = 'E';
			}
			else
				this->changeDir();
		}
		
	}
	else if (this->x_trans < 168)
	{
		this->rotate = 90.0f;
		this->x_trans += 0.5f;
				this->clearPosOnMap();
		// this->map[this->row][this->col] = '\0';
		this->col = static_cast<int>((-168 - (this->x_trans + 10.5)) /  -21);
		// this->active++;
				this->setPosOnMap();
		// this->map[this->row][this->col] = 'E';
	}
	else
		this->changeDir();
}

void	Enemy::changeDir()
{
	switch (this->dir)
	{
		case FWD:
		{
			if (this->row < 16 && this->map[this->row + 1][this->col] == '\0')
			{
				this->dir = BKW;
			}
			else if (this->col < 16 && this->map[this->row][this->col + 1] == '\0')
			{
				this->dir = RGT;
			}
			else if (this->col > 0 && this->map[this->row][this->col - 1] == '\0')
			{
				this->dir = LFT;
			}
			break;
		}
		case BKW:
		{
			if (this->row > 0 && this->map[this->row - 1][this->col] == '\0')
			{
				this->dir = FWD;
			}
			else if (this->col > 0 && this->map[this->row][this->col - 1] == '\0')
			{
				this->dir = LFT;
			}
			else if (this->col < 16 && this->map[this->row][this->col + 1] == '\0')
			{
				this->dir = RGT;
			}
			break;
		}
		case LFT:
		{
			if (this->col < 16 && this->map[this->row][this->col + 1] == '\0')
			{
				this->dir = RGT;
			}
			else if (this->row > 0 && this->map[this->row - 1][this->col] == '\0')
			{
				this->dir = FWD;
			}
			else if (this->row < 16 && this->map[this->row + 1][this->col] == '\0')
			{
				this->dir = BKW;
			}
			break;
		}
		case RGT:
		{
			if (this->col > 0 && this->map[this->row][this->col - 1] == '\0')
			{
				this->dir = LFT;
			}
			else if (this->row < 16 && this->map[this->row + 1][this->col] == '\0')
			{
				this->dir = BKW;
			}
			else if (this->row > 0 && this->map[this->row - 1][this->col] == '\0')
			{
				this->dir = FWD;
			}
			break;
		}
		default:
			break;
	}

	
}

void	Enemy::ProcessKeyboard(Direction direction)
{
	this->changedir++;
	switch(direction)
	{
		case FWD:
		{
			this->moveFwd();
			break;
		}
		case BKW:
		{
			this->moveBkw();
			break;
		}
		case LFT:
		{
			this->moveLft();
			break;
		}
		case RGT:
		{
			this->moveRgt();
			break;
		}
		case SPC:
			break;
	}
}