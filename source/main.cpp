#include "Game.class.hpp"

int	main(void)
{
	try
	{
		Game game(2500, 1350);
	}
	catch (std::exception &e)
	{
		std::cerr << "[ERROR]: " << e.what() << std::endl;
		return (-1);
	}
	return (0);
}
