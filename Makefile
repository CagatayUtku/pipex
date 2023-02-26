# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/05 15:43:45 by Cutku             #+#    #+#              #
#    Updated: 2023/02/26 16:06:12 by Cutku            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DEF_COLOR = \033[0;39m
RED = \033[0;91m
GREEN = \033[0;32m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

.SILENT:
# Sources
PIPEX_SRCS	=	pipex.c pipex_bonus.c
PIPEX_OBJS	=	$(PIPEX_SRCS:.c=.o)

LIBFT		= ./libft/
LIBFT_LIB	= ./libft/libft.a

CC			= cc
RM			= rm -f
CFLAGS		= -Wall -Wextra -Werror 

NAME		= pipex

all: $(NAME)

$(NAME): $(PIPEX_OBJS)
	make bonus -C $(LIBFT) && make clean -C $(LIBFT)
	echo "$(GREEN)Libft compiled successfully!$(DEF_COLOR)"
	$(CC) $(CFLAGS) $(PIPEX_OBJS) $(LIBFT_LIB) -o $(NAME)
	echo "$(GREEN)Pipex compiled successfully!$(DEF_COLOR)"


clean:
	$(RM) $(PIPEX_OBJS)

fclean: clean
	make fclean -C $(LIBFT)
	$(RM) $(NAME)

re:	fclean all

bonus: all

.PHONY:	all clean fclean re bonus