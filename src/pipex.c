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


int	main(int argc, char *argv[], char *envp[])
{
	t_cmd	*cmd;
	t_cmd	*cmds_list;
	pid_t	pid;
	int		pipe_fd[2][2];
	_Bool	is_first_cmd;
	int		status;
	int		flag;
	char	*infile;
	char	*outfile;

	flag = 0;
	if (argc < 5)
		return (1);
	infile = argv[1];
	outfile = argv[argc - 1];
	cmd = ft_calloc(sizeof(t_cmd), 1);
	if (!cmd)
		return (1);
	cmds_list = cmd;
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (!here_doc(argv[2]))
			return (free(cmd), 1);
		argv[1] = HERE_DOC_FILE;
		init_cmds_list(cmd, argc - 1, argv + 1, envp);
	}
	else
		init_cmds_list(cmd, argc, argv, envp);
	is_first_cmd = true;

	while (cmd)
	{
		if (cmd->next != NULL)
			pipe(pipe_fd[CURR]);
		pid = fork();
		if (pid < 0)
			return (free_cmds_list(cmds_list), 1);
		if (pid == 0)
			child_process(is_first_cmd, pipe_fd, cmd, infile, outfile, envp, cmds_list);
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
	{
		if(!WIFEXITED(status))
			flag = 1;
	}
	if (ft_strncmp(argv[1], HERE_DOC_FILE, ft_strlen(HERE_DOC_FILE)) == 0)
		unlink(HERE_DOC_FILE);
	free_cmds_list(cmds_list);
	return (flag);
}
