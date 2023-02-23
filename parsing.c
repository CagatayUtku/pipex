/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 18:08:47 by Cutku             #+#    #+#             */
/*   Updated: 2023/02/23 11:55:19 by Cutku            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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
		my_dup2(fd, pipeline[1]);
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
		my_dup2(pipeline[0], fd);
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