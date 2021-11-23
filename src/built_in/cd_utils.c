#include "minishell.h"

char	*find_old_pwd(t_vars *vars)
{
	t_envlist	*current_env;

	current_env = vars->envp;
	while (current_env && ft_strncmp(current_env->name, "OLDPWD", 7) != 0)
		current_env = current_env->next;
	if (current_env)
		return (current_env->value);
	return (NULL);
}

char	*find_pwd(t_vars *vars)
{
	t_envlist	*current_env;

	current_env = vars->envp;
	while (current_env && ft_strncmp(current_env->name, "PWD", 4) != 0)
		current_env = current_env->next;
	if (current_env)
		return (current_env->value);
	return (NULL);
}

char	*search_home(t_envlist	*envp)
{
	t_envlist	*current_envp;
	char		*home_path;

	home_path = NULL;
	current_envp = envp;
	while (current_envp)
	{
		if (ft_strncmp(current_envp->name, "HOME", 5) == 0
			&& current_envp->value != NULL)
			home_path = ft_strdup(current_envp->value);
		current_envp = current_envp->next;
	}
	return (home_path);
}

void	replace_main(t_vars *vars, t_envlist *current_envp,
			char *name, char *pwd)
{
	current_envp = vars->envp;
	while (current_envp && current_envp->next)
		current_envp = current_envp->next;
	if (vars->envp == NULL)
	{
		vars->envp = protected_malloc(1, sizeof(t_envlist));
		current_envp = vars->envp;
	}
	else
	{
		current_envp->next = protected_malloc(1, sizeof(t_envlist));
		current_envp = current_envp->next;
	}
	current_envp->next = NULL;
	current_envp->name = ft_strdup(name);
	if (pwd != NULL)
		current_envp->value = ft_strdup(pwd);
	else
		current_envp->value = NULL;
}

void	replace_pwd(t_vars *vars, t_envlist *current_envp,
		char *name, char *pwd)
{
	while (current_envp && ft_strncmp(current_envp->name,
			name, (int)ft_strlen(name)) != 0)
		current_envp = current_envp->next;
	if (current_envp && ft_strncmp(current_envp->name,
			name, (int)ft_strlen(name)) == 0)
	{
		free(current_envp->value);
		if (pwd != NULL)
			current_envp->value = ft_strdup(pwd);
		else
			current_envp->value = NULL;
	}
	else
		replace_main(vars, current_envp, name, pwd);
}
