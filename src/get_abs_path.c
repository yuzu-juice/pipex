/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_absolute_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 18:55:23 by takitaga          #+#    #+#             */
/*   Updated: 2025/01/09 21:31:26 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	*get_abs_path(char *command, char *envp[])
{
	size_t	i;
	size_t	path_len;
	char	**paths;
	char	*absolute_path;

	if (!command || !envp)
		return (NULL);
	if (access(command, X_OK) == 0)
		return (ft_strdup(command));
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp("PATH=", envp[i], 5) == 0)
			break ;
		i++;
	}
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + ft_strlen("PATH="), ':');
	i = 0;
	while (paths[i])
	{
		path_len = ft_strlen(paths[i]) + ft_strlen(command) + 2;
		absolute_path = malloc(sizeof(char) * path_len);
		ft_strlcpy(absolute_path, paths[i], path_len);
		ft_strlcat(absolute_path, "/", path_len);
		ft_strlcat(absolute_path, command, path_len);
		if (access(absolute_path, X_OK) == 0)
			return (absolute_path);
		i++;
	}
	return (NULL);
}
