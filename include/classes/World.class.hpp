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
# include "HUD.class.hpp"

# include <vector>

#include <mutex>
#include <unistd.h>

		
extern std::mutex mu;


class World
{
	private:
	protected:
		Model					*WorldModel;
		HUD						hud;
		Shader					*_shader;
		float					x_trans;
		float					y_trans;
		float					z_trans;
		Player					*player;
		char					**map;
		std::vector<Item*>		*objects;
		Model					*wall_model;
		std::vector<Enemy*>		*enemies;
		std::vector<Powerup*>	*powerups;
		int						worldStatus;
		std::vector<Powerup*>	bombRaduis;
		int						bombRaduis_index;
		std::vector<Model *>	*bombRaduis_model;
		std::vector<Powerup*>	bombCount;
		int						bombCount_index;
		std::vector<Model *>	*bombCount_model;
		std::vector<Powerup*>	speed;
		int						speed_index;
		std::vector<Model *>	*speed_model;
		GLFWwindow				*window;
		int						score;
		int						lives;




	public:
		World(Shader &shader, std::string model, float screen_x, float screen_y, GLFWwindow	*window);
		World(World const & src);
		~World(void);
		World const & operator=(World const & rhs);
		void	draw(glm::mat4 matCamera);
		virtual void	ProcessKeyboard(Direction direction);
		float	player_getX(void);
		float	player_getZ(void);
		void	player_clipX(float x_move);
		void	player_clipZ(float z_move);
		int		getStatus( void );
		int		getScore( void );
		int		getLives( void );

};

#endif
