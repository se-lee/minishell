#include "minishell.h"

static void	add_new_var_to_list(t_vars *vars, char *new_var)
{
	t_envlist	*current_env;

	current_env = vars->envp;
	while (current_env->next != NULL)
		current_env = current_env->next;
	current_env->next = protected_malloc(1, sizeof(t_envlist));
	current_env = current_env->next;
	current_env->name = env_separation(new_var, 0);
	current_env->value = env_separation(new_var, 1);
	current_env->next = NULL;
}

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

void	free_env(t_envlist *current_env)
{
	free(current_env->name);
	free(current_env->value);
	free(current_env);
}

void	delete_env(t_envlist *envp, char *str)
{
	t_envlist	*current_env;
	t_envlist	*temp;
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
	{
		while (current_env->next && (ft_strncmp(current_env->next->name,
					name, (ft_strlen(name) + 1)) != 0))
			current_env = current_env->next;
		if (current_env->next)
		{
			temp = current_env->next->next;
			free_env(current_env->next);
			current_env->next = temp;
		}
	}
}

void	export_while(t_vars *vars, char *command, int quotes)
{
	char	*var_str;
	int		res;

	res = export_syntax(command, quotes);
	printf("res:%d\n", res);
	if (res == 0)
		printf("export: %s: invalid token\n", command);
	else
	{
		var_str = command;
		if (ft_inenv(vars->envp, var_str) == 1)
			delete_env(vars->envp, var_str);
		add_new_var_to_list(vars, var_str);
	}
}

/*
// this is original
void	builtin_export(t_vars *vars, t_command *current_cmd)
{
	char		*var_str;
	char		*var_name;
	char		*temp;
	t_envlist	*sorted;
	int			i;

	if (current_cmd && current_cmd->command[1])
	{
		i = 1;
		while (current_cmd->command[i])
		{
	printf("cmd[%d]:%s\n", i, current_cmd->command[i]);
			export_while(vars, current_cmd->command[i]);
			i++;
		}
	}
	else
	{
		sorted = envlist_sort_ascii(vars);
		envlist_print_all(sorted);
		envlist_free(sorted);
	}
}
*/

// this is test version 
void	builtin_export(t_vars *vars, t_command *current_cmd)
{
	char		*var_str;
	char		*var_name;
	char		*temp;
	t_envlist	*sorted;
	int			i;

print_commands(current_cmd);

	if (current_cmd && current_cmd->command[1])
	{
		i = 1;
		while (current_cmd->command[i])
		{
printf("cmd[%d]:%s\n", i, current_cmd->command[i]);
			export_while(vars, current_cmd->command[i], current_cmd->quotes);
			i++;
		}
	}
	else
	{
		sorted = envlist_sort_ascii(vars);
		envlist_print_all(sorted);
		envlist_free(sorted);
	}
}
