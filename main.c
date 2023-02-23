/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 16:50:15 by Cutku             #+#    #+#             */
/*   Updated: 2023/02/23 17:16:35 by Cutku            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <errno.h>

void	free_dubleptr(char **ptr)
{
	int	i;

	i = 0;
	while (ptr[i] != NULL)
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

char	*get_command_path(char *envp, char *command)
{
	int		i;
	char	**all_paths;
	char	*real_path;
	int		j;

	i = 0;
	all_paths = ft_split(envp, ':');
	while (all_paths[i] != NULL)
	{
		j = 0;
		all_paths[i] = ft_strjoin(all_paths[i], "/");
		real_path = ft_strjoin(all_paths[i], command);
		if (access(real_path, F_OK) == 0)
		{
			if (access(real_path, X_OK) == 0)
				return (free_dubleptr(all_paths), real_path);
			else
			{
				perror("No permission to execute");
				exit(126);
			}
		}
		free(real_path);
		i++;
	}
	errno = ENOENT;
	return (free_dubleptr(all_paths), NULL);
}

char	*get_env_path(char **envp)
{
	int	i;

	i = 0;
	if (envp && envp[i])
	{
		while (ft_strncmp(*envp + i, "PATH=", 5))
			i++;
		if (!(envp[i]))
			return ("/usr/bin");
		if (ft_strncmp(*envp + i, "PATH=", 5) == 0)
			return (*envp + i + 5); 
	}
	return ("/usr/bin");
}

int	open_file(char *filename, int flag)
{
	int fd;

	if (flag == 0)
	{
		fd = open(filename, O_RDONLY);
		if (fd == -1)
			perror("File");		
	}
	if (flag == 1)
	{
		fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (fd == -1)
			perror("File");
	}
	return (fd);
}

void	my_dup2(int input, int output)
{
	if (dup2(input, STDIN_FILENO) == -1)
	{
		perror("Dup2");
		exit(EXIT_FAILURE);
	}
	close(input);
	if (dup2(output, STDOUT_FILENO) == -1)
	{
		perror("Dup2");
		exit(EXIT_FAILURE);
	}
	close(output);
}

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
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		if (pipe(pipex->pipeline[i]) == -1)
		{
			perror("Pipe");
			exit(EXIT_FAILURE);
		}
	}
}

void	child_process(t_pipex *pipex, int i, char **argv, int argc, char **envp)
{
	pipex->pid[i] = fork();
	if (pipex->pid[i] == -1)
	{
		perror("Fork");
		exit(EXIT_FAILURE);
	}
	if (pipex->pid[i] == 0)
	{
		pipex->command = ft_split(argv[i + 2], ' ');
		if (i == 0)
		{
			close(pipex->pipeline[0][0]);
			pipex->fd = open_file(argv[1], 0);
			my_dup2(pipex->fd, pipex->pipeline[0][1]);
			if (get_command_path(pipex->my_envp, pipex->command[0]) == NULL)
			{
				perror("Command not found");
				exit(127);
			}
			execve(get_command_path(pipex->my_envp, pipex->command[0]), pipex->command, envp);
			perror("Execve error");
			exit(EXIT_FAILURE);
		}
		if (i == argc - 4)
		{
			close(pipex->pipeline[0][1]);
			pipex->fd = open_file(argv[argc - 1], 1);
			my_dup2(pipex->pipeline[0][0], pipex->fd);
			if (get_command_path(pipex->my_envp, pipex->command[0]) == NULL)
			{
				perror("Command not found");
				exit(127);
			}
			execve(get_command_path(pipex->my_envp, pipex->command[0]), pipex->command, envp);
			perror("Execve error");
			exit(EXIT_FAILURE);
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex pipex;
	int		i;
	int		status;

	if (argc == 5)
	{
		create_pipelines(&pipex, argc - 4);
		pipex.pid = malloc(sizeof(pid_t) * argc - 3);
		pipex.my_envp = get_env_path(envp);
		if (pipex.pid == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		i = -1;
		while (++i < argc - 3)
			child_process(&pipex, i, argv, argc, envp);
		i = -1;
		while (++i < argc - 4)
		{
			close(pipex.pipeline[i][0]);
			close(pipex.pipeline[i][1]);
		}
		i = -1;
		while (++i < argc - 3)
			waitpid(pipex.pid[i], &status, 0);
		// wait(NULL);
	}
	else
	{
		errno = EINVAL;
		perror("Invalid argument");
		return (EINVAL);
	}
	return (EXIT_SUCCESS);
}
	