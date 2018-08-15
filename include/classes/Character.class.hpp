#ifndef CHARACTER_HPP
#define CHARACTER_HPP


// GLEW
# include <GL/glew.h>

// GLFW
# include <GLFW/glfw3.h>

// GLM Mathemtics
# include <glm/gtc/type_ptr.hpp>

#include "Model.class.hpp"

enum Direction
{
	FWD = 0,
	BKW,
	RGT,
	LFT,
	SPC
};

class Character
{
	private:
	protected:
		Model *characterModel;
		Shader *_shader;
		float	x_trans;
		float	y_trans;
		float	z_trans;
		float	rotate;
	public:
		Character( Shader &shader);
		// Character( Character const & src);
		~Character( void );
		// Character const & operator=(Character const & rhs);
		void	draw(void);
		virtual void	ProcessKeyboard(Direction direction) = 0;
};

#endif