#include "Item.class.hpp"

Item::Item( Shader &shader, std::string model)
{
	this->ItemModel =  new Model(model);
	this->_shader = &shader;
	this->x_trans = 0.0f;
	this->y_trans = 20.0f;
	this->z_trans = 0.0f;
	this->rotate = 180.0f;
	this->active = false;
}

Item::Item( Shader &shader, Model *model)
{
	this->ItemModel =  model;
	this->_shader = &shader;
	this->x_trans = 0.0f;
	this->y_trans = 20.0f;
	this->z_trans = 0.0f;
	this->rotate = 180.0f;
	this->active = false;
}

Item::Item( Item const & src)
{
	*this = src;
}

Item::~Item( void )
{
	return;
}

Item const & Item::operator=(Item const & rhs)
{
	this->ItemModel =  rhs.ItemModel;
	this->_shader = rhs._shader;
	this->x_trans = rhs.x_trans;
	this->y_trans = rhs.y_trans;
	this->z_trans = rhs.z_trans;
	this->rotate = rhs.rotate;
	this->active = rhs.active;
	return(*this);
}

void Item::draw(void)
{
	this->_shader->Use();
	glm::mat4 model(1);
	model = glm::translate( model, glm::vec3(this->x_trans, this->y_trans, this->z_trans)); // Translate item
	model = glm::scale(model, glm::vec3(4.25f, 2.25f, 4.25f));								// scale item
	model = glm::rotate(model, glm::radians(this->rotate), glm::vec3(0, 1, 0)); 			// where x, y, z is axis of rotation (e.g. 0 1 0)
	glUniformMatrix4fv( glGetUniformLocation(this->_shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model ));
	this->ItemModel->Draw(*this->_shader);
}

void Item::setPos(float x, float z, int row, int col)
{
	this->row = row;
	this->col = col;
	this->x_trans = x;
	this->z_trans = z;
}

void	Item::setActive(bool active)
{
	this->active = active;
}

bool	Item::getActive()
{
	return(this->active);
}
void Item::setMap( char **map)
{
	this->map = map;
}

int		Item::getCol()
{
	return(this->col);
}

int		Item::getRow()
{
	return(this->row);
}

float	Item::getX()
{
	return(this->x_trans);
}

float	Item::getZ()
{
	return(this->z_trans);
}

void	Item::setShader(Shader &shader)
{
	this->_shader = &shader;
}
