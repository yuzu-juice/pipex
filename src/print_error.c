/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 20:39:42 by takitaga          #+#    #+#             */
/*   Updated: 2025/02/03 20:40:04 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static void	print_error_1(char *name)
{
	char	*err_msg;
	unsigned long name_len;
	unsigned long err_len;
	unsigned long total_len;

	name_len = ft_strlen(name);
	err_len = ft_strlen(strerror(errno));
	err_msg = ft_calloc(sizeof(char), 10 + name_len + err_len);
	total_len = 9 + name_len + err_len;
	if (!err_msg)
		return ;
	ft_strncpy(err_msg, "bash: ", 6);
	ft_strncpy(err_msg + 6, name, name_len);
	ft_strncpy(err_msg + 6 + name_len, ": ", 2);
	ft_strncpy(err_msg + 8 + name_len, strerror(errno), err_len);
	ft_strlcpy(err_msg + total_len - 1, "\n", sizeof("\n"));
	write(2, err_msg, total_len);
	free(err_msg);
}

static void	print_error_2(char *name)
{
	char	*err_msg;
	unsigned long name_len;
	unsigned long total_len;

	name_len = ft_strlen(name);
	err_msg = ft_calloc(sizeof(char), 27 + name_len);
	total_len = 26 + name_len;
	if (!err_msg)
		return ;
	ft_strncpy(err_msg, "bash: ", 6);
	ft_strncpy(err_msg + 6, name, name_len);
	ft_strncpy(err_msg + 6 + name_len, ": command not found\n", 20);
	ft_strlcpy(err_msg + total_len - 1, "\n", sizeof("\n"));
	write(2, err_msg, total_len);
	free(err_msg);
}

void	print_error(int err, char *name)
{
	if (err == ERROR)
		print_error_1(name);
	else if (err == CMD_NOT_FOUND)
		print_error_2(name);
}
