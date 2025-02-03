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

_Bool	here_doc(char *limiter)
{
	char	*line;
	int		tmp_fd;

	tmp_fd = open(HERE_DOC_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd < 0)
		return (print_error(ERROR, HERE_DOC_FILE), false);
	while (true)
	{
		ft_printf("> ");
		line = get_next_line(STDIN_FILENO);
		if (!line)
			return (close(tmp_fd), false);
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
		{
			free(line);
			break ;
		}
		write(tmp_fd, line, ft_strlen(line));
		free(line);
	}
	close(tmp_fd);
	return (true);
}
