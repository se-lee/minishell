#include "minishell.h"

void	envlist_print(t_envlist *envp)
{
	t_envlist	*current_env;

	current_env = envp;
	while (current_env->next != NULL)
	{
		if (current_env->value)
		{
			printf("%s=", current_env->name);
			printf("%s\n", current_env->value);
		}
		current_env = current_env->next;
	}
	if (current_env->value)
	{
		printf("%s=", current_env->name);
		printf("%s\n", current_env->value);
	}
}

void	builtin_env(t_vars *vars)
{
	envlist_print(vars->envp);
}
