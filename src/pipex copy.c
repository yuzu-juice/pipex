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
	char	*infile_name;
	char	*outfile_name;
	int		infile_fd;
	int		outfile_fd;
	pid_t	pid;
	int		fd[2][2];
	_Bool	use_fd;
	_Bool	is_first_cmd;

	infile_name = argv[1];
	outfile_name = argv[argc - 1];

	// splited_argsの配列を確保
	init_cmds_list(&cmds_list);
	split_cmds(argc, argv, envp, &cmds_list);

	is_first_cmd = true;
	use_fd = true;
	cmd = &cmds_list;

	pipe(fd[use_fd]);
	pipe(fd[!use_fd]);
	while (cmd)
	{
		pid = fork();

		// @TODO: add case if fork has failed
		// child process
		if (pid == 0)
		{
			// first command case
			if (is_first_cmd)
			{
				close_pipe(fd[use_fd]);
				close(fd[!use_fd][IN]);
				infile_fd = open(infile_name, O_RDONLY);
				dup2(infile_fd, STDIN_FILENO);
				close(infile_fd);
				dup2(fd[!use_fd][OUT], STDOUT_FILENO);
				close(fd[!use_fd][OUT]);
				if (execve(cmd->abs_path, cmd->cmd, envp) == -1)
					ft_printf("%s\n", "An erroSTDIN_FILENOr has occured in first cmd.");
			}
			// last command case
			else if (cmd->next == NULL)
			{
				close_pipe(fd[!use_fd]);
				close(fd[use_fd][OUT]);
				dup2(fd[use_fd][IN], STDIN_FILENO);
				close(fd[use_fd][IN]);
				outfile_fd = open(outfile_name, O_WRONLY);
				dup2(outfile_fd, STDOUT_FILENO);
				close(outfile_fd);
				if (execve(cmd->abs_path, cmd->cmd, envp) == -1)
					ft_printf("%s\n", "An error has occured in mid cmd.");
			}
			// other case
			else
			{
				close(fd[use_fd][OUT]);
				close(fd[!use_fd][IN]);
				dup2(fd[use_fd][IN], STDIN_FILENO);
				close(fd[use_fd][IN]);
				dup2(fd[!use_fd][OUT], STDOUT_FILENO);
				close(fd[!use_fd][OUT]);
				if (execve(cmd->abs_path, cmd->cmd, envp) == -1)
					ft_printf("%s\n", "An error has occured in last cmd.");
			}
		}
		//parent processin
		cmd = cmd->next;
		use_fd = !use_fd;
		is_first_cmd = false;
		close_pipe(fd[use_fd]);
		close_pipe(fd[!use_fd]);
	}

	// // 最初のコマンド実行
	// pid = fork();
	// if (pid == 0)
	// {
	// 	close(fd1[0]);
	// 	infile_fd = open(infile_name, O_RDONLY);
	// 	dup2(infile_fd, STDIN_FILENO);
	// 	close(infile_fd);
	// 	dup2(fd1[1], STDOUT_FILENO);
	// 	close(fd1[1]);
	// 	execve(cmds_list.abs_path, cmds_list.cmd, envp);
	// }

	// // 途中のコマンド実行
	// cmd = cmds_list.next;
	// pipe(fd2);
	// while (cmd->next)
	// {
	// 	pid = fork();
	// 	if (pid == 0)
	// 	{
	// 		close(fd1[1]);
	// 		dup2(fd1[0], STDIN_FILENO);
	// 		close(fd1[0]);
	// 		close(fd2[0]);
	// 		dup2(fd2[1], STDOUT_FILENO);
	// 		close(fd2[1]);
	// 		execve(cmd->abs_path, cmd->cmd, envp);
	// 	}
	// 	cmd = cmd->next;
	// }
	// close(fd1[0]);
	// close(fd1[1]);

	// // 最後のコマンド実行
	// pid = fork();
	// if (pid == 0)
	// {
	// 	close(fd2[1]);
	// 	dup2(fd2[0], STDIN_FILENO);
	// 	close(fd2[0]);
	// 	outfile_fd = open(outfile_name, O_WRONLY);
	// 	dup2(outfile_fd, STDOUT_FILENO);
	// 	close(outfile_fd);
	// 	cmd = get_last_cmd(&cmds_list);
	// 	execve(cmd->abs_path, cmd->cmd, envp);
	// }

	// close(fd2[0]);
	// close(fd2[1]);
	while (wait(NULL) > 0) ;

	return (0);
}
