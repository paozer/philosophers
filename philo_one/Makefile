NAME	=	philo_one

SRCS	=	$(SRCS_D)philo_one.c\
			$(SRCS_D)philo_setup.c\
			$(SRCS_D)time.c\
			$(SRCS_D)utils.c\

OBJS	=	$(SRCS:.c=.o)

HEADER	=	$(SRCS:.c=.h)

SRCS_D	=	srcs/

CC		=	gcc

CFLAGS	=	-pthread -g3 -fsanitize=thread -I$(SRCS_D) -Wall -Wextra #-Werror

%.o:	%.c $(HEADER)
		@$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

all: 	$(NAME)

$(NAME):$(OBJS) Makefile
		$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
		@echo "\x1b[32m"$(NAME) compilation done..."\x1b[0m"
		@echo "\x1b[31m"all your base are belong to us"\x1b[0m"

clean:
		@rm -rf $(OBJS)
		@echo "\x1b[31m"make clean done..."\x1b[0m"

fclean: clean
		@rm -rf $(NAME)
		@echo "\x1b[31m"make fclean done..."\x1b[0m"

re:		fclean all

.PHONY: all fclean clean re
