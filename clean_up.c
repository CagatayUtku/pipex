/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 05:00:38 by Cutku             #+#    #+#             */
/*   Updated: 2023/04/16 05:02:01 by Cutku            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_pipes(t_pipex *pipex)
{
	int	i;

	i = -1;
	while (++i < pipex->num_commands - 1)
	{
		close(pipex->pipeline[i][0]);
		close(pipex->pipeline[i][1]);
	}
}

void	free_char_dubleptr(char **ptr)
{
	int	i;

	i = -1;
	while (ptr[++i] != NULL)
		free(ptr[i]);
	free(ptr);
}

void	free_int_dubleptr(int **ptr, int size)
{
	int	i;

	i = -1;
	while (++i < size)
		free(ptr[i]);
	free(ptr);
}
