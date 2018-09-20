
#ifndef BOMB_CLASS_HPP
# define BOMB_CLASS_HPP

# include "Item.class.hpp"
# include "Sound.class.hpp"

class Bomb : public Item
{
	private:
		GLfloat PlantTime;
		GLfloat PlantTimeLength;
		GLfloat ExplodeTimeLength;
		int		blastRaduis;
		int		activeFlames;
		bool	destruction;
		std::vector<Item*> flames;
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
		void	setBlastRaduis(int blastRaduis);
		int		getBlastRaduis( void );
		using Item::setPos;
		using Item::setMap;
		using Item::getActive;
		using Item::setShader;
};

#endif
