/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:48:34 by Cutku             #+#    #+#             */
/*   Updated: 2023/04/21 06:05:16 by Cutku            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc >= 5)
	{
		pipex.heredoc = 0;
		pipex.num_commands = argc - 3;
		pipex.infile = argv[1];
		pipex.envp = envp;
		if (ft_strncmp(argv[1], "here_doc", 9) == 0)
			exec_here_doc(&pipex, argv, argc);
		create_pipelines(&pipex, pipex.num_commands - 1);
		create_child_process(&pipex, argv, argc);
		close_pipes(&pipex);
		my_waitpid(&pipex);
	}
	else
		return (EINVAL);
	return (EXIT_SUCCESS);
}
