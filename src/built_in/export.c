#include "minishell.h"

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
static void	sort_env(t_vars *vars);

void	builtin_export(t_vars *vars, t_token *current_token)
{
	char		*var_str;
	char		*var_name;
	char		*temp;

	while (current_token)
	{
		if (current_token->token_type == WORD)
		{
			var_str = current_token->buffer.str;
			break ;
		}
		current_token = current_token->next;
	}
printf("var_str: %s\n", var_str);

// if the given variable does not exist
	add_new_var_to_list(vars, var_str);

// if the given does exists,
// rewrite_value();
}
