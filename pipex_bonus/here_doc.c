/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 06:35:09 by Cutku             #+#    #+#             */
/*   Updated: 2023/04/17 05:54:04 by Cutku            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	exec_here_doc(t_pipex *pipex, char **argv, int argc)
{
	int		fake_fd;
	char	*ptr;

	fake_fd = open("my_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fake_fd < 0)
	{
		perror("open");
		exit (EXIT_FAILURE);
	}
	ptr = NULL;
	while (1)
	{
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		ptr = get_next_line(STDIN_FILENO);
		if (!ft_strncmp(argv[2], ptr, ft_strlen(ptr) - 1)
			&& ft_strlen(argv[2]) == ft_strlen(ptr) - 1)
			break ;
		ft_putstr_fd(ptr, fake_fd);
		free(ptr);
	}
	free(ptr);
	close(fake_fd);
	pipex->num_commands = argc - 4;
	pipex->infile = "my_heredoc";
	pipex->heredoc = 1;
}
