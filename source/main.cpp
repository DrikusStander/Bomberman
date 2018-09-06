#include "Game.class.hpp"

int	main(void)
{
	try
	{
		// Game game(2300, 1920); -> works with full screen... at the moment
		// Game game(1024, 576);
		// Game game(1280, 720);
		Game game(1920, 1080);
	}
	catch (std::exception &e)
	{
		std::cerr << "[ERROR]: " << e.what() << std::endl;
		return (-1);
	}
	return (0);
}
