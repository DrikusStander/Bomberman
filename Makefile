# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dwilliam <dwilliam@42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/07/31 19:21:44 by cnolte            #+#    #+#              #
#    Updated: 2018/09/03 11:17:40 by dwilliam         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL := install_dependencies

NAME := bomberman
SRCS := source/main.cpp source/classes/Game.class.cpp source/classes/Camera.class.cpp source/classes/Mesh.class.cpp \
		source/classes/Shader.class.cpp source/classes/Model.class.cpp source/classes/Character.class.cpp			\
		source/classes/Player.class.cpp source/classes/Item.class.cpp source/classes/Bomb.class.cpp					\
		source/classes/World.class.cpp source/classes/Enemy.class.cpp source/Exceptions.cpp							\
		source/classes/Scoring.class.cpp source/classes/Powerup.class.cpp source/classes/MainMenu.class.cpp
OBJS := $(SRCS:.cpp=.o)

CXX := clang++ -std=c++11

INC_DIRS = include/ include/classes $(HOME)/.brew/include resources/SOIL2
CXXFLAGS += $(addprefix -I, $(INC_DIRS))

$(NAME): $(OBJS)
	@$(MAKE) -C resources/SOIL2 all
	mv resources/SOIL2/libSOIL2.a .
	@$(CXX) $(OBJS) -o $@ -L ~/.brew/lib -lGLEW -lGLFW -lassimp -framework OpenGL -L . -lSOIL2 -framework CoreFoundation
	@echo [INFO] $@ "compiled successfully."

install_dependencies:
	@if ! test -d ~/.brew/Library; \
	then if ! test -s resources/install_brew.sh; \
	then echo "ERROR: Brew install script missing."; \
	else \
		sh resources/install_brew.sh; \
		echo "\n\nDone installing brew. Please run 'make' again."; \
		exec zsh; \
	fi else \
	if ! test -d ~/.brew/Cellar/glfw; \
	then brew install glfw; \
	fi; \
	if ! test -d ~/.brew/include/glm; \
	then brew install glm; \
	fi; \
	if ! test -d ~/.brew/Cellar/assimp; \
	then brew install assimp; \
	fi; \
	if ! test -d ~/.brew/Cellar/glew; \
	then brew install glew; \
	fi; \
	fi
	$(MAKE) -C . all

all: $(NAME)

clean:
	@$(MAKE) -C resources/SOIL2 clean
	@rm -f $(OBJS)
	@echo "[INFO] Objects removed!"

fclean: clean
	@$(MAKE) -C resources/SOIL2 fclean
	@rm -rf libSOIL2.a
	@rm -rf $(NAME)
	@echo "[INFO] $(NAME) removed!"

re: fclean all

.PHONY: all clean fclean re
