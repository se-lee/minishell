#include "minishell.h"


void	free_env_list(t_envlist *to_free)
{
	t_envlist	*current_env;
	t_envlist	*temp;

	current_env = to_free;
	while (current_env->next)
	{
		temp = current_env->next;
		free(current_env->str);
		free(current_env);
		current_env = temp;
	}
	if (current_env)
		free(current_env);
}

void	create_envlist(t_vars *vars, char **envp)
{
	int	i;
	t_envlist	*current_envlist;

	vars->envp = malloc(sizeof(t_envlist));
	current_envlist = vars->envp;
	current_envlist->str = ft_strdup(envp[0]);
	current_envlist->next = NULL;
	i = 1;
	while (envp[i])
	{
		current_envlist->next = malloc(sizeof(t_envlist));
		current_envlist = current_envlist->next;
		current_envlist->str = ft_strdup(envp[i]);
		current_envlist->next = NULL;
		i++;
	}
}

static int	format_is_valid(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (TRUE);
		i++;
	}
	return (FALSE);
}

static void	add_new_var_to_list(t_vars *vars, char *new_var)
{
	t_envlist *current_env;

	current_env = vars->envp;
	while (current_env->next != NULL)
		current_env = current_env->next;
	current_env->next = malloc(sizeof(t_envlist));
	current_env = current_env->next;
	current_env->str = ft_strdup(new_var);
	current_env->next = NULL;
}

static void	rewrite_value(t_vars *vars);

t_envlist	*dup_env(t_envlist *envp)
{
	t_envlist	*new_env;
	t_envlist	*current_env;
	char		*temp;
	t_envlist	*first;
	

	current_env = envp;
	new_env = protected_malloc(1, sizeof(t_envlist));
	temp = ft_strdup(current_env->str);
	new_env->str = ft_strjoin("declare -x ", temp);
	free(temp);
	new_env->next = NULL;
	first = new_env;
	current_env = current_env->next;
	while (current_env)
	{
		new_env->next = protected_malloc(1, sizeof(t_envlist));
		new_env = new_env->next;
		temp = ft_strdup(current_env->str);
		new_env->str = ft_strjoin("declare -x ", temp);
		free(temp);
		new_env->next = NULL;
		current_env = current_env->next;
	}
	return (first);
}

t_envlist	*sort_env(t_vars *vars)
{
	t_envlist	*current_env;
	t_envlist	*new_env;
	char		*temp;

	new_env = dup_env(vars->envp);
	current_env = new_env;
	while (current_env->next)
	{
		if (ft_strcmp(current_env->str, current_env->next->str) > 0)
		{
			temp = current_env->str;
			current_env->str = current_env->next->str;
			current_env->next->str = temp;
			current_env = new_env;
		}
		current_env = current_env->next;
	}
	return(new_env);
}

void	builtin_export(t_vars *vars, t_token *current_token)
{
	char		*var_str;
	char		*var_name;
	char		*temp;
	t_envlist	*sorted;

	current_token = current_token->next;
	if (current_token && current_token->token_type != PIPE_SIGN
		&& current_token->token_type != REDIRECT)
	{
		var_str = current_token->buffer.str;
		add_new_var_to_list(vars, var_str);
	}
	else
	{
		sorted = sort_env(vars);
		print_all_env(sorted);
		free_env_list(sorted);
	}
// if the given does exists,
// rewrite_value();
}
