#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_vars	vars;

	vars.envp = envp;
	(void)argc;
	if (argc > 1)
		parsing(&vars, argv[1]);
	return (0);
}
