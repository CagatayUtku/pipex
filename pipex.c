/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 16:50:15 by Cutku             #+#    #+#             */
/*   Updated: 2023/02/26 16:33:26 by Cutku            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	free_int_dubleptr(int **ptr, int numsize)
{
	int	i;

	i = 0;
	while (i < numsize)
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
	if (command[0] == '/' || command[0] == '.')
	{
		if (access(command, X_OK) == 0)
			return (command);
		else
			return (errno = ENOENT, NULL);
	}
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
	char **my_envp;
	int	i;

	i = 0;
	my_envp = envp;
	if (my_envp && *my_envp)
	{
		while (ft_strncmp(*my_envp + i, "PATH=", 5))
			i++;
		if (!(*my_envp + i))
			return ("/usr/bin");
		if (ft_strncmp(*my_envp + i, "PATH=", 5) == 0)
			return (*my_envp + i + 5); 
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
		{
			perror("pipex: input");
			exit(errno);
		}
	}
	if (flag == 1)
	{
		fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (fd == -1)
		{
			perror("pipex: output");
			exit(errno);
		}
	}
	return (fd);
}

void	my_dup2(int input, int output)
{
	if (dup2(input, STDIN_FILENO) == -1)
	{
		// perror("Dup2");
		exit(9);
	}
	close(input);
	if (dup2(output, STDOUT_FILENO) == -1)
	{
		// perror("Dup2");
		exit(9);
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

void	first_child(t_pipex *pipex, char **argv, char **envp)
{
	char	**command;
	int		fd;

	pipex->pid[0] = fork();
	if (pipex->pid[0] == -1)
	{
		perror("Fork");
		exit(EXIT_FAILURE);
	}
	else if (pipex->pid[0] == 0)
	{
		close(pipex->pipeline[0][0]);
		command = ft_split(argv[2], ' ');
		fd = open_file(argv[1], 0);
		my_dup2(fd, pipex->pipeline[0][1]);
		if (get_command_path(pipex->all_paths, command[0]) == NULL)
		{
			perror("command not found");
			exit(127);
		}
		execve(get_command_path(pipex->all_paths, command[0]), command, envp);
		perror("Execve");
		exit(EXIT_FAILURE);
	}
}

void	last_child(t_pipex *pipex, char **argv, char **envp, int argc)
{
	char	**command;
	int		fd;

	pipex->pid[argc - 4] = fork();
	if (pipex->pid[argc - 4] == -1)
	{
		perror("Fork");
		exit(EXIT_FAILURE);
	}
	else if (pipex->pid[argc - 4] == 0)
	{
		close(pipex->pipeline[argc - 5][1]);
		command = ft_split(argv[argc - 2], ' ');
		fd = open_file(argv[argc - 1], 1);
		my_dup2(pipex->pipeline[argc - 5][0], fd);
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
	