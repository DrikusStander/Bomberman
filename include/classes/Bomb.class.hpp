
#ifndef BOMB_HPP
#define BOMB_HPP

#include "Item.class.hpp"

class Bomb : public Item
{
	private:
		GLfloat PlantTime;
		GLfloat PlantTimeLength;
	protected:

	public:
		Bomb( Shader &shader, std::string model);
		Bomb( Bomb const & src);
		~Bomb( void );
		Bomb const & operator=(Bomb const & rhs);
		void	draw(void);
		void	setActive( bool active );
		using Item::setPos;
		using Item::getActive;
};

#endif