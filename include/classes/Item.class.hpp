#ifndef ITEM_CLASS_HPP
# define ITEM_CLASS_HPP

# include <string>
// GLEW
# include <GL/glew.h>

// GLFW
# include <GLFW/glfw3.h>

// GLM Mathemtics
# include <glm/gtc/type_ptr.hpp>

# include "Model.class.hpp"

class Item
{
	private:
	protected:
		Model *ItemModel;
		std::vector<Model*> itemModelarr;
		Shader *_shader;
		float	x_trans;
		float	y_trans;
		float	z_trans;
		float	rotate;
		bool	active;
		int		row;
		int		col;
		char	**map;
	public:
		Item( Shader &shader, std::string model);
		Item( Item const & src);
		~Item( void );
		Item const & operator=(Item const & rhs);
		void	setPos(float x, float z, int row, int col);
		void	draw(void);
		void	setActive( bool active );
		bool	getActive( void );
		void	setMap( char **map);
		int		getCol();
		int		getRow();
		float	getX();
		float	getZ();
};

#endif
