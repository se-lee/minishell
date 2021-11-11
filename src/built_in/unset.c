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

void	envlist_delete_var(t_envlist *current_env)
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

/* ADDED 11/11 */
void	unset_check_error(char *var_to_unset)
{
	if (ft_isdigit(var_to_unset[0]))
	{
		ft_putstr_fd("minishell: unset: ", 2);
		ft_putstr_fd(var_to_unset, 2);
		ft_putendl_fd(": not a valid identifier", 2);
	}
}

void	builtin_unset(t_vars *vars, t_command *current_cmd, int i)
{
	char		*var_to_unset;
	t_envlist	*current_env;

	if (!(current_cmd))
		return ;
	while (current_cmd->command[i])
	{
		current_env = vars->envp;
		var_to_unset = current_cmd->command[i];
		unset_check_error(var_to_unset);
		if (ft_strncmp(current_env->name, var_to_unset,
				ft_strlen(var_to_unset)) == 0)
			envlist_delete_first(vars, current_env);
		else
		{
			while (current_env->next)
			{
				if (ft_strncmp(current_env->next->name, var_to_unset,
						ft_strlen(var_to_unset) + 1) == 0)
					envlist_delete_var(current_env);
				if (current_env->next)
					current_env = current_env->next;
			}
		}
		i++;
	}
}
