/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 00:36:23 by takitaga          #+#    #+#             */
/*   Updated: 2025/02/05 00:42:12 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	parent_process(int pipe_fd[][2], t_cmd *cmd, _Bool is_first_cmd)
{
	if (!is_first_cmd)
		close_pipe(pipe_fd[PREV]);
	if (cmd->next)
	{
		pipe_fd[PREV][READ] = pipe_fd[CURR][READ];
		pipe_fd[PREV][WRITE] = pipe_fd[CURR][WRITE];
	}
	else
		close_pipe(pipe_fd[CURR]);
}
