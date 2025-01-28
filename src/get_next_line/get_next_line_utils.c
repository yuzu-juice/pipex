/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 16:38:04 by takitaga          #+#    #+#             */
/*   Updated: 2024/05/26 09:18:18 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/get_next_line.h"

static size_t	ft_strlen(char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (*s)
	{
		len++;
		s++;
	}
	return (len);
}

static void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*d;
	const char	*s;
	size_t		i;

	d = dst;
	s = src;
	i = 0;
	if (n == 0 || (d == NULL && s == NULL))
		return (dst);
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

ssize_t	ft_getchar(int fd)
{
	static char		buf[BUFFER_SIZE];
	static ssize_t	n = 0;
	static size_t	i = 0;

	if (n == 0)
	{
		n = read(fd, buf, BUFFER_SIZE);
		if (n == -1)
		{
			n = 0;
			i = 0;
			return (-1);
		}
		if (n == 0)
			return (0);
		i = 0;
	}
	n--;
	return (buf[i++]);
}

char	*ft_str_c_join(char *str, char c)
{
	size_t	len;
	char	*result;

	if (ft_strlen(str) <= 0)
	{
		result = malloc(2);
		if (!result)
			return (NULL);
		result[0] = c;
		result[1] = '\0';
		return (result);
	}
	len = ft_strlen(str);
	result = malloc(len + 2);
	if (!result)
	{
		free(str);
		return (NULL);
	}
	ft_memcpy(result, str, len);
	free(str);
	result[len] = c;
	result[len + 1] = '\0';
	return (result);
}
