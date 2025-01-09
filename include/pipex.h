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

char	*get_absolute_path(char *command, char *envp[]);

#endif