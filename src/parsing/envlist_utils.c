#include "minishell.h"

void	envlist_create_loop(char **envp, t_envlist **current_envlist, int i)
{
	(*current_envlist)->next = malloc(sizeof(t_envlist));
	(*current_envlist) = (*current_envlist)->next;
	(*current_envlist)->name = env_separation(envp[i], 0);
	(*current_envlist)->value = env_separation(envp[i], 1);
	(*current_envlist)->next = NULL;
}

void	envlist_create(t_vars *vars, char **envp)
{
	int			i;
	t_envlist	*current_envlist;

	vars->envp = malloc(sizeof(t_envlist));
	current_envlist = vars->envp;
	current_envlist->name = env_separation(envp[0], 0);
	current_envlist->value = env_separation(envp[0], 1);
	current_envlist->next = NULL;
	i = 1;
	while (envp[i])
	{
		envlist_create_loop(envp, &current_envlist, i);
		i++;
	}
	current_envlist = vars->envp;
	while (current_envlist && ft_strncmp(current_envlist->name,
			"OLDPWD", 7) != 0)
		current_envlist = current_envlist->next;
	if (current_envlist && ft_strncmp(current_envlist->name,
			"OLDPWD", 7) == 0)
	{
		free(current_envlist->value);
		current_envlist->value = NULL;
	}
}

void	env_dup_loop(t_envlist **current_env, t_envlist **new_env)
{
	char	*temp;

	(*new_env)->next = protected_malloc(1, sizeof(t_envlist));
	(*new_env) = (*new_env)->next;
	temp = ft_strdup((*current_env)->name);
	(*new_env)->name = ft_strjoin("declare -x ", temp);
	free(temp);
	if ((*current_env)->value != NULL)
		(*new_env)->value = ft_strdup((*current_env)->value);
	else
		(*new_env)->value = NULL;
	(*new_env)->next = NULL;
	(*current_env) = (*current_env)->next;
}

t_envlist	*envlist_duplicate(t_envlist *envp)
{
	t_envlist	*new_env;
	t_envlist	*current_env;
	char		*temp;
	t_envlist	*first;

	current_env = envp;
	new_env = protected_malloc(1, sizeof(t_envlist));
	temp = ft_strdup(current_env->name);
	new_env->name = ft_strjoin("declare -x ", temp);
	free(temp);
	if (current_env->value != NULL)
		new_env->value = ft_strdup(current_env->value);
	else
		new_env->value = NULL;
	new_env->next = NULL;
	first = new_env;
	current_env = current_env->next;
	while (current_env)
		env_dup_loop(&current_env, &new_env);
	return (first);
}
