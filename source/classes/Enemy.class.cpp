#include "Enemy.class.hpp"

Enemy::Enemy( Shader &shader, std::string model) : Character(shader, model)
{
	std::cout << "Enemy - Constructor called " << std::endl;
	this->dir = FWD;
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
	glm::mat4 model(1);
	model = glm::translate( model, glm::vec3(this->x_trans, this->y_trans, this->z_trans)); 	// Translate item
	model = glm::scale( model, glm::vec3(6.25f, 4.25f, 6.25f));									// so scale item
	model = glm::rotate(model, glm::radians(this->rotate), glm::vec3(0, 1, 0)); 				// where x, y, z is axis of rotation (e.g. 0 1 0)
	glUniformMatrix4fv( glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model ));
	this->characterModel->Draw(*this->_shader);
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

// void	Enemy::moveFwd()
// {
// 	if (this->row > 0)
// 	{
// 		if (this->map[this->row - 1][this->col] == '\0')
// 		{
// 			if (fmod(((168) - (this->x_trans - 10.5)), -21) != 0.0f)
// 			{
// 				if ((fmod(((168) - (this->z_trans - 10.5)), -21) == 0) && (this->row >= 0))
// 				{
// 					if (fmod( (168 - (this->x_trans - 10.5)), -21 ) > 10.5)
// 						this->clipX(fmod(((168) - (this->x_trans)), -21));
// 					else if (fmod(((168) - (this->x_trans - 10.5)), -21) <= 10.5)
// 						this->clipX(fmod(((-168) - (this->x_trans)), -21));
// 				}
// 				this->rotate = 180.0f;
// 				this->z_trans -= 0.5f;
// 				this->row = static_cast<int>((-168 - (this->z_trans + 10.5)) /  -21);
// 			}
// 		}
// 		else if (static_cast<int>((-168 - ((this->z_trans - 0.5) + 10.5)) /  -21) > this->row - 1) 
// 		{
// 			if (fmod(((168) - (this->z_trans - 10.5)), -21) < 10.5)
// 			{
// 				if (fmod( (168 - (this->x_trans - 10.5)), -21 ) > 10.5)
// 					this->clipX(fmod(((168) - (this->x_trans)), -21));
// 				else if (fmod(((168) - (this->x_trans - 10.5)), -21) <= 10.5)
// 					this->clipX(fmod(((-168) - (this->x_trans)), -21));
// 				this->rotate = 180.0f;
// 				this->z_trans -= 0.5f;
// 				this->row = static_cast<int>((-168 - (this->z_trans + 10.5)) /  -21);
// 			}
// 		}
// }

