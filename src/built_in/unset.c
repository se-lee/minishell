#include "minishell.h"

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

void	envlist_delete_var(t_vars *vars, t_envlist  *current_env)
{
	t_envlist *temp;

	//	A B C (remove B)
	
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

	while (current_token && (current_token->token_type != WORD
		&& current_token->token_type != QUOTE
		&& current_token->token_type != SINGLE_QUOTE))
		current_token = current_token->next;
	var_to_unset = current_token->buffer.str;
	current_env = vars->envp;
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

/*
void	builtin_unset(t_vars *vars, t_token *current_token)
{
	char *var_str;
	char *temp;
	char *var_to_unset;
	int	i;
	
// printf("var_to_unset: %s\n", var_to_unset);
	if (getenv(var_to_unset) == NULL)
		return ;
	temp = ft_strjoin(var_to_unset, "=");
	var_str = ft_strjoin(temp, getenv(var_to_unset));
	free(temp);
	i = 0;
	while (vars->envp->next != NULL)
	{
		if ((ft_strcmp(vars->envp->str, var_str) == 0))
			delete_env_var(vars->envp, i);
		vars->envp = vars->envp->next;
	}
}
*/