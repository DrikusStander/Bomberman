#ifndef HUD_CLASS_HPP
# define HUD_CLASS_HPP

# include "Model.class.hpp"

// GLM Mathemtics
# include <glm/gtc/type_ptr.hpp>
# include "glm/gtc/matrix_inverse.hpp"

struct	HUD_properties
{
	Model		model;
	glm::vec3	pos;
	float		rotate;
};

class HUD
{
	private:
		Shader		*_shader;
		std::vector<HUD_properties>	HUD_item;

	public:
		HUD(Shader &shader, std::string model);
		HUD(HUD const & src);
		~HUD(void);
		HUD const & operator=(HUD const & rhs);
		void	draw(glm::mat4 matCamera);
};

#endif
