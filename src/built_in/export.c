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
	current_env->next = malloc(sizeof(t_envlist));
	current_env = current_env->next;
	current_env->str = ft_strdup(new_var);
	current_env->next = NULL;
}

static char	*get_var_name(char *var_str)
{
	int		i;
	char	*var_name;

	i = 0;
	while (var_str[i])
	{
		if (var_str[i] == '=')
			break ;
		i++;
	}
printf("i: %d\n", i);
	var_name = ft_substr(var_str, 0, i);
printf("var_name: %s\n", var_name);
	return (var_name);
}

// unset var then re-add the variable

static void	rewrite_value(t_vars *vars, t_token *current_token, char *var_name)
{
	t_envlist	*current_env;
	int			var_name_len;

	var_name_len = ft_strlen(var_name);
	current_env = vars->envp;
	while (current_env)
	{
		if (ft_strncmp(current_env->str, var_name, var_name_len) == 0)
		{
			free(current_env->str);
			current_env->str = current_token->buffer.str;
		}
		current_env = current_env->next;
	}
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
		var_name = get_var_name(var_str);
printf("var_str: %s\n", var_str);
		if (getenv(var_name) == NULL)
			add_new_var_to_list(vars, var_str);
		else if (getenv(var_name) != NULL)
			rewrite_value(vars, current_token, var_name);
	}
	else
	{
		sorted = envlist_sort_ascii(vars);
		envlist_print_all(sorted);
		envlist_free(sorted);
	}
}
