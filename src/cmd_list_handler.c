/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 22:56:49 by takitaga          #+#    #+#             */
/*   Updated: 2025/01/10 20:56:49 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	init_cmd_list(t_cmd *cmd_list)
{
	cmd_list->cmd = NULL;
	cmd_list->abs_path = NULL;
	cmd_list->next = NULL;
}

size_t	cmd_list_len(t_cmd *cmd_list)
{
	size_t	count;

	if (!cmd_list->cmd)
		return (0);
	count = 1;
	while (cmd_list->next)
		count++;
	return (count);
}

t_cmd *get_last_cmd(t_cmd *cmd_list)
{
	t_cmd	*last;

	if (!cmd_list)
		return (NULL);
	last = cmd_list;
	while (last->next)
		last = last->next;
	return (last);
}

static void	append_cmd(char *str, t_cmd *cmd_list, char *envp[])
{
	char	**splited;
	t_cmd	*new_cmd;
	t_cmd	*last;

	splited = ft_split(str, ' ');
	if (cmd_list->cmd == NULL)
	{
		cmd_list->cmd = splited;
		cmd_list->abs_path = get_abs_path(splited[0], envp);
		// cmd_list->cmd[0] = cmd_list->abs_path;
		return ;
	}
	new_cmd = malloc(sizeof(t_cmd));
	new_cmd->cmd = splited;
	new_cmd->abs_path = get_abs_path(splited[0], envp);
	// new_cmd->cmd[0] = new_cmd->abs_path;
	new_cmd->next = NULL;
	last = get_last_cmd(cmd_list);
	last->next = new_cmd;
}

void	split_cmds(int argc, char *argv[], char *envp[], t_cmd *cmd_list)
{
	size_t	i;
	size_t	cmd_count;

	i = 0;
	cmd_count = argc - 3;
	while (i < cmd_count)
	{
		append_cmd(argv[i + 2], cmd_list, envp);
		i++;
	}
}
