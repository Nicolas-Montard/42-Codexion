# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nmontard <nmontard@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/05 15:10:14 by nmontard          #+#    #+#              #
#    Updated: 2026/05/26 12:16:50 by nmontard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC      = cc
CFLAGS  = -Wall -Wextra -Werror -pthread -MMD -MP -I headers -ggdb

DIR_TO_CREATE = .obj_dep

CFILES  = create_threads/create_threads.c create_threads/create_threads_struct.c \
          parsing/error_manager.c parsing/parsing.c parsing/verif_number.c \
          utils/utils.c main.c clean/clean.c thread_function/thread_function.c \
		  thread_function/utils.c monitor/monitor.c thread_function/dongle_utils.c

OBJECTS = $(addprefix $(DIR_TO_CREATE)/, $(CFILES:.c=.o))
DEPS    = $(addprefix $(DIR_TO_CREATE)/, $(CFILES:.c=.d))
NAME    = codexion


all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

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

debug: fclean
	$(MAKE) CFLAGS="-Wall -Wextra -Werror -pthread -MMD -MP -I headers -fsanitize=thread" 

.PHONY: all clean fclean re debug

-include $(DEPS)
