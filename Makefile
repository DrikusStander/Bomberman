# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cnolte <cnolte@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/07/31 19:21:44 by cnolte            #+#    #+#              #
#    Updated: 2018/08/09 18:41:46 by cnolte           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL := install_dependencies

NAME := nibbler libraries/FLTK/lib1-FLTK.so libraries/SDL2/lib2-SDL2.so libraries/SFML/lib3-SFML.so

SRCS := source/main.cpp source/Exceptions.cpp source/classes/Game.class.cpp source/classes/Snake.class.cpp
OBJS := $(SRCS:.cpp=.o)

CXX := clang++ -std=c++11

INC_DIRS = include/ include/classes libraries/FLTK/include/classes libraries/SDL2/include/classes libraries/SFML/include/classes $(HOME)/.brew/include
CXXFLAGS += $(addprefix -I, $(INC_DIRS))

DLIBRARIES = libraries/FLTK/source/FLTKGameEvent.class.o libraries/FLTK/source/Cube.class.o \
			 libraries/SDL2/source/SDL2GameEvent.class.o libraries/SDL2/source/Cube.class.o \
			 libraries/SFML/source/SFMLGameEvent.class.o libraries/SFML/source/Cube.class.o

$(NAME): $(OBJS)
	@$(MAKE) -C libraries all
	@$(CXX) $(OBJS) -o $@ 
	@echo [INFO] $@ "compiled successfully."

all: $(NAME)

install_dependencies:
ifneq ($(LD_LIBRARY_PATH),$(HOME)/.brew/lib)
	@echo "export LD_LIBRARY_PATH=~/.brew/lib" >> ~/.zshrc
	@echo "Added missing environment variable. Please run 'make' again."
	@exec zsh;
endif
	@if ! test -d ~/.brew/Library; \
	then if ! test -s resources/install_brew.sh; \
	then echo "ERROR: Brew install script missing."; \
	else \
		sh resources/install_brew.sh; \
		echo "\n\nDone installing brew. Please run 'make' again."; \
		exec zsh; \
	fi else \
	if ! test -d ~/.brew/Cellar/fltk; \
	then brew install fltk; \
	fi; \
	if ! test -d ~/.brew/Cellar/sdl2; \
	then brew install SDL2; \
	fi; \
	if ! test -d ~/.brew/Cellar/sfml; \
	then brew install sfml; \
	fi; \
	fi
	$(MAKE) -C . all; \

clean:
	@$(MAKE) -C libraries clean
	@rm -f $(OBJS)
	@echo "[INFO] Objects removed!"

fclean: clean
	@$(MAKE) -C libraries fclean
	@rm -f $(NAME)
	@echo "[INFO] $(NAME) removed!"

re: fclean all

.PHONY: all clean fclean re
