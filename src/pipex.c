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

void	close_pipe(int pipe_fd[2])
{
	close(pipe_fd[READ]);
	close(pipe_fd[WRITE]);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_cmd	cmds_list;
	t_cmd	*cmd;
	int		infile_fd;
	int		outfile_fd;
	pid_t	pid;
	int		prev_pipe_fd[2];
	int		current_pipe_fd[2];
	_Bool	is_first_cmd;

	if (!validate_input(argc, argv))
		return (1);

	// splited_argsの配列を確保
	init_cmds_list(&cmds_list);
	split_cmds(argc, argv, envp, &cmds_list);

	is_first_cmd = true;
	cmd = &cmds_list;

	while (cmd)
	{
		if (cmd->next != NULL)
			pipe(current_pipe_fd);
		pid = fork();

		// child process
		if (pid == 0)
		{
			if (is_first_cmd)
			{
				close(current_pipe_fd[READ]);
				infile_fd = open(argv[1], O_RDONLY);
				outfile_fd = current_pipe_fd[WRITE];
			}
			else if (cmd->next == NULL)
			{
				close(prev_pipe_fd[WRITE]);
				infile_fd = prev_pipe_fd[READ];
				outfile_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			}
			else
			{
				close(prev_pipe_fd[WRITE]);
				close(current_pipe_fd[READ]);
				infile_fd = prev_pipe_fd[READ];
				outfile_fd = current_pipe_fd[WRITE];
			}
			dup2(infile_fd, STDIN_FILENO);
			close(infile_fd);
			dup2(outfile_fd, STDOUT_FILENO);
			close(outfile_fd);
			if (execve(cmd->abs_path, cmd->cmd, envp) == -1)
				ft_printf("An error has occured.\n");
		}
		// parent process
		else
		{
			if (!is_first_cmd)
				close_pipe(prev_pipe_fd);
			if (cmd->next == NULL)
				close_pipe(current_pipe_fd);
			else
			{
				prev_pipe_fd[READ] = current_pipe_fd[READ];
				prev_pipe_fd[WRITE] = current_pipe_fd[WRITE];
			}
			waitpid(pid, NULL, 0);
			cmd = cmd->next;
			is_first_cmd = false;
		}
	}
	return (0);
}
