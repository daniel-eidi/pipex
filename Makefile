# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: daeidi-h <daeidi-h@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/03 18:17:47 by daeidi-h          #+#    #+#              #
#    Updated: 2022/05/09 17:33:40 by daeidi-h         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# COLORS
NOCOL=\033[0m
RED=\033[1;31m
YEL=\033[1;33m
ORG=\033[0;33m
GRN=\033[1;32m
DGRAY=\033[1;30m
BLU=\033[1;34m

# OS
UNAME_S := $(shell uname -s)

# **************************************************************************** #
#                                   PROGRAM                                    #
# **************************************************************************** #

NAME = pipex

# **************************************************************************** #
#                                   COMPILER                                   #
# **************************************************************************** #

MAKE = make

CC = gcc

CFLAGS = -Wall -Wextra -Werror

# **************************************************************************** #
#                                    FLAGS                                     #
# **************************************************************************** #

CFLAGS += -I ./inc

# **************************************************************************** #
#                                    PATHS                                     #
# **************************************************************************** #

SRC_PATH = src
OBJ_PATH = obj
INC_PATH = inc

# **************************************************************************** #
#                                     LIBS                                     #
# **************************************************************************** #

LFT_NAME = libft.a
LFT_DIR = libft
LFT = $(LFT_DIR)/$(LFT_NAME)

CFLAGS += -I ./$(LFT_DIR)

LDFLAGS = -L ./$(LFT_DIR)

LDLIBS = -lft

# **************************************************************************** #
#                                   SOURCES                                    #
# **************************************************************************** #

SRC_FILES = main.c utils.c ft_get_next_line.c
SRC_FILES_BONUS = main_bonus.c utils.c ft_get_next_line.c here_doc.c

SRC = $(addprefix $(SRC_PATH)/, $(SRC_FILES))
SRC_BONUS = $(addprefix $(SRC_PATH)/, $(SRC_FILES_BONUS))

OBJ_FILES = $(SRC_FILES:%.c=%.o)
OBJ_FILES_BONUS = $(SRC_FILES_BONUS:%.c=%.o)

OBJ = $(addprefix $(OBJ_PATH)/, $(OBJ_FILES))
OBJ_BONUS = $(addprefix $(OBJ_PATH)/, $(OBJ_FILES_BONUS))

# **************************************************************************** #
#                                    RULES                                     #
# **************************************************************************** #

.PHONY: all clean fclean re norm

# all
all: $(NAME)
	@printf "OK\n"

# name
$(NAME): $(LFT_NAME) $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

bonus:  $(LFT_NAME) $(OBJ_BONUS)
	$(CC) $(CFLAGS) $^ -o $(NAME) $(LDFLAGS) $(LDLIBS)

# libft
$(LFT_NAME): $(LFT)
	cp $(LFT) $(LFT_NAME)
$(LFT):
	$(MAKE) all -sC $(LFT_DIR)

# objs
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

# obj path
$(OBJ_PATH):
	mkdir -p $(OBJ_PATH) 

# clean
clean:
	$(MAKE) clean -sC $(LFT_DIR)
	rm -rf $(LFT_NAME)
	rm -rf $(OBJ_PATH)

# fclean
fclean: clean
	$(MAKE) fclean -sC $(LFT_DIR)
	rm -rf $(NAME)

# re
re: fclean all
rebonus: fclean bonus
# norminette
norm:
	@printf "\n${GRN}##########${YEL} NORMINETTE ${GRN}##########${NOCOL}\n"
	@printf "\n${GRN}LIBFT:${BLU}\n\n"
	@norminette $(LFT_DIR)
	@printf "\n${GRN}PIPEX:${BLU}\n\n"
	@norminette $(SRC_PATH)
	@printf "${NOCOL}"
