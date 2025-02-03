#include "../include/pipex.h"

void	close_pipe(int pipe_fd[2])
{
	close(pipe_fd[READ]);
	close(pipe_fd[WRITE]);
}

void	child_process(_Bool is_first_cmd, int pipe_fd[2][2], t_cmd *cmd, int argc, char *argv[], char *envp[], t_cmd *cmds_list)
{
	int	infile_fd;
	int	outfile_fd;

	if (is_first_cmd)
	{
		close(pipe_fd[CURR][READ]);
		infile_fd = open(argv[1], O_RDONLY);
		if (infile_fd < 0)
		{
			print_error(ERROR, argv[1]);
			free_cmds_list(cmds_list);
			exit(EXIT_FAILURE);
		}
		outfile_fd = pipe_fd[CURR][WRITE];
	}
	else if (cmd->next == NULL)
	{
		close(pipe_fd[PREV][WRITE]);
		infile_fd = pipe_fd[PREV][READ];
		outfile_fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (outfile_fd < 0)
		{
			print_error(ERROR, argv[argc - 1]);
			free_cmds_list(cmds_list);
			exit(EXIT_FAILURE);
		}
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
	if (cmd->abs_path == NULL)
	{
		if (cmd->cmd[0] == NULL)
			print_error(CMD_NOT_FOUND, "");
		else
			print_error(CMD_NOT_FOUND, cmd->cmd[0]);
		exit(EXIT_FAILURE) ;
	}
	if (execve(cmd->abs_path, cmd->cmd, envp) == -1)
	{
		free_cmds_list(cmds_list);
		exit (EXIT_FAILURE);
	}
}
