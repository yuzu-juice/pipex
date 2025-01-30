#include "../include/pipex.h"

void	print_error(int errno, char *filename)
{
	dup2(STDERR_FILENO, STDOUT_FILENO);
	ft_printf("bash: ");
	if (errno == -1)
		perror(filename);
	else if (errno == -2)
		ft_printf("%s: command not found\n", filename);
	else
		ft_printf("%s\n", strerror(errno));
}
