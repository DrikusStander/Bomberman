#ifndef WTCLOGO_CLASS_HPP
# define WTCLOGO_CLASS_HPP

# include "Model.class.hpp"

// GLM Mathemtics
# include <glm/gtc/type_ptr.hpp>

class wtcLogo
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
		wtcLogo(Shader &shader, std::string model);
		wtcLogo(wtcLogo const & src);
		~wtcLogo(void);
		wtcLogo const & operator=(wtcLogo const & rhs);
		void	draw(void);
};

#endif
