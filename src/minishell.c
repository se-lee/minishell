#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
	t_vars	vars;

	vars.envp = envp;
	(void)argc;
	// if (argc > 1)
	// 	parsing(&vars, argv[1]);
	printf("%s\n", replace(argv[1], argv[2], argv[3]));
	return (0);
}
