#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.class.hpp"
#include "Item.class.hpp"


class Player : public Character
{
	private:
		Item	*bomb;

	protected:
	public:
		Player( Shader &shader );
		// Player( Player const & src);
		~Player( void );
		// Player const & operator=(Player const & rhs);
		void draw(void);
		void	ProcessKeyboard(Direction direction);
};

#endif