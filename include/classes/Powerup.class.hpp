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
		int powerupType;
	public:
		Powerup( Shader &shader, std::string model, int type, std::vector<Model*> *modelArr);
		Powerup( Powerup const & src);
		~Powerup( void );
		Powerup const & operator=(Powerup const & rhs);
		void	draw(void);
		int		getType(void);
};

#endif
