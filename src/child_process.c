/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 20:39:42 by takitaga          #+#    #+#             */
/*   Updated: 2025/02/03 20:39:42 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	close_pipe(int pipe_fd[2])
{
	close(pipe_fd[READ]);
	close(pipe_fd[WRITE]);
}

static void	handle_first_cmd(int pipe_fd[2][2], char *infile, t_cmd *cmds_list, int *infile_fd, int *outfile_fd)
{
    *infile_fd = open(infile, O_RDONLY);
    if (*infile_fd < 0)
    {
        print_error(ERROR, infile);
        free_cmds_list(cmds_list);
        exit(EXIT_FAILURE);
    }
    close(pipe_fd[CURR][READ]);
    *outfile_fd = pipe_fd[CURR][WRITE];
}

static void	handle_last_cmd(int pipe_fd[2][2], char *outfile, t_cmd *cmds_list, int *infile_fd, int *outfile_fd)
{
    close(pipe_fd[PREV][WRITE]);
    *infile_fd = pipe_fd[PREV][READ];
    *outfile_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (*outfile_fd < 0)
    {
        print_error(ERROR, outfile);
        free_cmds_list(cmds_list);
        exit(EXIT_FAILURE);
    }
}

static void	handle_middle_cmd(int pipe_fd[2][2], int *infile_fd, int *outfile_fd)
{
    close(pipe_fd[PREV][WRITE]);
    close(pipe_fd[CURR][READ]);
    *infile_fd = pipe_fd[PREV][READ];
    *outfile_fd = pipe_fd[CURR][WRITE];
}

void	child_process(_Bool is_first_cmd, int pipe_fd[2][2], t_cmd *cmd, char *infile, char *outfile, char *envp[], t_cmd *cmds_list)
{
    int	infile_fd;
    int	outfile_fd;

    if (is_first_cmd)
        handle_first_cmd(pipe_fd, infile, cmds_list, &infile_fd, &outfile_fd);
    else if (cmd->next == NULL)
        handle_last_cmd(pipe_fd, outfile, cmds_list, &infile_fd, &outfile_fd);
    else
        handle_middle_cmd(pipe_fd, &infile_fd, &outfile_fd);
    dup2(infile_fd, STDIN_FILENO);
    close(infile_fd);
    dup2(outfile_fd, STDOUT_FILENO);
    close(outfile_fd);
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
        free_cmds_list(cmds_list);
        exit(EXIT_FAILURE);
    }
}
