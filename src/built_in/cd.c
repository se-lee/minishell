#include "minishell.h"

char	*search_home(t_envlist	*envp)
{
	t_envlist	*current_envp;
	char		*home_path;

	home_path = NULL;
	current_envp = envp;
	while (current_envp && ft_strncmp(current_envp->name, "HOME", 5) != 0)
		current_envp = current_envp->next;
	if (ft_strncmp(current_envp->name, "HOME", 5) == 0)
		home_path = ft_strdup(current_envp->value);
	return (home_path);
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
	{
		current_envp = vars->envp;
		while (current_envp->next)
			current_envp = current_envp->next;
		current_envp->next = protected_malloc(1, sizeof(t_envlist));
		current_envp = current_envp->next;
		current_envp->next = NULL;
		current_envp->name = ft_strdup(name);
		if (pwd != NULL)
			current_envp->value = ft_strdup(pwd);
		else
			current_envp->value = NULL;
	}
}

void	replace_pwds(t_vars *vars, char *old_pwd)
{
	t_envlist	*current_envp;
	char		pwd[MAXPATHLEN];

	current_envp = vars->envp;
	getcwd(pwd, MAXPATHLEN);
	replace_pwd(vars, current_envp, "OLDPWD", old_pwd);
	replace_pwd(vars, current_envp, "PWD", pwd);
}

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

void	builtin_cd(t_vars *vars, t_command *current_cmd)
{
	char	*path;
	char	*path_temp;
	char	*home;
	char	*old_pwd;

	path_temp = current_cmd->command[1];
	if (ft_strncmp(path_temp, "~", 1) == 0)
	{
		home = search_home(vars->envp);
		path = ft_strjoin(home,
				ft_substr(path_temp, 1, ft_strlen(path_temp) - 1));
		free(home);
		chdir(path);
		free(path);
		old_pwd = find_pwd(vars);
		replace_pwds(vars, old_pwd);
	}
	else if (ft_strncmp(path_temp, "-", 2) == 0)
	{
		old_pwd = find_old_pwd(vars);
		printf("%s\n", old_pwd);
		chdir(old_pwd);
		old_pwd = find_pwd(vars);
		replace_pwds(vars, old_pwd);
	}
	else
	{
		path = path_temp;
		old_pwd = find_pwd(vars);
		if (chdir(path) == -1)
			perror("cd");
		replace_pwds(vars, old_pwd);
	}
}
