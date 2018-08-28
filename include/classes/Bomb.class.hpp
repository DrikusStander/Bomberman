
#ifndef BOMB_CLASS_HPP
# define BOMB_CLASS_HPP

# include "Item.class.hpp"

class Bomb : public Item
{
	private:
		GLfloat PlantTime;
		GLfloat PlantTimeLength;
		int		blastRaduis;
	protected:

	public:
		Bomb( Shader &shader, std::string model);
		Bomb( Bomb const & src);
		~Bomb( void );
		Bomb const & operator=(Bomb const & rhs);
		void	draw(void);
		void	setActive( bool active );
		void	checkDestruction();
		void	incBlastRaduis();
		using Item::setPos;
		using Item::setMap;
		using Item::getActive;
};

#endif
