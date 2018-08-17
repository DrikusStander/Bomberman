#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.class.hpp"
#include "Bomb.class.hpp"


class Player : public Character
{
	private:
		Bomb	*bomb;

	protected:
	public:
		Player( Shader &shader, std::string model);
		Player( Player const & src);
		~Player( void );
		Player const & operator=(Player const & rhs);
		void draw(void);
		void	ProcessKeyboard(Direction direction);
};

#endif