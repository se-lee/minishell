#include "../../include/minishell.h"

/* 
print all env variables
*/

void	print_all_env(t_vars *vars)
{
	int	i;

	i = 0;
	while (vars->envp[i] != NULL)
	{
		printf("%s\n", vars->envp[i]);
		i++;
	}
}

void	builtin_env(t_vars *vars)
{
	print_all_env(vars);
}
