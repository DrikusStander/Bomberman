/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.class.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwilliam <dwilliam@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 11:20:47 by cnolte            #+#    #+#             */
/*   Updated: 2018/09/17 18:53:20 by dwilliam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_CLASS_HPP
# define GAME_CLASS_HPP

// GLEW
# include <GL/glew.h>

// GLFW
# include <GLFW/glfw3.h>

// GLM Mathemtics
# include <glm/gtc/type_ptr.hpp>

# include "Camera.class.hpp"
# include "Model.class.hpp"

// #include "Character.class.hpp"
# include "World.class.hpp"

# include "Exceptions.hpp"

# include "MainMenu.class.hpp"
# include "LoadingScreen.class.hpp"
# include "SoundMenu.class.hpp"
# include <unistd.h>
# include <thread>
# include <future>

# include "Sound.class.hpp"

# include <mutex>

extern std::mutex mu;

class Game
{
	public:
		//start canonical form
		Game(void);
		Game(const int width, const int height);
		Game(Game const & src);
		~Game(void);

		Game	&operator=(Game const &rhs);
		//end canonical form

		World	*world;
		void 	createWorld2( void);
		void 	loadStage1(void );
		// static void 	createWorld( Game *game);

	private:
		std::vector<MainMenu*>	Menus;
		std::vector<LoadingScreen*> load;
		std::vector<SoundMenu*> soundMenu;
		GLfloat	deltaTime;
		GLfloat	lastFrame;
		GLuint	screen_x;
		GLuint	screen_y;
		int		SCREEN_HEIGHT;
		int		SCREEN_WIDTH;
		int		menuActive;
		bool	menuVisible;
		bool	loadVisible;
		int		loadActive;
		bool	soundMenuVisible;
		int		soundActive;

		bool 		WorldLoaded;
		Shader		*shader;
		void		DoMovement(void);
		void		MoveMenu(void);
		void		MoveSoundMenu(void);
		GLFWwindow	*window;
		glm::mat4	projection;

};
void 	createWorld(Game *game);
void 	loadStage(Game *game);

#endif
