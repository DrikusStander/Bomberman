/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.class.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cnolte <cnolte@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 11:20:47 by cnolte            #+#    #+#             */
/*   Updated: 2018/08/14 13:16:13 by cnolte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODEL_CLASS_HPP
# define MODEL_CLASS_HPP

# include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SOIL2.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.class.hpp"

class Model
{
	public:
		//start canonical form
		Model(void);
		Model(std::string path);
		Model(Model const & src);
		~Model(void);

		Model	&operator=(Model const &rhs);
		//end canonical form

		void	Draw(Shader shader);

	private:
		std::vector<Mesh>		meshes;
		std::string				directory;
		std::vector<Texture>	textures_loaded;

		void	loadModel(std::string path);
		void	processNode(aiNode* node, const aiScene* scene);
		Mesh	processMesh(aiMesh *mesh, const aiScene *scene);
		GLint	TextureFromFile(const char *path, std::string directory);

		std::vector<Texture>	loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif
