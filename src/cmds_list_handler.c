/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_list_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 23:59:26 by takitaga          #+#    #+#             */
/*   Updated: 2025/01/28 00:23:42 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_list_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 22:56:49 by takitaga          #+#    #+#             */
/*   Updated: 2025/01/10 20:56:49 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static t_cmd *get_last_cmd(t_cmd *cmds_list)
{
	t_cmd	*last;

	if (!cmds_list)
		return (NULL);
	last = cmds_list;
	while (last->next)
		last = last->next;
	return (last);
}

static void	append_cmd(char *str, t_cmd *cmds_list, char *envp[])
{
	char	**splited;
	t_cmd	*new_cmd;
	t_cmd	*last;

	splited = ft_split(str, ' ');
	if (cmds_list->cmd == NULL)
	{
		cmds_list->cmd = splited;
		cmds_list->abs_path = get_abs_path(splited[0], envp);
		return ;
	}
	new_cmd = malloc(sizeof(t_cmd));
	new_cmd->cmd = splited;
	new_cmd->abs_path = get_abs_path(splited[0], envp);
	new_cmd->next = NULL;
	last = get_last_cmd(cmds_list);
	last->next = new_cmd;
}

static void	split_cmds(int argc, char *argv[], char *envp[], t_cmd *cmds_list)
{
	size_t	i;
	size_t	cmd_count;

	i = 0;
	cmd_count = argc - 3;
	while (i < cmd_count)
	{
		append_cmd(argv[i + 2], cmds_list, envp);
		i++;
	}
}

void	init_cmds_list(t_cmd *cmds_list, int argc, char *argv[], char *envp[])
{
	cmds_list->cmd = NULL;
	cmds_list->abs_path = NULL;
	cmds_list->next = NULL;
	split_cmds(argc, argv, envp, cmds_list);
}

void	free_cmds_list(t_cmd *cmds_list)
{
	t_cmd	*tmp;
	t_cmd	*cmd;

	cmd = cmds_list;
	while (cmds_list)
	{
		tmp = cmds_list;
		cmd = cmd->next;
		free(tmp->cmd);
		free(tmp->abs_path);
		free(tmp);
	}
}
