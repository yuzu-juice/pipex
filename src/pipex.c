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

int	main(int argc, char *argv[], char *envp[])
{
	(void)envp;
	t_cmd	cmd_list;
	t_cmd	*cmd;
	char	*infile_name;
	int		infile_fd;
	int		outfile_fd;
	char	*outfile_name;
	pid_t	pid;
	int		fd[2];

	pipe(fd);
	infile_name = argv[1];
	outfile_name = argv[argc - 1];

	// splited_argsの配列を確保
	cmd_list.cmd = NULL;
	cmd_list.abs_path = NULL;
	cmd_list.next = NULL;
	split_cmds(argc, argv, envp, &cmd_list);

	// 最初のコマンド実行
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		infile_fd = open(infile_name, O_RDONLY);
		dup2(infile_fd, STDIN_FILENO);
		close(infile_fd);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execve(cmd_list.abs_path, cmd_list.cmd, envp);
	}

	// // 途中のコマンド実行
	// // パイプを増やして対応
	// cmd = cmd_list.next;
	// while (cmd->next)
	// {
	// 	pid = fork();
	// 	if (pid == 0)
	// 	{
	// 		dup2(fd[0], STDIN_FILENO);
	// 		close(fd[0]);
	// 		dup2(fd[1], STDOUT_FILENO);
	// 		close(fd[1]);
	// 		execve(cmd->abs_path, cmd->cmd, envp);
	// 	}
	// 	cmd = cmd->next;
	// }

	// 最後のコマンド実行
	pid = fork();
	if (pid == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		outfile_fd = open(outfile_name, O_WRONLY);
		dup2(outfile_fd, STDOUT_FILENO);
		close(outfile_fd);
		cmd = get_last_cmd(&cmd_list);
		execve(cmd->abs_path, cmd->cmd, envp);
	}
	close(fd[0]);
	close(fd[1]);
	while (wait(NULL) > 0);

	return (0);
}
