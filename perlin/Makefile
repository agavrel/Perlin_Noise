# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/07/04 17:04:50 by angavrel          #+#    #+#              #
#    Updated: 2017/07/04 17:04:58 by angavrel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = Perlin

CPP = gcc

SDLFLAGS = -framework SDL2 -F ./frameworks

SDL2_HEADER = -I ./frameworks/SDL2.framework/Headers/

SDL2_P = -rpath @loader_path/frameworks/

CFLAGS = -O3

HEADER = includes

SOURCES = main.c \
		event.c \
		parsing.c \
		raycast.c \
		misc.c \
		ray_misc.c \
		thread.c

DIR_O = obj

DIR_S = srcs

LIB = libft

SRCS = $(addprefix $(DIR_S)/,$(SOURCES))
OBJS = $(addprefix $(DIR_O)/,$(SOURCES:.c=.o))

all: obj $(NAME)

${NAME}: $(OBJS)
	make -C $(LIB)
	${CPP} $(CFLAGS) -o $@ $^ -L $(LIB) $(SDL2_P) $(SDLFLAGS) $(SDL2_HEADER)

$(DIR_O)/%.o: $(DIR_S)/%.c
	$(CPP) $(CFLAGS) -I $(HEADER) $(SDL2_HEADER) -c -o $@ $<

obj:
	mkdir -p obj

clean:
	rm -rf obj
	make -C $(LIB) clean

fclean: clean
	rm -rf $(NAME)
	make -C $(LIB) fclean

re: clean fclean all

.PHONY: all $(NAME)
