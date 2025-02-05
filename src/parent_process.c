/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 00:36:23 by takitaga          #+#    #+#             */
/*   Updated: 2025/02/05 00:42:12 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	parent_process(int pfd[2][2], t_cmd *cmd)
{
	if (cmd->index != 0)
		close_pipe(pfd[PREV]);
	if (cmd->next)
	{
		pfd[PREV][READ] = pfd[CURR][READ];
		pfd[PREV][WRITE] = pfd[CURR][WRITE];
	}
	else
		close_pipe(pfd[CURR]);
}
