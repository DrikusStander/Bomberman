/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.class.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnolte <cnolte@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 11:20:47 by cnolte            #+#    #+#             */
/*   Updated: 2018/08/14 13:32:43 by cnolte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESH_CLASS_HPP
# define MESH_CLASS_HPP

# include <vector>

# include <glm/gtc/matrix_transform.hpp>
# include "Shader.class.hpp"

# include <assimp/postprocess.h>

struct	Vertex
{
	// Position
	glm::vec3	Position;

	// Normal
	glm::vec3	Normal;

	// TexCoords
	glm::vec2	TexCoords;
};

struct	Texture
{
	GLuint		id;
	std::string	type;
	aiString	path;
};

class Mesh
{
	public:
		//start canonical form
		Mesh(void);
		Mesh(Mesh const & src);
		~Mesh(void);

		Mesh	&operator=(Mesh const &rhs);
		//end canonical form

		Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
		void	Draw(Shader shader);

	private:
		void	setupMesh(void);

		std::vector<Vertex>		vertices;
		std::vector<GLuint>		indices;
		std::vector<Texture>	textures;

		GLuint	VAO;
		GLuint	VBO;
		GLuint	EBO;
};

#endif
