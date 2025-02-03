/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 13:54:05 by takitaga          #+#    #+#             */
/*   Updated: 2025/01/28 00:12:42 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static int	pipex(int argc, char *argv[], char *envp[])
{
	t_cmd	cmds_list;
	t_cmd	*cmd;
	pid_t	pid;
	int		pipe_fd[2][2];
	_Bool	is_first_cmd;
	int		status;
	int		flag;

	flag = 0;
	init_cmds_list(&cmds_list, argc, argv, envp);
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (!here_doc(argv[2]))
			return (free_cmds_list(&cmds_list), 1);
		argv[1] = HERE_DOC_FILE;
	}
	is_first_cmd = true;
	cmd = &cmds_list;
	while (cmd)
	{
		if (cmd->next != NULL)
			pipe(pipe_fd[CURR]);
		pid = fork();
		if (pid < 0)
			return (free_cmds_list(&cmds_list), 1);
		if (pid == 0)
			child_process(is_first_cmd, pipe_fd, cmd, argv[1], argv[argc - 1], envp, &cmds_list);
		if (!is_first_cmd)
			close_pipe(pipe_fd[PREV]);
		if (cmd->next == NULL)
			close_pipe(pipe_fd[CURR]);
		else
		{
			pipe_fd[PREV][READ] = pipe_fd[CURR][READ];
			pipe_fd[PREV][WRITE] = pipe_fd[CURR][WRITE];
		}
		cmd = cmd->next;
		is_first_cmd = false;
	}
	while (wait(&status) > 0)
		if (!WIFEXITED(status))
			flag = 1;
	if (ft_strncmp(argv[1], HERE_DOC_FILE, ft_strlen(HERE_DOC_FILE)) == 0)
		unlink(HERE_DOC_FILE);
	return (free_cmds_list(&cmds_list), flag);
}

int	main(int argc, char *argv[], char *envp[])
{
	if (argc < 5)
		return (1);
	return (pipex(argc, argv, envp));
}
