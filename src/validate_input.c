/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:07:12 by takitaga          #+#    #+#             */
/*   Updated: 2025/01/08 14:19:25 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

_Bool	validate_input(int argc, char *argv[])
{
	int tmp_fd;

	if (argc < 5)
		return (false);
	if (ft_strncmp(argv[1], "here_doc", 8) != 0)
	{
		if (access(argv[1], R_OK) == -1)
			perror(argv[1]);
		if (access(argv[argc - 1], F_OK) == -1)
		{
			tmp_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			close(tmp_fd);
		}
	}
	if (access(argv[argc - 1], W_OK) == -1)
	{
		perror(argv[argc - 1]);
		return (false);
	}
	return (true);
}
