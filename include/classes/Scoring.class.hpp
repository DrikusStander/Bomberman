#ifndef SCORING_CLASS_HPP
# define SCORING_CLASS_HPP

# include "Model.class.hpp"

// GLM Mathemtics
# include <glm/gtc/type_ptr.hpp>
# include "glm/gtc/matrix_inverse.hpp"

class Scoring
{
	private:
		Model	*text;
		Shader	*_shader;
		float	x_trans;
		float	y_trans;
		float	z_trans;
		float	rotate;
	protected:
	public:
		Scoring(Shader &shader, std::string model);
		Scoring(Scoring const & src);
		~Scoring(void);
		Scoring const & operator=(Scoring const & rhs);
		void	draw(glm::mat4 matCamera);
};

#endif
