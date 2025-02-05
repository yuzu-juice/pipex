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

static void	handle_first_cmd(int pfd[2][2], char *infile, t_cmd *cmds,  int *infile_fd, int *outfile_fd)
{
	*infile_fd = open(infile, O_RDONLY);
	if (*infile_fd < 0)
	{
		print_error(ERROR, infile);
		free_cmds_list(cmds);
		exit(EXIT_FAILURE);
	}
	close(pfd[CURR][READ]);
	*outfile_fd = pfd[CURR][WRITE];
}

static void	handle_last_cmd(int pfd[2][2], char *outfile, t_cmd *cmds_list, int *infile_fd, int *outfile_fd)
{
	close(pfd[PREV][WRITE]);
	*infile_fd = pfd[PREV][READ];
	*outfile_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*outfile_fd < 0)
	{
		print_error(ERROR, outfile);
		free_cmds_list(cmds_list);
		exit(EXIT_FAILURE);
	}
}

static void	handle_middle_cmd(int pfd[2][2], int *infile_fd, int *outfile_fd)
{
	close(pfd[PREV][WRITE]);
	close(pfd[CURR][READ]);
	*infile_fd = pfd[PREV][READ];
	*outfile_fd = pfd[CURR][WRITE];
}

void	child_process(int pfd[2][2], t_cmd *cmd, t_files files, char *envp[])
{
	int	infile_fd;
	int	outfile_fd;

	if (cmd->index == 0)
		handle_first_cmd(pfd, files.infile, cmd->head, &infile_fd, &outfile_fd);
	else if (cmd->next == NULL)
		handle_last_cmd(pfd, files.outfile, cmd->head, &infile_fd, &outfile_fd);
	else
		handle_middle_cmd(pfd, &infile_fd, &outfile_fd);
	dup2_and_close(infile_fd, STDIN_FILENO);
	dup2_and_close(outfile_fd, STDOUT_FILENO);
	if (cmd->abs_path == NULL)
	{
		if (cmd->cmd[0] == NULL)
			print_error(CMD_NOT_FOUND, "");
		else
			print_error(CMD_NOT_FOUND, cmd->cmd[0]);
		exit(EXIT_FAILURE);
	}
	if (execve(cmd->abs_path, cmd->cmd, envp) == -1)
	{
		free_cmds_list(cmd->head);
		exit(EXIT_FAILURE);
	}
}
