# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pramella <pramella@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/05/09 16:37:03 by pramella          #+#    #+#              #
#    Updated: 2020/05/22 02:09:59 by pramella         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME	=	philo_one

SRCS	=	$(SRCS_D)main.c\
			$(SRCS_D)life_cycle.c\
			$(SRCS_D)monitor.c\
			$(SRCS_D)setup.c\
			$(SRCS_D)utils.c

OBJS	=	$(SRCS:.c=.o)
HEADER	=	philo.h
SRCS_D	=	srcs/
CC		=	clang
CFLAGS	=	-pthread -Wall -Wextra -Werror

%.o:	%.c $(HEADER)
		@$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

all: 	$(NAME)

debug:	CFLAGS += -g3 -fsanitize=thread
debug:	$(NAME)

$(NAME):$(OBJS) Makefile
		$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
		@echo "$(NAME) compilation done..."
		@echo "all your base are belong to us"

clean:
		@rm -rf $(OBJS)
		@echo "make clean done..."

fclean: clean
		@rm -rf $(NAME)
		@echo "make fclean done..."

re:		fclean all

.PHONY: all fclean clean re
