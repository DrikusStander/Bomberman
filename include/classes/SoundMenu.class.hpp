#ifndef SOUNDMENU_CLASS_HPP
# define SOUNDMENU_CLASS_HPP

# include "Model.class.hpp"

// GLM Mathemtics
# include <glm/gtc/type_ptr.hpp>

class SoundMenu
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
		SoundMenu(Shader &shader, std::string model);
		SoundMenu(SoundMenu const & src);
		~SoundMenu(void);
		SoundMenu const & operator=(SoundMenu const & rhs);
		void	draw(void);
};

#endif
