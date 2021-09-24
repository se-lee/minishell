#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_vars	vars;
	char	*str;

	vars.envp = envp;
	(void)argc;
	(void)argv;
	str = readline("minishell$ ");
	while (str != NULL)
	{
		if (str)
		{
			add_history(str);
			parsing(&vars, str);
			free(str);
		}
		str = readline("minishell$ ");
	}
	return (0);
}
