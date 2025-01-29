/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 00:03:47 by takitaga          #+#    #+#             */
/*   Updated: 2025/01/30 00:04:19 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	here_doc(char *limiter)
{
	char	*line;
	int 	tmp_fd;

	tmp_fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd < 0)
	{
		ft_printf("An error has occured.\n");
		exit(1);
	}
	while (true)
	{
		ft_printf("heredoc> ");
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (!ft_strncmp(line, limiter, ft_strlen(limiter)) && line[ft_strlen(limiter)] == '\n')
		{
			free(line);
			break ;
		}
		write(tmp_fd, line, ft_strlen(line));
	}
	close(tmp_fd);
}
