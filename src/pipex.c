/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 13:54:05 by takitaga          #+#    #+#             */
/*   Updated: 2025/02/04 00:12:42 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static void	finalize(char *here_doc, t_cmd *cmds_list)
{
	if (ft_strncmp(here_doc, HERE_DOC_FILE, ft_strlen(HERE_DOC_FILE)) == 0)
		unlink(HERE_DOC_FILE);
	free_cmds_list(cmds_list);
}

static int	handle_cmd(t_cmd *cmd, int argc, char *argv[], char *envp[])
{
	pid_t	pid;
	int		pfd[2][2];

	while (cmd)
	{
		if (cmd->next)
			pipe(pfd[CURR]);
		pid = fork();
		if (pid < 0)
			return (1);
		if (pid == 0)
			child_process(pfd, cmd, (t_files){argv[1], argv[argc - 1]}, envp);
		else
			parent_process(pfd, cmd);
		cmd = cmd->next;
	}
	return (0);
}

static int	pipex(int argc, char *argv[], char *envp[])
{
	t_cmd	cmd_head;
	t_cmd	*cmd;
	int		status;
	int		ret_val;

	ret_val = 0;
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (!here_doc(argv[2]))
			return (1);
		argv[1] = HERE_DOC_FILE;
	}
	if (!init_cmds_list(&cmd_head, argc, argv, envp))
		return (finalize(argv[1], &cmd_head), 1);
	cmd = cmd_head.next;
	ret_val = handle_cmd(cmd, argc, argv, envp);
	while (wait(&status) > 0)
		if (!WIFEXITED(status))
			ret_val = 1;
	return (finalize(argv[1], &cmd_head), ret_val);
}

int	main(int argc, char *argv[], char *envp[])
{
	if (argc < 5)
		return (1);
	return (pipex(argc, argv, envp));
}
