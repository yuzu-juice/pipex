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

# define IN		0
# define OUT	1

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "../../libft/libft.h"

typedef struct s_cmd
{
	char			**cmd;
	char			*abs_path;
	struct s_cmd	*next;
}	t_cmd;

char	*get_abs_path(char *command, char *envp[]);
void	split_cmds(int argc, char *argv[], char *envp[], t_cmd *cmds_list);
t_cmd	*get_last_cmd(t_cmd *cmds_list);
size_t	cmds_list_len(t_cmd *cmds_list);
void	init_cmds_list(t_cmd *cmds_list);

#endif