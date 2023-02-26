/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 06:44:20 by Cutku             #+#    #+#             */
/*   Updated: 2023/02/26 07:12:57 by Cutku            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex pipex;
	int		i;
	int		status;

	if (argc == 5)
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
		last_child(&pipex, argv, envp, argc);
		close(pipex.pipeline[0][0]);
		close(pipex.pipeline[0][1]);
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