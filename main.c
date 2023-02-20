/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 16:50:15 by Cutku             #+#    #+#             */
/*   Updated: 2023/02/20 17:59:01 by Cutku            ###   ########.fr       */
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
	if (envp && *envp)
	{
		while (ft_strncmp(*envp + i, "PATH=", 5))
			i++;
		if (!(*envp + i))
			return ("/usr/bin");
		if (ft_strncmp(*envp + i, "PATH=", 5) == 0)
			i += 5;
		return (*envp + i);
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
			perror("File");
			exit(errno);
		}		
	}
	if (flag == 1)
	{
		fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (fd == -1)
		{
			perror("File");
			exit(errno);
		}
	}
	return (fd);
}

void	multiple_command(int argc, char **argv, char **envp)
{
	
}

void	first_child(int	pipeline[2], int argc, char **argv, char **envp)
{
	char	**command;
	char	*my_envp;
	pid_t	pid;
	int		fd;

	pid = fork();
	my_envp = get_env_path(envp);
	if (pid == -1)
	{
		perror("Fork");
		exit(EXIT_FAILURE);	
	}
	else if (pid == 0)
	{
		close(pipeline[0]);
		command = ft_split(argv[2], ' ');
		fd = open_file(argv[1], 0);
		dup2(fd, STDIN_FILENO);
		close(fd);
		dup2(pipeline[1], STDOUT_FILENO);
		close(pipeline[1]);
		if (get_command_path(my_envp, command[0]) == NULL)
		{
			perror("Command not found");
			exit(127);
		}
		execve(get_command_path(my_envp, command[0]), command, envp);
		perror("Execve error");
		exit(EXIT_FAILURE);
	}
}
// void	mid_child()
// {

// }
void	last_child(int	pipeline[2], int argc, char **argv, char **envp)
{
	char	**command;
	char	*my_envp;
	pid_t	pid;
	int		fd;

	pid = fork();
	my_envp = get_env_path(envp);
	if (pid == -1)
	{
		perror("Fork");
		exit(EXIT_FAILURE);
	}
	if (pid > 0)
	{
		close(pipeline[1]);
		command = ft_split(argv[argc - 2], ' ');
		fd = open_file(argv[argc - 1], 1);
		dup2(pipeline[0], STDIN_FILENO);
		close(pipeline[0]);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		if (get_command_path(my_envp, command[0]) == NULL)
		{
			perror("Command not found");
			exit(127);
		}
		execve(get_command_path(my_envp, command[0]), command, envp);
		perror("Execve error");
		exit(EXIT_FAILURE);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		pipeline[2];
	char	**my_envp;

	if (argc == 5)
	{
		if (pipe(pipeline) == -1)
		{
			perror("Pipe Error :");
			exit(EXIT_FAILURE);
		}
		first_child(pipeline, argc, argv, envp);
		last_child(pipeline, argc, argv, envp);
	}
	else
	{
		errno = EINVAL;
		perror("Invalid argument");
		return (EINVAL);
	}
	close(pipeline[0]);
	close(pipeline[1]);
	wait(NULL);
	return (EXIT_SUCCESS);
}
