/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takitaga  <takitaga@student.42tokyo.>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:07:12 by takitaga          #+#    #+#             */
/*   Updated: 2025/01/08 14:19:25 by takitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

_Bool	validate_input(int argc, char *argv[])
{
	if (argc < 5)
		return (false);
	(void)argv;
	return (true);
}
