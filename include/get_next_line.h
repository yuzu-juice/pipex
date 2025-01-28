/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 16:38:09 by takitaga          #+#    #+#             */
/*   Updated: 2024/05/26 09:18:42 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1000
# endif

# include <stddef.h>
# include <unistd.h>
# include <stdlib.h>

char		*get_next_line(int fd);
ssize_t		ft_getchar(int fd);
char		*ft_str_c_join(char *str, char c);

#endif
