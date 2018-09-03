#ifndef MAINMENU_CLASS_HPP
# define MAINMENU_CLASS_HPP

# include "Model.class.hpp"

// GLM Mathemtics
# include <glm/gtc/type_ptr.hpp>

class MainMenu
{
	private:
		Model	*menu;
		Shader	*_shader;
		float	x_trans;
		float	y_trans;
		float	z_trans;
		float	rotate;
	protected:
	public:
		MainMenu(Shader &shader, std::string model);
		MainMenu(MainMenu const & src);
		~MainMenu(void);
		MainMenu const & operator=(MainMenu const & rhs);
		void	draw(void);
};

#endif
