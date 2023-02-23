/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 02:32:15 by Cutku             #+#    #+#             */
/*   Updated: 2023/02/23 11:54:16 by Cutku            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "./libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>

typedef struct s_pipex
{
	int		fd;
	pid_t	*pid;
	char	*my_envp;
	int		**pipeline;
	char	**command;
	char	**real_envp;
} t_pipex;


void	my_dup2(int input, int output);

#endif