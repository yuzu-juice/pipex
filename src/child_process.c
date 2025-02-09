/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 21:12:54 by takitaga          #+#    #+#             */
/*   Updated: 2025/02/03 21:13:44 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static void	first_cmd(int pfd[2][2], char *infile, t_cmd *cmds, t_file_fds *fds)
{
	fds->infile_fd = open(infile, O_RDONLY);
	if (fds->infile_fd < 0)
	{
		print_error(ERROR, infile);
		free_cmds_list(cmds);
		exit(EXIT_SUCCESS);
	}
	close(pfd[CURR][READ]);
	fds->outfile_fd = pfd[CURR][WRITE];
}

static void	last_cmd(int pfd[2][2], char *outfile, t_cmd *cmds, t_file_fds *fds)
{
	close(pfd[PREV][WRITE]);
	fds->infile_fd = pfd[PREV][READ];
	if (cmds->is_heredoc)
	{
		ft_printf("heredoc");
		fds->outfile_fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else
		fds->outfile_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fds->outfile_fd < 0)
	{
		print_error(ERROR, outfile);
		free_cmds_list(cmds);
		exit(EXIT_FAILURE);
	}
}

static void	middle_cmd(int pfd[2][2], t_file_fds *fds)
{
	close(pfd[PREV][WRITE]);
	close(pfd[CURR][READ]);
	fds->infile_fd = pfd[PREV][READ];
	fds->outfile_fd = pfd[CURR][WRITE];
}

void	child_process(int pfd[2][2], t_cmd *cmd, t_files files, char *envp[])
{
	t_file_fds	file_fds;

	if (cmd->index == 0)
		first_cmd(pfd, files.infile, cmd->head, &file_fds);
	else if (cmd->next == NULL)
		last_cmd(pfd, files.outfile, cmd->head, &file_fds);
	else
		middle_cmd(pfd, &file_fds);
	dup2_and_close(file_fds.infile_fd, STDIN_FILENO, cmd);
	dup2_and_close(file_fds.outfile_fd, STDOUT_FILENO, cmd);
	if (cmd->abs_path == NULL)
	{
		if (cmd->cmd[0] == NULL)
			print_error(CMD_NOT_FOUND, "");
		else
			print_error(CMD_NOT_FOUND, cmd->cmd[0]);
		free_cmds_list(cmd->head);
		exit(EXIT_SUCCESS);
	}
	if (execve(cmd->abs_path, cmd->cmd, envp) < 0)
	{
		free_cmds_list(cmd->head);
		exit(EXIT_FAILURE);
	}
}
