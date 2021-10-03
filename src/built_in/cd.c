#include "minishell.h"

// handle env variable (rewrite
// cd ~
// cd $___
// cd ~ 1234 --> no error, go to ~
// 

char	*search_home(t_envlist	*envp)
{
	t_envlist	*current_envp;
	char		*home_path;

	home_path = NULL;
	current_envp = envp;
	while (current_envp && ft_strncmp(current_envp->str, "HOME=", 5) == 0)
		current_envp = current_envp->next;
	if (ft_strncmp(current_envp->str, "HOME=", 5) == 1)
		home_path = ft_strdup(&current_envp->str[4]);
	return (home_path);
}

void	replace_pwd(t_vars *vars, t_envlist *current_envp, char *name, char *pwd)
{
	while (current_envp && ft_strncmp(current_envp->str, name, (int)ft_strlen(name)) != 0)
		current_envp = current_envp->next;
	if (current_envp && ft_strncmp(current_envp->str, name, (int)ft_strlen(name)) == 0)
	{
		free(current_envp->str);
		current_envp->str = ft_strjoin(name, pwd);
	}
	else
	{
		current_envp = vars->envp;
		while (current_envp->next)
			current_envp = current_envp->next;
		current_envp->next = protected_malloc(1, sizeof(t_envlist));
		current_envp = current_envp->next;
		current_envp->next = NULL;
		current_envp->str = ft_strjoin(name, pwd);
	}
}

void	replace_pwds(t_vars *vars, char *old_pwd)
{
	t_envlist	*current_envp;
	char		pwd[MAXPATHLEN];

	current_envp = vars->envp;
	getcwd(pwd, MAXPATHLEN);
	replace_pwd(vars, current_envp, "OLDPWD=", old_pwd);
	replace_pwd(vars, current_envp, "PWD=", pwd);
}

void	builtin_cd(t_vars *vars, t_token *current_token)
{
	char	*path;
	char	*path_temp;
	char	*home;
	char	old_pwd[MAXPATHLEN];

	current_token = current_token->next;
	path_temp = current_token->buffer.str;
	if (path_temp[0] == '~')
	{
		home = search_home(vars->envp);
		path = ft_strjoin(home, ft_substr(path_temp, 1, ft_strlen(path_temp) - 1));
		free(home);
		getcwd(old_pwd, MAXPATHLEN);
		chdir(path);
		replace_pwds(vars, old_pwd);
		free(path);
	}
	else
	{
		path = path_temp;
		getcwd(old_pwd, MAXPATHLEN);
		chdir(path);
		replace_pwds(vars, old_pwd);
	}
}