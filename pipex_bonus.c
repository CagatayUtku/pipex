/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 06:35:09 by Cutku             #+#    #+#             */
/*   Updated: 2023/02/26 16:24:56 by Cutku            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	mid_child(t_pipex *pipex, char **argv, char **envp, int argc, int i)
{
	char	**command;

	pipex->pid[i] = fork();
	if (pipex->pid[i] == -1)
	{
		perror("Fork");
		exit(EXIT_FAILURE);
	}
	else if (pipex->pid[i] == 0)
	{
		close(pipex->pipeline[i - 1][1]);
		command = ft_split(argv[argc - 2], ' ');
		my_dup2(pipex->pipeline[i - 1][0], pipex->pipeline[i][1]);
		if (get_command_path(pipex->all_paths, command[0]) == NULL)
		{
			perror("command not found");
			exit(127);
		}
		execve(get_command_path(pipex->all_paths, command[0]), command, envp);
		perror("Execve error");
		exit(EXIT_FAILURE);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex pipex;
	int		i;
	int		status;

	if (argc >= 5)
	{
		create_pipelines(&pipex, argc - 4);
		pipex.pid = malloc(sizeof(pid_t) * argc - 3);
		if (pipex.pid == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		pipex.all_paths = get_env_path(envp);
		first_child(&pipex, argv, envp);
		i = 0;
		while (++i < argc - 3)
			mid_child(&pipex, argv, envp, argc, i);
		last_child(&pipex, argv, envp, argc);
		i = -1;
		while (++i < argc - 4)
		{
			close(pipex.pipeline[i][0]);
			close(pipex.pipeline[i][1]);
		}
		free_int_dubleptr(pipex.pipeline, argc - 4);
		free(pipex.pid);
		i = -1;
		while (++i < argc - 3)
			waitpid(pipex.pid[i], &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
	}
	else
	{
		errno = EINVAL;
		perror("Invalid argument");
		return (errno);
	}
	return (EXIT_SUCCESS);
}