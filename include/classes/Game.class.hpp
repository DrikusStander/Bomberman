/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.class.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnolte <cnolte@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 11:20:47 by cnolte            #+#    #+#             */
/*   Updated: 2018/09/25 13:55:30 by cnolte           ###   ########.fr       */
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
# include "PauseMenu.class.hpp"
# include "wtcLogo.class.hpp"
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
		Game(const int width, const int height, const bool debug);
		Game(Game const & src);
		~Game(void);

		Game	&operator=(Game const &rhs);
		//end canonical form

		World	*world;
		void 	createWorld2( void);
		void 	loadStage1(void );
		void 	loadGame1(void );
		void	writeConfig(void);
		void	readConfig(void);
		void	loadmodels1();
		void	loadWTCmodels1(int i);

		// static void 	createWorld( Game *game);

	private:
		std::vector<MainMenu*>	Menus;
		std::vector<wtcLogo*>	wtc;
		std::vector<LoadingScreen*> load;
		std::vector<SoundMenu*> soundMenu;
		std::vector<PauseMenu*> pauseMenu;
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
		bool	pauseVisible;
		int		pauseActive;
		int		check;
		int		keyUP;
		int		keyDOWN;
		int		keyLEFT;
		int		keyRIGHT;
		int		keyBOMB;
		int		keyFLASH;
		bool	toggleFlash;
		int		stage;
		int		key_change;
		float	volMusic;
		float	volEffect;
		bool	debug;
		int		last_menu;
		int		fullScreenItteration;
		GLfloat	pause_died;
		GLfloat	you_won;
		GLfloat	you_died;
		// GLfloat	wtclogo_animate;
		bool	wtc_logo;
		int		lives;

		bool 		WorldLoaded;
		Shader		*shaderNormal;
		Shader		*shaderActive;
		Shader		*shaderFlash;
		void		DoMovement(void);
		void		MoveMenu(void);
		void		MovePause(void);
		void		MoveSoundMenu(void);
		void		placeSpotLight(void);
		inline bool exist(const std::string& name);
		GLFWwindow	*window;
		glm::mat4	projection;
		void		menuIsVisible( void );
		void		loadIsVisible( void );
		void		pauseIsVisible( void );


};
void 	createWorld(Game *game);
void 	loadStage(Game *game);
void 	loadGame(Game *game);
void	loadmodels(Game *game);
void	loadWTCmodels(Game *game, int i);

#endif
