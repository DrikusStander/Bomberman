#ifndef WORLD_CLASS_HPP
# define WORLD_CLASS_HPP

# include <string>

// GLEW
# include <GL/glew.h>

// GLFW
# include <GLFW/glfw3.h>

// GLM Mathemtics
# include <glm/gtc/type_ptr.hpp>

# include "Model.class.hpp"
# include "Player.class.hpp"
# include "Enemy.class.hpp"
# include "Item.class.hpp"
# include "Powerup.class.hpp"
# include "Scoring.class.hpp"

# include <vector>

class World
{
	private:
	protected:
		Model	*WorldModel;
		Scoring	*text;
		Shader	*_shader;
		float	x_trans;
		float	y_trans;
		float	z_trans;
		Player	*player;
		char	**map;
		std::vector<Item*>	*objects;
		std::vector<Enemy*>	*enemies;
		std::vector<Powerup*>	*powerups;
		int		worldStatus;

	public:
		World( Shader &shader, std::string model);
		World( World const & src);
		~World( void );
		World const & operator=(World const & rhs);
		void	draw(glm::mat4 matCamera);
		virtual void	ProcessKeyboard(Direction direction);
		float	player_getX(void);
		float	player_getZ(void);
		void	player_clipX(float x_move);
		void	player_clipZ(float z_move);
		int		getStatus( void );
};

#endif
