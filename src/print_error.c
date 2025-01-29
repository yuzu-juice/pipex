#include "../include/pipex.h"

void	print_error(int errno)
{
	strerror(errno);
	exit(1);
}
