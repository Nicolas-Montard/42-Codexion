# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/05 15:10:14 by nmontard          #+#    #+#              #
#    Updated: 2026/04/30 02:56:32 by nmontard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC      = cc
CFLAGS  = -Wall -Wextra -Werror -MMD -MP -I headers -ggdb

DIR_TO_CREATE = obj_and_dep_dir

CFILES  = create_threads/create_threads.c create_threads/create_threads_struct.c \
          parsing/error_manager.c parsing/parsing.c parsing/verif_number.c \
          utils/utils.c main.c clean/clean.c

OBJECTS = $(addprefix $(DIR_TO_CREATE)/, $(CFILES:.c=.o))
DEPS    = $(addprefix $(DIR_TO_CREATE)/, $(CFILES:.c=.d))
NAME    = codexion

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $^ -o $@

$(DIR_TO_CREATE)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(DIR_TO_CREATE)

fclean: clean
	rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) $(NAME)

.PHONY: all clean fclean re

-include $(DEPS)
