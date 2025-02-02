#include "../include/pipex.h"

void    free_string_array(char **str_arr)
{

	size_t	i;

	i = 0;
	while (str_arr[i])
	{
		free(str_arr[i]);
		i++;
	}
	free(str_arr);
}
