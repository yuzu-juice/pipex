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

static char	**get_paths_from_env(char *envp[])
{
	size_t	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
	{
		if (ft_strncmp("PATH=", envp[i], 5) == 0)
			break ;
		i++;
	}
	if (!envp[i])
		return (NULL);
	return (ft_split(envp[i] + ft_strlen("PATH="), ':'));
}

static char	*build_executable_path(char *path, char *command)
{
	size_t	path_len;
	char	*absolute_path;

	path_len = ft_strlen(path) + ft_strlen(command) + 2;
	absolute_path = ft_calloc(sizeof(char), path_len);
	if (!absolute_path)
		return (NULL);
	ft_strlcpy(absolute_path, path, path_len);
	ft_strlcat(absolute_path, "/", path_len);
	ft_strlcat(absolute_path, command, path_len);
	return (absolute_path);
}

char	*get_abs_path(char *command, char *envp[])
{
	char	**paths;
	char	*absolute_path;
	size_t	i;

	if (!command)
		return (NULL);
	if (access(command, X_OK) == 0)
		return (ft_strdup(command));
	paths = get_paths_from_env(envp);
	if (paths == NULL)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		absolute_path = build_executable_path(paths[i], command);
		if (!absolute_path)
		{
			free_string_array(paths);
			return (NULL);
		}
		if (access(absolute_path, X_OK) == 0)
		{
			free_string_array(paths);
			return (absolute_path);
		}
		free(absolute_path);
		i++;
	}
	free_string_array(paths);
	return (NULL);
}
