/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 02:32:15 by Cutku             #+#    #+#             */
/*   Updated: 2023/04/20 06:12:22 by Cutku            ###   ########.fr       */
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
# include <ctype.h>

typedef struct s_pipex
{
	pid_t	*pid;
	char	**command;
	char	**all_paths;
	int		**pipeline;
	char	**envp;
	char	*cmd_path;
	char	*infile;
	int		heredoc;
	int		num_commands;
}	t_pipex;

// Main process. File > pipex.c
void	create_pipelines(t_pipex *pipex, int num);
void	create_child_process(t_pipex *pipex, char **argv, int argc);
void	exec_child_process(t_pipex *pipex, char **argv, int argc, int i);
//Finding env-command path. File > get_paths.c
char	*get_command_path(t_pipex *pipex);
char	**get_env_path(char **envp);
char	*is_exact_path(t_pipex *pipex);
void	error_cmdpath(t_pipex *pipex);
//Here_doc. File > pipex_bonus.c
void	exec_here_doc(t_pipex *pipex, char **argv, int argc);
//Some usefull functions. File > pipex_utils.c
void	my_dup2(int input, int output, t_pipex *pipex);
void	my_waitpid(t_pipex *pipex);
int		open_file(char *filename, int flag, t_pipex *pipex);
//Parsing. File > parsing.c 
char	**ft_parser(char *string);
char	**word_lenght(char *string, int size);
void	remove_escape(char *str);
//Clean-up. File > clean_up.c
void	free_char_dubleptr(char **ptr);
void	free_int_dubleptr(int **ptr, int size);
void	close_pipes(t_pipex *pipex);
void	free_all(t_pipex *pipex);

#endif