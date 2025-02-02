#include "../include/pipex.h"

void	print_error(int err, char *name)
{
	if (err == ERROR)
		ft_fprintf(2, "bash: %s: %s\n", name, strerror(errno));
	else if (err == CMD_NOT_FOUND)
		ft_fprintf(2, "bash: %s: command not found\n", name);
}
