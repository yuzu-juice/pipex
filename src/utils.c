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

void	close_pipe(int pfd[2])
{
	close(pfd[READ]);
	close(pfd[WRITE]);
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

void	dup2_and_close(int old_fd, int new_fd, t_cmd *cmd)
{
	if (dup2(old_fd, new_fd) < 0)
	{
		close(old_fd);
		close(new_fd);
		free_cmds_list(cmd->head);
		exit(EXIT_FAILURE);
	}
	close(old_fd);
}
