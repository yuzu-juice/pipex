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

static int	handle_cmd(t_cmd *cmd, t_cmd *cmds_list, int argc, char *argv[], char *envp[])
{
	pid_t	pid;
	int		pipe_fd[2][2];

	while (cmd)
	{
		if (cmd->next)
			pipe(pipe_fd[CURR]);
		pid = fork();
		if (pid < 0)
			return (1);
		if (pid == 0)
			child_process(pipe_fd, cmd, argv[1], argv[argc - 1], envp, cmds_list);
		else
			parent_process(pipe_fd, cmd);
		cmd = cmd->next;
	}
	return (0);
}

static int	pipex(int argc, char *argv[], char *envp[])
{
	t_cmd	cmds_list;
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
	if (!init_cmds_list(&cmds_list, argc, argv, envp))
		return (finalize(argv[1], &cmds_list), 1);
	cmd = cmds_list.next;
	ret_val = handle_cmd(cmd, &cmds_list, argc, argv, envp);
	while (wait(&status) > 0)
		if (!WIFEXITED(status))
			ret_val = 1;
	return (finalize(argv[1], &cmds_list), ret_val);
}

int	main(int argc, char *argv[], char *envp[])
{
	if (argc < 5)
		return (1);
	return (pipex(argc, argv, envp));
}
