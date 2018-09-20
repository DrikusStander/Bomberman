#ifndef HUD_CLASS_HPP
# define HUD_CLASS_HPP

# include "Model.class.hpp"

// GLM Mathemtics
# include <glm/gtc/type_ptr.hpp>
# include "glm/gtc/matrix_inverse.hpp"
# include "Camera.class.hpp"

# define KERNING 0.0063f
# define TEXT_SCALE 0.002f

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
	float	none;
};

class HUD
{
	public:
		HUD(void);
		HUD(Shader shader, float screen_x, float screen_y);
		HUD(HUD const & src);
		~HUD(void);
		HUD const & operator=(HUD const & rhs);
		void	draw(Camera &camera, const int time, const int score, const int lives);

	private:
		Shader		_shader;
		hudPos		HUD_Pos;
		std::vector<HUD_properties>	HUD_item;
		void	loadOBJ(std::string objName, float pos);
		void	drawNumbers(glm::mat4 matCamera);

		int		lives;
		int		score;
		int		time;
};

#endif
