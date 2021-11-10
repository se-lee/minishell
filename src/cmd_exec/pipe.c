#include "minishell.h"

void	fd_dup_and_close(int input, int output)
{
	if (input != 0)
	{
		if (dup2(input, 0) < 0)
			perror("dup2");
		close(input);
	}
	if (output != 1)
	{
		if (dup2(output, 1) < 0)
			perror("dup2");
		close(output);
	}
}

void	fd_close(int input, int output)
{
	if (input != 0)
		close(input);
	if (output != 1)
		close(output);
}
