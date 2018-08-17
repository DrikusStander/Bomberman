#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
// GLEW
# include <GL/glew.h>

// GLFW
# include <GLFW/glfw3.h>

// GLM Mathemtics
# include <glm/gtc/type_ptr.hpp>

#include "Model.class.hpp"
#include "Item.class.hpp"

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
		std::vector<std::vector<Item*>> *map;

	public:
		Character( Shader &shader, std::string model);
		Character( Character const & src);
		~Character( void );
		Character const & operator=(Character const & rhs);
		void	draw(void);
		virtual void	ProcessKeyboard(Direction direction) = 0;
		void setMap( std::vector<std::vector<Item*>> *map);
};

#endif