/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Cutku <cutku@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 05:19:04 by Cutku             #+#    #+#             */
/*   Updated: 2023/04/21 06:04:50 by Cutku            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	remove_escape(char *str)
{
	char	*ptr;

	ptr = str;
	if (ptr != NULL)
	{
		while (ft_strchr(ptr, '\\'))
		{
			ptr = ft_strchr(ptr, '\\');
			ft_memmove(ptr, ptr + 1, ft_strlen(ptr + 1) + 1);
			ptr++;
		}
	}
}

int	word_len(char *temp)
{
	int	len;

	len = 0;
	while (temp[len] != '\0' && temp[len] != '\'' \
		&& temp[len] != '\"' && temp[len] != ' ')
		len++;
	return (len);
}

char	**word_lenght(char *string, int size)
{
	int		i;
	char	*temp;
	char	**str;

	temp = string;
	i = 0;
	str = (char **)malloc((size + 1) * sizeof(char *));
	while (*temp != '\0')
	{
		while (*temp == ' ')
			temp++;
		if (*temp != '\0' && (*temp == '\'' || *temp == '\"'))
		{
			str[i] = ft_substr(temp, 1, ft_strrchr(temp, *temp) - temp - 1);
			temp = ft_strrchr(temp, *temp) + 1;
			remove_escape(str[i]);
			i++;
		}
		if (*temp != '\0' && *temp != '\'' && *temp != '\"' && *temp != ' ')
		{
			str[i++] = ft_substr(temp, 0, word_len(temp));
			temp += word_len(temp);
		}
	}
	return (str[i] = NULL, str);
}

char	**ft_parser(char *string)
{
	int		ct;
	char	*p;

	ct = 1;
	p = string;
	if (ft_strchr(p, ' ') != NULL)
	{
		while (*p != '\0')
		{
			while (*p == ' ')
				p++;
			if (*p == '\'' || *p == '\"')
			{
				p = ft_strrchr(p, *p);
				p++;
				ct++;
			}
			if (*p != ' ' && *p != '\'' && *p != '\"' && *p != '\0' && ct++)
				while (*p != '\0' && *p != ' ' && *p != '\'' && *p != '\"')
					p++;
		}
		return (word_lenght(string, ct - 1));
	}
	return (remove_escape(p), ft_split(string, ' '));
}
