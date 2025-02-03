/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 20:39:42 by takitaga          #+#    #+#             */
/*   Updated: 2025/02/03 21:13:52 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	close_pipe(int pipe_fd[2])
{
	close(pipe_fd[READ]);
	close(pipe_fd[WRITE]);
}

static void	free_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->cmd[i])
	{
		free(cmd->cmd[i]);
		i++;
	}
	free(cmd->cmd);
	free(cmd->abs_path);
}

void	free_cmds_list(t_cmd *cmds_list)
{
	t_cmd	*tmp;
	t_cmd	*cmd;

	cmd = cmds_list->next;
	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->next;
		free_cmd(tmp);
		free(tmp);
	}
}

void	free_string_array(char **str_arr)
{
	size_t	i;

	i = 0;
	while (str_arr[i])
	{
		free(str_arr[i]);
		i++;
	}
	free(str_arr);
}
