/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnolte <cnolte@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 11:26:34 by cnolte            #+#    #+#             */
/*   Updated: 2018/09/20 15:38:22 by cnolte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Shader.class.hpp"

Shader::Shader(void)
{
	return;
}

Shader::Shader(Shader const & src)
{
	*this = src;
}

Shader::~Shader(void)
{
	return;
}

Shader	&Shader::operator=(Shader const &rhs)
{
	if (this != &rhs)
		this->program = rhs.program;
	return (*this);
}

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string		vertexCode;
	std::string		fragmentCode;
	std::ifstream	vShaderFile;
	std::ifstream	fShaderFile;
	std::string temp(vertexPath);
	if (temp == "resources/shaders/lighting.vs")
	{
		this->flashlight = true;
	}
	else
		this->flashlight = false;
	// ensures ifstream objects can throw exceptions:
	vShaderFile.exceptions ( std::ifstream::badbit );
	fShaderFile.exceptions ( std::ifstream::badbit );
	try
	{
		// Open files
		vShaderFile.open( vertexPath );
		fShaderFile.open( fragmentPath );
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf( );
		fShaderStream << fShaderFile.rdbuf( );
		// close file handlers
		vShaderFile.close( );
		fShaderFile.close( );
		// Convert stream into string
		vertexCode = vShaderStream.str( );
		fragmentCode = fShaderStream.str( );
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar	*vShaderCode = vertexCode.c_str();
	const GLchar	*fShaderCode = fragmentCode.c_str();
	// 2. Compile shaders
	GLuint	vertex;
	GLuint	fragment;
	GLint	success;
	GLchar	infoLog[512];
	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// Print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Shader Program
	this->program = glCreateProgram();
	glAttachShader(this->program, vertex);
	glAttachShader(this->program, fragment);
	glBindFragDataLocation(this->program, 0, "color");
	glLinkProgram(this->program);
	// Print linking errors if any
	glGetProgramiv(this->program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

GLuint	Shader::getProgram(void)
{
	return (this->program);
}

// Uses the current shader
void	Shader::Use(void)
{
	glUseProgram(this->program);
}

bool	Shader::getFlashLight()
{
	return(this->flashlight);
}
