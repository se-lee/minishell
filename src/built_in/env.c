#include "minishell.h"

void	envlist_print(t_envlist *envp)
{
	t_envlist	*current_env;

	current_env = envp;
	while (current_env != NULL)
	{
		if (current_env->value)
		{
			printf("%s=", current_env->name);
			printf("%s\n", current_env->value);
		}
		current_env = current_env->next;
	}
	if (current_env && current_env->value)
	{
		printf("%s=", current_env->name);
		printf("%s\n", current_env->value);
	}
}

int	builtin_env(t_vars *vars)
{
	if (vars->cmd->command[1] != NULL)
	{
		display_cmd_error(&vars->cmd[0], "No such file or directory", TRUE);
		return (127);
	}
	else
		envlist_print(vars->envp);
	return (EXIT_SUCCESS);
}
