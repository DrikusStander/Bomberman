#ifndef PAUSEMENU_CLASS_HPP
# define PAUSEMENU_CLASS_HPP

# include "Model.class.hpp"

// GLM Mathemtics
# include <glm/gtc/type_ptr.hpp>

class PauseMenu
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
		PauseMenu(Shader &shader, std::string model);
		PauseMenu(PauseMenu const & src);
		~PauseMenu(void);
		PauseMenu const & operator=(PauseMenu const & rhs);
		void	draw(void);
};

#endif
