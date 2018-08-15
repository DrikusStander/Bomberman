
#ifndef ITEM_HPP
#define ITEM_HPP

// GLEW
# include <GL/glew.h>

// GLFW
# include <GLFW/glfw3.h>

// GLM Mathemtics
# include <glm/gtc/type_ptr.hpp>

#include "Model.class.hpp"

class Item
{
	private:
	protected:
		Model *ItemModel;
		Shader *_shader;
		float	x_trans;
		float	y_trans;
		float	z_trans;
		float	rotate;
		bool	active;
		GLfloat PlantTime;
		GLfloat PlantTimeLength;


	public:
		Item( Shader &shader);
		Item( Item const & src);
		~Item( void );
		Item const & operator=(Item const & rhs);
		void setPos(float x, float y);
		void	draw(void);
		void	setActive( bool active );
		bool	getActive( void );
};

#endif