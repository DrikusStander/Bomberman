#ifndef PLAYER_CLASS_HPP
# define PLAYER_CLASS_HPP

# include "Character.class.hpp"
# include "Bomb.class.hpp"

class Player : public Character
{
	private:
		std::vector<Bomb*>	bomb;
		int		bombCount;
		int		active;
		int		speedMult;
	protected:
	public:
		Player( Shader &shader, std::string model);
		Player( Player const & src);
		~Player( void );
		Player const & operator=(Player const & rhs);
		void draw(void);
		void	ProcessKeyboard(Direction direction);
		using Character::setMap;
		using Character::getCol;
		using Character::getRow;
		float	getX();
		float	getZ();
		void	clipX(float x_move);
		void	clipZ(float z_move);
		void	setPosOnMap();
		void	clearPosOnMap();
		void	handlePowerup(int powerup);
};

#endif
