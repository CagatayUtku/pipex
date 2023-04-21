/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 04:59:37 by Cutku             #+#    #+#             */
/*   Updated: 2023/04/21 06:06:56 by Cutku            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	my_dup2(int input, int output, t_pipex *pipex)
{
	if (dup2(input, STDIN_FILENO) == -1)
	{
		perror("dup2");
		free_all(pipex);
		exit(9);
	}
	close(input);
	if (dup2(output, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		free_all(pipex);
		exit(9);
	}
	close(output);
}

void	my_waitpid(t_pipex *pipex)
{
	int	i;
	int	status;

	i = -1;
	while (++i < pipex->num_commands)
		waitpid(pipex->pid[i], &status, 0);
	if (pipex->heredoc == 1)
		unlink("my_heredoc");
	free(pipex->pid);
	free_int_dubleptr(pipex->pipeline, pipex->num_commands - 1);
	if (WIFEXITED(status))
		exit (WEXITSTATUS(status));
}

int	open_file(char *filename, int flag, t_pipex *pipex)
{
	int	fd;

	if (flag == 0)
	{
		fd = open(filename, O_RDONLY);
		if (fd < 0)
		{
			free_all(pipex);
			perror("pipex: input");
			exit(EXIT_FAILURE);
		}
		return (fd);
	}
	if (pipex->heredoc == 1)
		fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
		fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0 || (access(filename, W_OK) != 0))
	{
		free_all(pipex);
		perror("pipex: output");
		exit(EXIT_FAILURE);
	}
	return (fd);
}
