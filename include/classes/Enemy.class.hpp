#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Character.class.hpp"
#include "Bomb.class.hpp"


class Enemy : public Character
{
	private:
	protected:
	public:
		Enemy( Shader &shader, std::string model);
		Enemy( Enemy const & src);
		~Enemy( void );
		Enemy const & operator=(Enemy const & rhs);
		void draw(void);
		void	ProcessKeyboard(Direction direction);
		using Character::setMap;
		using Character::setPos;
		using Character::getCol;
		using Character::getRow;
		float	getX();
		float	getZ();
		void	clipX(float x_move);
		void	clipZ(float z_move);
};

#endif