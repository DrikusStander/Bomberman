#ifndef LOADINGSCREEN_CLASS_HPP
# define LOADINGSCREEN_CLASS_HPP

# include "Model.class.hpp"

// GLM Mathemtics
# include <glm/gtc/type_ptr.hpp>

class LoadingScreen
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
		LoadingScreen(Shader &shader, std::string model);
		LoadingScreen(LoadingScreen const & src);
		~LoadingScreen(void);
		LoadingScreen const & operator=(LoadingScreen const & rhs);
		void	draw(void);
};

#endif
