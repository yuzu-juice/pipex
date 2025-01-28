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

void	close_pipe(int pipe_fd[2])
{
	close(pipe_fd[READ]);
	close(pipe_fd[WRITE]);
}

void	child_process(_Bool is_first_cmd, int pipe_fd[2][2], t_cmd *cmd, int argc, char *argv[], char *envp[])
{
	int	infile_fd;
	int	outfile_fd;

	if (is_first_cmd)
	{
		close(pipe_fd[CURR][READ]);
		infile_fd = open(argv[1], O_RDONLY);
		outfile_fd = pipe_fd[CURR][WRITE];
	}
	else if (cmd->next == NULL)
	{
		close(pipe_fd[PREV][WRITE]);
		infile_fd = pipe_fd[PREV][READ];
		outfile_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else
	{
		close(pipe_fd[PREV][WRITE]);
		close(pipe_fd[CURR][READ]);
		infile_fd = pipe_fd[PREV][READ];
		outfile_fd = pipe_fd[CURR][WRITE];
	}
	dup2(infile_fd, STDIN_FILENO);
	close(infile_fd);
	dup2(outfile_fd, STDOUT_FILENO);
	close(outfile_fd);
	if (execve(cmd->abs_path, cmd->cmd, envp) == -1)
		ft_printf("An error has occured.\n");
}

void	here_doc(char *limiter)
{
	char	*line;
	int 	tmp_fd;

	tmp_fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd < 0)
	{
		ft_printf("An error has occured.\n");
		exit(1);
	}
	while (true)
	{
		ft_printf("heredoc> ");
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (!ft_strncmp(line, limiter, ft_strlen(limiter)) && line[ft_strlen(limiter)] == '\n')
		{
			free(line);
			break ;
		}
		write(tmp_fd, line, ft_strlen(line));
	}
	close(tmp_fd);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_cmd	cmds_list;
	t_cmd	*cmd;
	pid_t	pid;
	int		pipe_fd[2][2];
	_Bool	is_first_cmd;

	if (!validate_input(argc, argv))
		return (1);
	init_cmds_list(&cmds_list, argc, argv, envp);
	cmd = &cmds_list;
	is_first_cmd = true;

	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		here_doc(argv[2]);
		argv[1] = ".heredoc_tmp";
		cmd = cmd->next;
	}

	while (cmd)
	{
		if (cmd->next != NULL)
			pipe(pipe_fd[CURR]);
		pid = fork();

		if (pid == 0)
			child_process(is_first_cmd, pipe_fd, cmd, argc, argv, envp);
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
	while (wait(NULL) > 0) ;
	if (ft_strncmp(argv[1], ".heredoc_tmp", 12) == 0)
		unlink(".heredoc_tmp");
	return (0);
}
