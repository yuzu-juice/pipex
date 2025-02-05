/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 13:54:08 by takitaga          #+#    #+#             */
/*   Updated: 2025/01/08 14:08:18 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define READ			0
# define WRITE			1
# define PREV			0
# define CURR			1
# define HERE_DOC_FILE	"/tmp/.heredoc_tmp42"
# define ERROR			0
# define CMD_NOT_FOUND	1

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include "../../libft/libft.h"
# include "./get_next_line.h"

typedef struct s_cmd
{
	int				index;
	char			**cmd;
	char			*abs_path;
	struct s_cmd	*next;
	struct s_cmd 	*head;
} t_cmd;

char	*get_abs_path(char *command, char *envp[]);
_Bool	init_cmds_list(t_cmd *cmds_list, int argc, char *argv[], char *envp[]);
_Bool	here_doc(char *limiter);
void	print_error(int err, char *name);
void	free_cmds_list(t_cmd *cmds_list);
void	free_string_array(char **str_arr);
void 	close_pipe(int pipe_fd[2]);
void	child_process(int pipe_fd[2][2], t_cmd *cmd, char *infile, char *outfile, char *envp[]);
void	parent_process(int pipe_fd[][2], t_cmd *cmd);

#endif
