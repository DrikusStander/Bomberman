#include "Game.class.hpp"

int	main(int argc, char **argv)
{
	try
	{
		if (argc == 2)
		{
			std::string arg(argv[1]);
			if (arg == "-help")
				std::cout << std::endl << "Usage: " << "./bomberman" << std::endl << "For degbug use flag: -debug" << std::endl;
			else if (arg == "-debug")
				Game game(1920, 1080, true);
			else
				throw Exceptions::InvalidArguments();
		}
		else if (argc > 1)
			throw Exceptions::TooManyArguments();
		else
			Game game(1920, 1080, false);
	}
	catch (std::exception &e)
	{
		std::cerr << "[ERROR]: " << e.what() << std::endl;
		return (-1);
	}
	return (0);
}
