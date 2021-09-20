#include "minishell.h"

int main(int argc, char **argv)
{
	t_vars	vars;

	(void)argc;
	if (argc > 1)
		parsing(&vars, argv[0]);
	return (0);
}
