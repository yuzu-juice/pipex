/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 16:37:58 by takitaga          #+#    #+#             */
/*   Updated: 2024/05/26 09:21:31 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/get_next_line.h"

char	*get_next_line(int fd)
{
	ssize_t	result;
	char	*str;
	int		flag;

	str = NULL;
	flag = 1;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while (flag)
	{
		result = ft_getchar(fd);
		if ((!str && result == 0) || result == -1)
		{
			free(str);
			return (NULL);
		}
		if (result == '\n')
			flag = 0;
		if (result == 0 && str)
			return (str);
		str = ft_str_c_join(str, result);
	}
	return (str);
}

// #include <fcntl.h>

// int main(void)
// {
//     int fd = open("testfile.txt", O_RDONLY);
//     if (fd == -1)
//     {
//         perror("Error opening file");
//         return (1);
//     }

//     char *line;
//     while ((line = get_next_line(fd)) != NULL)
//     {
//         printf("output: %s", line);
//         free(line);
//     }

//     close(fd);
//     return (0);
// }
