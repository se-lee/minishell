#include "minishell.h"

void	envlist_create(t_vars *vars, char **envp)
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
	current_envlist = vars->envp;
	while (current_envlist && ft_strncmp(current_envlist->str, "OLDPWD=", 7) != 0)
		current_envlist = current_envlist->next;
	if (current_envlist && ft_strncmp(current_envlist->str, "OLDPWD=", 7) == 0)
	{
		free(current_envlist->str);
		current_envlist->str = ft_strjoin_char("OLDPWD=", ' ');
	}
}

void	envlist_free(t_envlist *to_free)
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

void	envlist_print_all(t_envlist *envp)
{
	t_envlist *current_env;

	current_env = envp;
	while (current_env->next != NULL)
	{
		printf("%s\n", current_env->str);
		current_env = current_env->next;
	}
	printf("%s\n", current_env->str);
}

t_envlist	*envlist_duplicate(t_envlist *envp)
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

t_envlist	*envlist_sort_ascii(t_vars *vars)
{
	t_envlist	*current_env;
	t_envlist	*new_env;
	char		*temp;

	new_env = envlist_duplicate(vars->envp);
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