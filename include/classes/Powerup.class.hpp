#ifndef POWERUP_CLASS_HPP
# define POWERUP_CLASS_HPP

# include <string>
// GLEW
# include <GL/glew.h>

// GLFW
# include <GLFW/glfw3.h>

// GLM Mathemtics
# include <glm/gtc/type_ptr.hpp>

# include "Item.class.hpp"

class Powerup : public Item
{
	private:
	protected:
		int activeFrame;
	public:
		Powerup( Shader &shader, std::string model);
		Powerup( Powerup const & src);
		~Powerup( void );
		Powerup const & operator=(Powerup const & rhs);
		// void	setPos(float x, float z, int row, int col);
		void	draw(void);
		// void	setActive( bool active );
		// bool	getActive( void );
		// void	setMap( char **map);
		// int		getCol();
		// int		getRow();
};

#endif
