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

# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <sys/wait.h>
#include <fcntl.h>
# include "../../libft/libft.h"

typedef struct s_cmd
{
	char			**cmd;
	char			*abs_path;
	struct s_cmd	*next;
}	t_cmd;



char	*get_abs_path(char *command, char *envp[]);
void	split_cmds(int argc, char *argv[], char *envp[], t_cmd *cmd_list);
t_cmd	*get_last_cmd(t_cmd *cmd_list);
size_t	cmd_list_len(t_cmd *cmd_list);
void	init_cmd_list(t_cmd *cmd_list);

#endif