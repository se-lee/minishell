#include "minishell.h"

void	envlist_delete_first(t_vars *vars, t_envlist *current_env)
{
	t_envlist	*temp;

	if (current_env->next != NULL)
	{
		temp = current_env->next;
		free(current_env->name);
		if (current_env->value)
			free(current_env->value);
		free(current_env);
		vars->envp = temp;
	}
	else
	{
		free(current_env->name);
		if (current_env->value)
			free(current_env->value);
		free(current_env);
		vars->envp = NULL;
	}
}

void	envlist_delete_var(t_vars *vars, t_envlist *current_env)
{
	t_envlist	*temp;

	if (current_env->next->next == NULL)
	{
		free(current_env->next->name);
		if (current_env->next->value)
			free(current_env->next->value);
		free(current_env->next);
		current_env->next = NULL;
	}
	else
	{
		temp = current_env->next->next;
		free(current_env->next->name);
		if (current_env->next->value)
			free(current_env->next->value);
		free(current_env->next);
		current_env->next = temp;
	}
}

void	builtin_unset(t_vars *vars, t_command *current_cmd)
{
	char		*var_to_unset;
	char		*var_str;
	t_envlist	*current_env;
	int			i;

	if (!(current_cmd))
		return ;
	var_to_unset = current_cmd->command[1];
	current_env = vars->envp;
	if (ft_strncmp(current_env->name, var_to_unset,
			ft_strlen(var_to_unset)) == 0)
		envlist_delete_first(vars, current_env);
	else
	{
		while (current_env->next)
		{
			if (ft_strncmp(current_env->next->name, var_to_unset,
					ft_strlen(var_to_unset) + 1) == 0)
				envlist_delete_var(vars, current_env);
			if (current_env->next)
				current_env = current_env->next;
		}
	}	
}
