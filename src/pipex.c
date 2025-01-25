/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 13:54:05 by takitaga          #+#    #+#             */
/*   Updated: 2025/01/08 14:12:42 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	close_pipe(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_cmd	cmds_list;
	t_cmd	*cmd;
	int		infile_fd;
	int		outfile_fd;
	pid_t	pid;
	int		fd[2];
	_Bool	is_first_cmd;
	int		status;

	status = 0;

	if (argc < 5)
	{
		ft_printf("invalid args\n");
		return (1);
	}

	// splited_argsの配列を確保
	init_cmds_list(&cmds_list);
	split_cmds(argc, argv, envp, &cmds_list);

	is_first_cmd = true;
	cmd = &cmds_list;

	pipe(fd);
	while (cmd)
	{
		pid = fork();

		// @TODO: add case if fork has failed
		// child process
		if (pid == 0)
		{
			// if first cmd
			if (is_first_cmd)
			{
				close(fd[IN]);
				infile_fd = open(argv[1], O_RDONLY);
				dup2(infile_fd, STDIN_FILENO);
				close(infile_fd);
				dup2(fd[OUT], STDOUT_FILENO);
				close(fd[OUT]);
				if (execve(cmd->abs_path, cmd->cmd, envp) == -1)
					ft_printf("An error has occured in first cmd.\n");
			}
			// if last cmd
			else if (cmd->next == NULL)
			{
				close(fd[OUT]);
				dup2(fd[IN], STDIN_FILENO);
				close(fd[IN]);
				outfile_fd = open(argv[argc - 1], O_WRONLY);
				dup2(outfile_fd, STDOUT_FILENO);
				close(outfile_fd);
				if (execve(cmd->abs_path, cmd->cmd, envp) == -1)
					ft_printf("An error has occured in last cmd.\n");

			}
			// if mid cmd
			else
			{
				dup2(fd[IN], STDIN_FILENO);
				close(fd[IN]);
				dup2(fd[OUT], STDOUT_FILENO);
				close(fd[OUT]);
				if (execve(cmd->abs_path, cmd->cmd, envp) == -1)
					ft_printf("An error has occured in mid cmd.\n");
			}
		}
		// parent process
		cmd = cmd->next;
		is_first_cmd = false;
	}
	close_pipe(fd);
	while (wait(&status))
	{
		if (WIFEXITED(status)) {
			ft_printf("親プロセス : 子プロセスは終了ステータス%dで正常終了しました\n",
					WEXITSTATUS(status));
		}
		if (WIFSIGNALED(status)) {
			ft_printf("親プロセス : 子プロセスはシグナル番号%dで終了しました\n",
					WTERMSIG(status));
		}
	}
	return (0);
}
