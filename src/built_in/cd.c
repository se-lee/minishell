#include "minishell.h"

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

void	cd_to_home(t_vars *vars, char *path_temp)
{
	char	*home;
	char	*path;

	home = search_home(vars->envp);
	printf("home:%s\n", home);
	if (home == NULL)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putendl_fd(": HOME not set", 2);
		return ;
	}
	path = NULL;
	if (path_temp == NULL)
		path = ft_strdup(home);
	else if (ft_strncmp(path_temp, "~", 1) == 0)
		path = ft_strjoin(home,
				ft_substr(path_temp, 1, ft_strlen(path_temp) - 1));
	free(home);
	chdir(path);
	free(path);
	replace_pwds(vars, find_pwd(vars));
}

void	builtin_cd(t_vars *vars, t_command *current_cmd)
{
	char	*path;
	char	*old_pwd;

	path = current_cmd->command[1];
	if (path == NULL || ft_strncmp(path, "~", 1) == 0)
		cd_to_home(vars, path);
	else if (ft_strncmp(path, "-", 2) == 0)
	{
		old_pwd = find_old_pwd(vars);
		if (old_pwd != NULL)
			printf("%s\n", old_pwd);
		else
			printf("OLDPWD not set\n");
		chdir(old_pwd);
		old_pwd = find_pwd(vars);
		replace_pwds(vars, old_pwd);
	}
	else
	{
		old_pwd = find_pwd(vars);
		if (chdir(path) == -1)
			perror("cd");
		replace_pwds(vars, old_pwd);
	}
}
