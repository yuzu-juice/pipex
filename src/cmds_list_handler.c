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

#include "../include/pipex.h"

static t_cmd	*get_last_cmd(t_cmd *cmds_list)
{
	t_cmd	*last;

	if (!cmds_list)
		return (NULL);
	last = cmds_list;
	while (last->next)
		last = last->next;
	return (last);
}

static _Bool	remove_quotes(char **tokens)
{
	int		i;
	size_t	len;
	char	*tmp;

	i = 0;
	while (tokens[i])
	{
		len = ft_strlen(tokens[i]);
		if (len >= 2 && ((tokens[i][0] == '\'' && tokens[i][len - 1] == '\'') ||
			(tokens[i][0] == '"' && tokens[i][len - 1] == '"')))
		{
			tmp = tokens[i];
			tokens[i] = ft_substr(tokens[i], 1, len - 2);
			free(tmp);
			if (tokens[i] == NULL)
				return (false);
		}
		i++;
	}
	return (true);
}

static _Bool	append_cmd(char *str, t_cmd *cmds_list, char *envp[])
{
	char	**splited;
	t_cmd	*new_cmd;
	t_cmd	*last;

	splited = ft_split(str, ' ');
	if (!splited)
		return (false);
	if (!remove_quotes(splited))
		return (free_string_array(splited), false);
	if (cmds_list->cmd == NULL)
	{
		cmds_list->cmd = splited;
		cmds_list->abs_path = get_abs_path(splited[0], envp);
		return (true);
	}
	new_cmd = ft_calloc(sizeof(t_cmd), 1);
	if (!new_cmd)
		return (free_string_array(splited), false);
	new_cmd->cmd = splited;
	new_cmd->abs_path = get_abs_path(splited[0], envp);
	if (new_cmd->abs_path == NULL)
		return (free_string_array(splited), free(new_cmd), false);
	new_cmd->next = NULL;
	last = get_last_cmd(cmds_list);
	last->next = new_cmd;
	return (true);
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
