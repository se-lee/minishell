#include "minishell.h"

// unset should delete both from envlist and real env

void	envlist_delete_first(t_vars *vars, t_envlist *current_env)
{
	t_envlist *temp;

	if(current_env->next != NULL)
	{
		temp = current_env->next;
		free(current_env->str);
		free(current_env);
		vars->envp = temp;
	}
	else
	{
		free(current_env->str);
		free(current_env);
		vars->envp = NULL;
	}
}
	//	A B C (remove B)
void	envlist_delete_var(t_vars *vars, t_envlist  *current_env)
{
	t_envlist *temp;

	if (current_env->next->next == NULL)
	{
		free(current_env->next->str);
		free(current_env->next);
		current_env->next = NULL;
	}
	else
	{
		temp = current_env->next->next;
		free(current_env->next->str);
		free(current_env->next);
		current_env->next = temp;
	}
}

void	builtin_unset(t_vars *vars, t_token *current_token)
{
	char	*var_to_unset;
	char	*var_str;
	t_envlist *current_env;

	current_token = current_token->next;
	while (current_token && ft_piperedirect(current_token->token_type) == 1)
		current_token = current_token->next;
	current_env = vars->envp;
	var_to_unset = current_token->buffer.str;
	printf("var_to_unset: %s\n", var_to_unset);
	if (ft_strncmp(current_env->str, var_to_unset, ft_strlen(var_to_unset)) == 0)
		envlist_delete_first(vars, current_env);
	else
	{
		while (current_env)
		{
			if (ft_strncmp(current_env->next->str, var_to_unset, ft_strlen(var_to_unset)) == 0)
				envlist_delete_var(vars, current_env);
			current_env = current_env->next; 
		}
	}	
}
