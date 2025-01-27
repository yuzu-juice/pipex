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
	t_cmd	cmds_list;
	t_cmd	*cmd;
	int		infile_fd;
	int		outfile_fd;
	pid_t	pid;
	int		fd[2];
	_Bool	is_first_cmd;
	int		status;

	status = 0;

	if (!validate_input(argc, argv))
		return (1);

	// splited_argsの配列を確保
	init_cmds_list(&cmds_list);
	split_cmds(argc, argv, envp, &cmds_list);

	is_first_cmd = true;
	cmd = &cmds_list;

	while (cmd)
	{
		pipe(fd);
		pid = fork();
		if (is_first_cmd)
		{
			// child
			if (pid == 0)
			{
				close(fd[READ]);
				infile_fd = open(argv[1], O_RDONLY);
				dup2(infile_fd, STDIN_FILENO);
				close(infile_fd);
				dup2(fd[WRITE], STDOUT_FILENO);
				close(fd[WRITE]);
				if (execve(cmd->abs_path, cmd->cmd, envp) == -1)
					ft_printf("An error has occured in first cmd.\n");
			}
			// parent
			else
			{
				close(fd[WRITE]);
				close(STDIN_FILENO);
				dup2(fd[READ], STDIN_FILENO);
				close(fd[READ]);
				waitpid(pid, NULL, 0);
			}
		}
		else if (cmd->next == NULL)
		{
			outfile_fd = open(argv[argc - 1], O_WRONLY);
			dup2(outfile_fd, STDOUT_FILENO);
			close(outfile_fd);
			if (execve(cmd->abs_path, cmd->cmd, envp) == -1)
				ft_printf("An error has occured in last cmd.\n");
		}
		else
		{
			if (pid == 0)
			{
				dup2(fd[WRITE], STDOUT_FILENO);
				if (execve(cmd->abs_path, cmd->cmd, envp) == -1)
					ft_printf("An error has occured in mid cmd.\n");
			}
			else
			{
				waitpid(pid, NULL, 0);
				dup2(fd[READ], STDIN_FILENO);
			}
		}
		// parent process
		cmd = cmd->next;
		is_first_cmd = false;
	}
	return (0);
}
