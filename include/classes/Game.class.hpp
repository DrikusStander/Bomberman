/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.class.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dwilliam <dwilliam@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 11:20:47 by cnolte            #+#    #+#             */
/*   Updated: 2018/08/18 16:15:19 by dwilliam         ###   ########.fr       */
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
#include "World.class.hpp"

class Game
{
	public:
		//start canonical form
		Game(void);
		Game(const int width, const int height);
		Game(Game const & src);
		~Game(void);

		Game	&operator=(Game const &rhs);
		World *world;
		//end canonical form

	private:
		GLfloat	deltaTime;
		GLfloat	lastFrame;
		GLuint	s_WIDTH;
		GLuint	s_HEIGHT;
		int		SCREEN_WIDTH;
		int		SCREEN_HEIGHT;
		// char	mapArray[17][17];
		// int		player_x_pos;
		// int		player_z_pos;

		void	DoMovement(void);
};

#endif
