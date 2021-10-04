#include "minishell.h"

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
	current_env->next = protected_malloc(1, sizeof(t_envlist));
	current_env = current_env->next;
	current_env->name = env_separation(new_var, 0);
	current_env->value = env_separation(new_var, 1);
	current_env->next = NULL;
}

void	builtin_export(t_vars *vars, t_token *current_token)
{
	char		*var_str;
	char		*var_name;
	char		*temp;
	t_envlist	*sorted;

	current_token = current_token->next;
	if (current_token && ft_piperedirect(current_token->token_type) == 0)
	{
		var_str = current_token->buffer.str;
		add_new_var_to_list(vars, var_str);
	}
	else
	{
		sorted = envlist_sort_ascii(vars);
		envlist_print_all(sorted);
		envlist_free(sorted);
	}
}
