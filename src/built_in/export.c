#include "minishell.h"

t_envlist	*envlist_sort_ascii(t_vars *vars)
{
	t_envlist	*current_env;
	t_envlist	*new_env;
	char		*temp_name;
	char		*temp_value;

	new_env = envlist_duplicate(vars->envp);
	current_env = new_env;
	while (current_env->next)
	{
		if (ft_strcmp(current_env->name, current_env->next->name) > 0)
		{
			temp_name = current_env->name;
			temp_value = current_env->value;
			current_env->name = current_env->next->name;
			current_env->value = current_env->next->value;
			current_env->next->name = temp_name;
			current_env->next->value = temp_value;
			current_env = new_env;
		}
		current_env = current_env->next;
	}
	return (new_env);
}

void	delete_env_else(t_envlist **current_env, char *name)
{
	t_envlist	*temp;

	while ((*current_env)->next && (ft_strncmp((*current_env)->next->name,
				name, (ft_strlen(name) + 1)) != 0))
		(*current_env) = (*current_env)->next;
	if ((*current_env)->next)
	{
		temp = (*current_env)->next->next;
		free_env((*current_env)->next);
		(*current_env)->next = temp;
	}
}

void	delete_env(t_envlist *envp, char *str)
{
	t_envlist	*current_env;
	char		*name;

	current_env = envp;
	name = env_separation(str, 0);
	if (current_env && (ft_strncmp(current_env->next->name,
				name, ft_strlen(name) + 1) == 0))
	{
		if (current_env->next)
			envp = current_env->next;
		else
			envp = NULL;
		free_env(current_env);
	}
	else
		delete_env_else(&current_env, name);
}

void	export_while(t_vars *vars, char *command, int quotes)
{
	char	*var_str;
	int		res;

	res = export_syntax(command, quotes);
	// printf("res:%d\n", res);
	if (res == 0)
	{
		ft_putstr_fd("minishell: export: ", 2);
		ft_putstr_fd(command, 2);
		ft_putendl_fd(": not a valid identifier", 2);
		// printf("export: %s: invalid token\n", command);
	}
	else
	{
		var_str = command;
		if (ft_inenv(vars->envp, var_str) == 1)
			delete_env(vars->envp, var_str);
		add_new_var_to_list(vars, var_str);
	}
}

void	builtin_export(t_vars *vars, t_command *current_cmd)
{
	t_envlist	*sorted;
	int			i;

	// print_commands(current_cmd);
	if (current_cmd && current_cmd->command[1])
	{
		i = 1;
		while (current_cmd->command[i])
		{
			// printf("cmd[%d]:%s\n", i, current_cmd->command[i]);
			export_while(vars, current_cmd->command[i], current_cmd->quotes);
			i++;
		}
	}
	else if (vars->envp != NULL)
	{
		sorted = envlist_sort_ascii(vars);
		envlist_print_all(sorted);
		envlist_free(sorted);
	}
}
