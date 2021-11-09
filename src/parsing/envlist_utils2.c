#include "minishell.h"

void	envlist_free(t_envlist *to_free)
{
	t_envlist	*current_env;
	t_envlist	*temp;

	current_env = to_free;
	while (current_env->next)
	{
		temp = current_env->next;
		free(current_env->name);
		if (current_env->value)
			free(current_env->value);
		free(current_env);
		current_env = temp;
	}
	if (current_env)
		free(current_env);
}

void	envlist_print_all(t_envlist *envp)
{
	t_envlist	*current_env;

	current_env = envp;
	while (current_env != NULL)
	{
		if (current_env->value)
		{
			printf("%s=", current_env->name);
			if (ft_strncmp(current_env->value, "", 2) == 0)
				printf("\"\"\n");
			else
				printf("\"%s\"\n", current_env->value);
		}
		else
			printf("%s\n", current_env->name);
		current_env = current_env->next;
	}
	if (current_env && current_env->value)
	{
		printf("%s=", current_env->name);
		printf("%s\n", current_env->value);
	}
}
