# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/05 15:43:45 by Cutku             #+#    #+#              #
#    Updated: 2023/04/21 23:00:54 by Cutku            ###   ########.fr        #
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
PIPEX_SRCS	=	pipex.c parsing.c clean_up.c pipex_utils.c get_paths.c
PIPEX_OBJS	=	$(PIPEX_SRCS:.c=.o)

PIPEX_MAIN		=	pipex_main.c
PPX_MAIN_OBJ	=	$(PIPEX_MAIN:.c=.o)

PIPEX_BNS	=	pipex_bonus/here_doc.c pipex_bonus/pipex_main_bonus.c
BONUS_OBJ	=	$(PIPEX_BNS:.c=.o)

LIBFT		= ./libft/
LIBFT_LIB	= ./libft/libft.a

CC			= cc
RM			= rm -f
CFLAGS		= -Wall -Wextra -Werror 

NAME		= pipex

all: $(NAME)

$(NAME): $(PIPEX_OBJS) $(PPX_MAIN_OBJ)
	make bonus -C $(LIBFT) && make clean -C $(LIBFT)
	echo "$(GREEN)Libft compiled successfully!$(DEF_COLOR)"
	$(CC) $(CFLAGS) $(PPX_MAIN_OBJ) $(PIPEX_OBJS) $(LIBFT_LIB) -o $(NAME)
	echo "$(GREEN)Pipex compiled successfully!$(DEF_COLOR)"

clean:
	$(RM) $(PIPEX_OBJS)
	$(RM) $(BONUS_OBJ)
	$(RM) $(PPX_MAIN_OBJ)

fclean: clean
	make fclean -C $(LIBFT)
	$(RM) $(NAME)

re:	fclean all

bonus: $(BONUS_OBJ) $(PIPEX_OBJS)
	make bonus -C $(LIBFT) && make clean -C $(LIBFT)
	echo "$(GREEN)Libft compiled successfully!$(DEF_COLOR)"
	$(CC) $(CFLAGS) $(BONUS_OBJ) $(PIPEX_OBJS) $(LIBFT_LIB) -o $(NAME)
	echo "$(GREEN)Pipex-Bonus compiled successfully!$(DEF_COLOR)"

.PHONY:	all clean fclean re bonus