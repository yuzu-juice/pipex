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
	char	***splited_args;
	char	*infile_name;
	int		infile_fd;
	int		outfile_fd;
	char	*outfile_name;
	pid_t	pid;
	int		fd[2];
	size_t	i;

	pipe(fd);
	infile_name = argv[1];
	outfile_name = argv[argc - 1];

	// splited_argsの配列を確保
	splited_args = malloc(sizeof(char **) * (argc - 1));
	if (!splited_args)
		return (1);

	// splited_argsに格納
	i = 2;
	while ((int)i < argc - 1)
	{
		splited_args[i - 2] = ft_split(argv[i], ' ');
		splited_args[i - 2][0] = get_absolute_path(splited_args[i - 2][0], envp);
		i++;
	}
	splited_args[i] = NULL;

	// 一つ目のコマンド実行
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		infile_fd = open(infile_name, O_RDONLY);
		dup2(infile_fd, STDIN_FILENO);
		close(infile_fd);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execve(splited_args[0][0], splited_args[0], envp);
	}

	pid = fork();
	if (pid == 0)
	{
		close(fd[1]);
		outfile_fd = open(outfile_name, O_WRONLY);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		dup2(outfile_fd, STDOUT_FILENO);
		close(outfile_fd);
		execve(splited_args[1][0], splited_args[1], envp);
	}

	wait(NULL);
	ft_printf("te");
	return (0);
}
