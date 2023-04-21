/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 06:44:20 by Cutku             #+#    #+#             */
/*   Updated: 2023/04/20 06:10:00 by Cutku            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc == 5)
	{
		pipex.heredoc = 0;
		pipex.num_commands = argc - 3;
		pipex.infile = argv[1];
		pipex.envp = envp;
		create_pipelines(&pipex, pipex.num_commands - 1);
		create_child_process(&pipex, argv, argc);
		close_pipes(&pipex);
		my_waitpid(&pipex);
	}
	else
		return (EINVAL);
	return (EXIT_SUCCESS);
}

//i have allocated space for pipelines in parent
//i have allocated space for pid`s in parent

//allocated space for command char** in child process
//allocated space for command path char* in child process
