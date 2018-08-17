#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>
// GLEW
# include <GL/glew.h>

// GLFW
# include <GLFW/glfw3.h>

// GLM Mathemtics
# include <glm/gtc/type_ptr.hpp>

#include "Model.class.hpp"
#include "Player.class.hpp"
#include "Item.class.hpp"

#include <vector>

class World
{
	private:
	protected:
		Model	*WorldModel;
		Shader	*_shader;
		float	x_trans;
		float	y_trans;
		float	z_trans;
		Player	*player;
		std::vector<std::vector<Item*>> *map;
		// Item	**map;

	public:
		World( Shader &shader, std::string model);
		World( World const & src);
		~World( void );
		World const & operator=(World const & rhs);
		void	draw(void);
		virtual void	ProcessKeyboard(Direction direction);
};

#endif