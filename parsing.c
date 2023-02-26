/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 18:08:47 by Cutku             #+#    #+#             */
/*   Updated: 2023/02/25 16:34:04 by Cutku            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			if (pipex->fd == -1) return ;
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
			if (pipex->fd == -1) return ;
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