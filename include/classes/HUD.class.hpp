#ifndef HUD_CLASS_HPP
# define HUD_CLASS_HPP

# include "Model.class.hpp"

// GLM Mathemtics
# include <glm/gtc/type_ptr.hpp>
# include "glm/gtc/matrix_inverse.hpp"

struct	HUD_properties
{
	std::string	obj_name;
	Model		model;
	glm::vec3	pos;
	float		rotate;
};

struct hudPos
{
	float	left;
	float	center;
	float	right;
};

class HUD
{
	private:
		Shader		*_shader;
		hudPos		HUD_Pos;
		std::vector<HUD_properties>	HUD_item;
		int	loadOBJ(std::string objName, float pos);

	public:
		HUD(void);
		HUD(Shader &shader, float screen_x, float screen_y);
		HUD(HUD const & src);
		~HUD(void);
		HUD const & operator=(HUD const & rhs);
		void	draw(glm::mat4 matCamera);
};

#endif
