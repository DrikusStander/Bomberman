#ifndef PLAYER_CLASS_HPP
# define PLAYER_CLASS_HPP

# include "Character.class.hpp"
# include "Bomb.class.hpp"

class Player : public Character
{
	private:
		std::vector<Bomb*>	bomb;
		int					bombCount;
		float				active;
		int					speedMult;
		float				activeMult;
		int					lives;
		bool				dead;
		bool				invincible;
		bool				toggleFlash;
		Direction			dirLast;

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
		int		getLives( void );
		int		subLife( void );
		bool	getInvincible( void );
		void	setShader(Shader &shader);
		void	drawBomb(void);
		int		getBombCount();
		int		getBombRaduis();
		int		getSpeedMult();
		void	setSpeedMult(int speedMult);
		void	setlives(int lives);
		void	setBombCount(int bombCount);
		void	setBombRaduis(int bombRaduis);
		void	setActiveMult(float activeMult);
		float	getActiveMult( void );
		Direction	getDirLast(void);

};

#endif
