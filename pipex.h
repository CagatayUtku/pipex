/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 02:32:15 by Cutku             #+#    #+#             */
/*   Updated: 2023/02/26 06:59:39 by Cutku            ###   ########.fr       */
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
# include <errno.h>

typedef struct s_pipex
{
	pid_t	*pid;
	char	*all_paths;
	char	*command_path;
	int		**pipeline;
} t_pipex;

void	my_dup2(int input, int output);
void	free_dubleptr(char **ptr);
void	free_int_dubleptr(int **ptr, int numsize);
char	*get_command_path(char *envp, char *command);
char	*get_env_path(char **envp);
int	open_file(char *filename, int flag);
void	create_pipelines(t_pipex *pipex, int num);
void	first_child(t_pipex *pipex, char **argv, char **envp);
void	last_child(t_pipex *pipex, char **argv, char **envp, int argc);
void	mid_child(t_pipex *pipex, char **argv, char **envp, int argc, int i);


#endif