void	Enemy::ProcessKeyboard(Direction direction)
{}
// 	switch(direction)
// 	{
// 		case FWD:
// 		{
// 			
// 			}
// 			else if (this->z_trans > -168)
// 			{
// 				this->rotate = 180.0f;
// 				this->z_trans -= 0.5f;
// 				this->row = static_cast<int>((-168 - (this->z_trans + 10.5)) /  -21);
// 			}
// 			break;
// 		}
// 		case BKW:
// 		{
// 			if (this->row < 16)
// 			{
// 				if (this->map[this->row + 1][this->col] == '\0')
// 				{
// 					// if (fmod(((168) - (this->x_trans - 10.5)), -21) != 0.0f)
// 					{
// 						if ((fmod(((168) - (this->z_trans - 10.5)), -21) == 0) && (this->row < 16))
// 						{
// 							if (fmod(((168) - (this->x_trans) - 10.5), -21) > 10.5)
// 								this->clipX(fmod(((168) - (this->x_trans)), -21));
// 							else if (fmod(((168) - (this->x_trans - 10.5)), -21) <= 10.5)
// 								this->clipX(fmod(((-168) - (this->x_trans)), -21));
// 							else if (fmod(((168) - (this->x_trans - 10.5)), -21) > 10.5)
// 								this->clipX(fmod(((168) - (this->x_trans)), -21));
// 						}
// 						this->rotate = 0.0f;
// 						this->z_trans += 0.5f;
// 						this->row = static_cast<int>((-168 - (this->z_trans + 10.5)) /  -21);
// 					}
// 				}
// 				else if (static_cast<int>((-168 - ((this->z_trans + 0.5) + 10.5)) /  -21) < this->row + 1) 
// 				{
// 					if (fmod(((168) - (this->z_trans - 10.5)), -21) > 10.5 ||  fmod(((168) - (this->z_trans - 10.5)), -21) == 0)
// 					{
// 						if (fmod(((168) - (this->x_trans) - 10.5), -21) > 10.5)
// 							this->clipX(fmod(((168) - (this->x_trans)), -21));
// 						else if (fmod(((168) - (this->x_trans - 10.5)), -21) <= 10.5)
// 							this->clipX(fmod(((-168) - (this->x_trans)), -21));
// 						else if (fmod(((168) - (this->x_trans - 10.5)), -21) > 10.5)
// 							this->clipX(fmod(((168) - (this->x_trans)), -21));
// 						this->rotate = 0.0f;
// 						this->z_trans += 0.5f;
// 						this->row = static_cast<int>((-168 - (this->z_trans + 10.5)) /  -21);
// 					}
// 			}
// 			}
// 			else if (this->z_trans < 168)
// 			{
// 				this->rotate = 0.0f;
// 				this->z_trans += 0.5f;
// 				this->row = static_cast<int>((-168 - (this->z_trans + 10.5)) /  -21);
// 			}
// 			break;
// 		}
// 		case LFT:
// 		{
// 			if (this->col > 0)
// 			{
// 				if (this->map[this->row][this->col - 1] == '\0')
// 				{
// 					// if (fmod(((168) - (this->z_trans - 10.5)), -21) != 0.0f)
// 					{
// 						if ((fmod(((168) - (this->x_trans - 10.5)), -21) == 0) && (this->col >= 0) )
// 						{
// 							if (fmod(((168) - (this->z_trans - 10.5)), -21) > 10.5)
// 								this->clipZ(fmod(((168) - (this->z_trans)), -21));
// 							else if (fmod(((168) - (this->z_trans - 10.5)), -21) <= 10.5)
// 								this->clipZ(fmod(((-168) - (this->z_trans)), -21));
// 						}
// 						this->rotate = 270.0f;
// 						this->x_trans -= 0.5f;
// 						this->col = static_cast<int>((-168 - (this->x_trans + 10.5)) /  -21);
// 					}
// 				}
// 				else if (static_cast<int>((-168 - ((this->x_trans - 0.5) + 10.5)) /  -21) > this->col - 1)
// 				{
// 					if (fmod(((168) - (this->x_trans - 10.5)), -21) < 10.5)
// 					{
// 						if (fmod(((168) - (this->z_trans - 10.5)), -21) > 10.5)
// 							this->clipZ(fmod(((168) - (this->z_trans)), -21));
// 						else if (fmod(((168) - (this->z_trans - 10.5)), -21) <= 10.5)
// 							this->clipZ(fmod(((-168) - (this->z_trans)), -21));
// 						this->rotate = 270.0f;
// 						this->x_trans -= 0.5f;
// 						this->col = static_cast<int>((-168 - (this->x_trans + 10.5)) /  -21);
// 					}
// 				}
// 			}
// 			else if (this->x_trans > -168)
// 			{
// 				this->rotate = 270.0f;
// 				this->x_trans -= 0.5;
// 				this->col = static_cast<int>((-168 - (this->x_trans + 10.5)) /  -21);
// 			}
// 			break;
// 		}
// 		case RGT:
// 		{
// 			if (this->col < 16 )
// 			{
// 				if (this->map[this->row][this->col + 1] == '\0')
// 				{
// 					// if (fmod( ( (168) - (this->z_trans - 10.5) ), -21 ) != 0.0f)
// 					{
// 						if ((fmod(((168) - (this->x_trans - 10.5)), -21) == 0) && (this->col < 16) )
// 						{
// 							if (fmod(((168) - (this->z_trans - 10.5)), -21) > 10.5)
// 								this->clipZ(fmod(((168) - (this->z_trans)), -21));
// 							else if (fmod(((168) - (this->z_trans - 10.5)), -21) <= 10.5)
// 								this->clipZ(fmod(((-168) - (this->z_trans)), -21));
// 						}
// 						this->rotate = 90.0f;
// 						this->x_trans += 0.5f;
// 						this->col = static_cast<int>((-168 - (this->x_trans + 10.5)) /  -21);
// 					}
// 				}
// 				else if (static_cast<int>((-168 - ((this->x_trans + 0.5) + 10.5)) /  -21) < this->col + 1)
// 				{
// 					if (fmod(((168) - (this->x_trans - 10.5)), -21) > 10.5 || fmod(((168) - (this->x_trans - 10.5)), -21) == 0)
// 					{
// 						if (fmod(((168) - (this->z_trans - 10.5)), -21) > 10.5)
// 							this->clipZ(fmod(((168) - (this->z_trans)), -21));
// 						else if (fmod(((168) - (this->z_trans - 10.5)), -21) <= 10.5)
// 							this->clipZ(fmod(((-168) - (this->z_trans)), -21));
// 						this->rotate = 90.0f;
// 						this->x_trans += 0.5f;
// 						this->col = static_cast<int>((-168 - (this->x_trans + 10.5)) /  -21);
// 					}
// 				}
// 			}
// 			else if (this->x_trans < 168)
// 			{
// 				this->rotate = 90.0f;
// 				this->x_trans += 0.5f;
// 				this->col = static_cast<int>((-168 - (this->x_trans + 10.5)) /  -21);
// 			}
// 			break;
// 		}
// 		case SPC:
// 		{
// 			if (this->bomb->getActive() == false)
// 			{
// 				float bomb_x;
// 				float bomb_z;
// 				if (fmod(((168) - (this->z_trans - 10.5)), -21) > 10.5)
// 					bomb_z = this->z_trans + fmod(((168) - (this->z_trans)), -21);
// 				else if (fmod(((168) - (this->z_trans - 10.5)), -21) <= 10.5)
// 					bomb_z = this->z_trans + fmod(((-168) - (this->z_trans)), -21);
// 				if (fmod(((168) - (this->x_trans) - 10.5), -21) > 10.5)
// 					bomb_x = this->x_trans + fmod(((168) - (this->x_trans)), -21);
// 				else if (fmod(((168) - (this->x_trans - 10.5)), -21) <= 10.5)
// 					bomb_x = this->x_trans + fmod(((-168) - (this->x_trans)), -21);
// 				this->bomb->setPos(bomb_x, bomb_z, this->row, this->col);
// 				this->bomb->setMap(this->map);
// 				this->bomb->setActive(true);
// 			}
// 			break;
// 		}
// 	}
	
// 	// std::cout << "* x_trans: " << this->x_trans << std::endl;
// 	// std::cout << "* z_trans: " << this->z_trans << std::endl;
// 	std::cout << "x_trans: " << std::to_string((fmod(((168.0f) - (this->x_trans - 10.5f)), -21.0f))) << std::endl;
// 		std::cout << "z_trans: "<< std::to_string((fmod(((168) - (this->z_trans - 10.5)), -21))) << std::endl;
// 	std::cout << "array[" << this->row << "][" << this->col << "]" << std::endl;
// }
