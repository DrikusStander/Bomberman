/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnolte <cnolte@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 11:20:47 by cnolte            #+#    #+#             */
/*   Updated: 2018/08/14 13:35:37 by cnolte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADER_CLASS_HPP
# define SHADER_CLASS_HPP

# include <iostream>
# include <fstream>
# include <sstream>

# include <GL/glew.h>

class Shader
{
	public:
		//start canonical form
		Shader(void);
		Shader(const GLchar *vertexPath, const GLchar *fragmentPath);
		Shader(Shader const & src);
		~Shader(void);

		Shader	&operator=(Shader const &rhs);
		//end canonical form

		void	Use(void);
		GLuint	getProgram(void);
	private:
		GLuint	program;
};

#endif
