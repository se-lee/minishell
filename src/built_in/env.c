#include "../../include/minishell.h"

void	print_all_env(t_vars *vars)
{
	t_envlist *current_env;

	current_env = vars->envp;
	while (current_env->next != NULL)
	{
		printf("%s\n", current_env->str);
		current_env = current_env->next;
	}
	printf("%s\n", vars->envp->str);
}

void	builtin_env(t_vars *vars)
{
	print_all_env(vars);
}
