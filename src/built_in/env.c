#include "minishell.h"

void	print_all_env(t_envlist *envp)
{
	t_envlist *current_env;

	current_env = envp;
	while (current_env->next != NULL)
	{
		printf("%s\n", current_env->str);
		current_env = current_env->next;
	}
	printf("%s\n", current_env->str);
}

void	builtin_env(t_vars *vars)
{
	print_all_env(vars->envp);
}
