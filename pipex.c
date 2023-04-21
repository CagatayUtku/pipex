/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 16:50:15 by Cutku             #+#    #+#             */
/*   Updated: 2023/04/21 06:04:17 by Cutku            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	create_pipelines(t_pipex *pipex, int num)
{
	int	i;

	i = -1;
	pipex->pipeline = (int **)malloc(num * sizeof(int *));
	if (pipex->pipeline == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	while (++i < num)
	{
		pipex->pipeline[i] = (int *)malloc(sizeof(int) * 2);
		if (pipex->pipeline[i] == NULL)
		{
			free_int_dubleptr(pipex->pipeline, i);
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		if (pipe(pipex->pipeline[i]) == -1)
		{
			free_int_dubleptr(pipex->pipeline, num);
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}
}

void	create_child_process(t_pipex *pipex, char **argv, int argc)
{
	int	i;

	pipex->pid = malloc(sizeof(pid_t) * (pipex->num_commands));
	if (pipex->pid == NULL)
	{
		free_int_dubleptr(pipex->pipeline, pipex->num_commands - 1);
		exit(EXIT_FAILURE);
	}
	i = -1;
	while (++i < pipex->num_commands)
	{
		pipex->pid[i] = fork();
		if (pipex->pid[i] == -1)
		{
			perror("Fork");
			free(pipex->pid);
			free_int_dubleptr(pipex->pipeline, pipex->num_commands - 1);
			exit(EXIT_FAILURE);
		}
		exec_child_process(pipex, argv, argc, i);
	}
}

void	exec_child_process(t_pipex *pipex, char **argv, int argc, int i)
{
	int	fd;

	if (pipex->pid[i] == 0)
	{
		pipex->command = ft_parser(argv[i + 2 + pipex->heredoc]);
		if (i == 0)
		{
			fd = open_file(pipex->infile, 0, pipex);
			my_dup2(fd, pipex->pipeline[i][1], pipex);
			close(fd);
		}
		else if (i == pipex->num_commands - 1)
		{
			fd = open_file(argv[argc - 1], 1, pipex);
			my_dup2(pipex->pipeline[pipex->num_commands - 2][0], fd, pipex);
			close (fd);
		}
		else
			my_dup2(pipex->pipeline[i - 1][0], pipex->pipeline[i][1], pipex);
		close_pipes(pipex);
		execve(get_command_path(pipex), pipex->command, pipex->envp);
		free_all(pipex);
		free(pipex->cmd_path);
		exit(EXIT_FAILURE);
	}
}

void	free_all(t_pipex *pipex)
{
	free(pipex->pid);
	free_int_dubleptr(pipex->pipeline, pipex->num_commands - 1);
	free_char_dubleptr(pipex->command);
}
