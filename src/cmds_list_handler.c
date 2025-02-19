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

static t_cmd	*get_last_cmd(t_cmd *cmds)
{
	t_cmd	*last;

	if (!cmds)
		return (NULL);
	last = cmds;
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

static _Bool	append_cmd(t_cmd *cmds, char *str, char *envp[])
{
	char	**splited;
	t_cmd	*new_cmd;
	t_cmd	*last;

	splited = ft_split(str, ' ');
	if (!splited)
		return (false);
	if (!remove_quotes(splited))
		return (free_string_array(splited), false);
	new_cmd = ft_calloc(sizeof(t_cmd), 1);
	if (!new_cmd)
		return (free_string_array(splited), false);
	new_cmd->cmd = splited;
	new_cmd->abs_path = get_abs_path(splited[0], envp);
	new_cmd->next = NULL;
	last = get_last_cmd(cmds);
	if (last->index == -1)
		new_cmd->index = 0;
	else
		new_cmd->index = last->index + 1;
	new_cmd->head = last->head;
	new_cmd->is_heredoc = last->is_heredoc;
	last->next = new_cmd;
	return (true);
}

static _Bool	split_cmds(int argc, char *argv[], char *envp[], t_cmd *cmds)
{
	size_t	i;
	size_t	cmd_count;

	i = 0;
	cmd_count = argc - 3;
	if (ft_strncmp(argv[1], HERE_DOC_FILE, ft_strlen(HERE_DOC_FILE)) == 0)
	{
		i++;
	}
	while (i < cmd_count)
	{
		if (!append_cmd(cmds, argv[i + 2], envp))
			return (false);
		i++;
	}
	return (true);
}

_Bool	init_cmds_list(t_cmd *cmds, int argc, char *argv[], char *envp[])
{
	cmds->cmd = NULL;
	cmds->abs_path = NULL;
	cmds->next = NULL;
	cmds->index = -1;
	cmds->head = cmds;
	if (ft_strncmp(argv[1], HERE_DOC_FILE, ft_strlen(HERE_DOC_FILE)) == 0)
		cmds->is_heredoc = true;
	else
		cmds->is_heredoc = false;
	if (!split_cmds(argc, argv, envp, cmds))
		return (false);
	return (true);
}
