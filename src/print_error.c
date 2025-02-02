#include "../include/pipex.h"

void	print_error(int errno, char *filename)
{
	write(2, "bash: ", 6);
	if (errno == -1)
		perror(filename);
	else if (errno == -2)
	{
		write(2, filename, ft_strlen(filename));
		write(2, ": command not found\n", 20);
	}
	else
	{
		write(2, strerror(errno), ft_strlen(strerror(errno)));
		write(2, "\n", 1);
	}
}
