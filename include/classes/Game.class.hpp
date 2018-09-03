/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.class.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwilliam <dwilliam@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 11:20:47 by cnolte            #+#    #+#             */
/*   Updated: 2018/09/03 13:07:48 by dwilliam         ###   ########.fr       */
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
#include <unistd.h>

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

	private:
		std::vector<MainMenu*>	Menus;
		GLfloat	deltaTime;
		GLfloat	lastFrame;
		GLuint	s_WIDTH;
		GLuint	s_HEIGHT;
		int		SCREEN_HEIGHT;
		int		SCREEN_WIDTH;
		int		menuActive;
		bool	menuVisible;

		void	DoMovement(void);
		void	MoveMenu(void);
};

#endif
