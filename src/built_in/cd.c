/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 07:43:14 by acusanno          #+#    #+#             */
/*   Updated: 2021/11/26 07:43:15 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_oldpwd_and_pwd(t_vars *vars, char *old_pwd)
{
	t_envlist	*current_envp;
	char		pwd[MAXPATHLEN];

	current_envp = vars->envp;
	getcwd(pwd, MAXPATHLEN);
	replace_pwd(vars, current_envp, "OLDPWD", old_pwd);
	replace_pwd(vars, current_envp, "PWD", pwd);
}

void	cd_to_home(t_vars *vars, char *path_temp)
{
	char	*home;
	char	*path;
	char	*temp;

	home = search_home(vars->envp);
	if (home == NULL)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putendl_fd("HOME not set", 2);
		return ;
	}
	path = NULL;
	temp = ft_substr(path_temp, 1, ft_strlen(path_temp) - 1);
	if (path_temp == NULL)
		path = ft_strdup(home);
	else if (ft_strncmp(path_temp, "~", 1) == 0)
		path = ft_strjoin(home, temp);
	free(home);
	free(temp);
	chdir(path);
	free(path);
	replace_oldpwd_and_pwd(vars, find_pwd(vars));
}

int	cd_to_oldpwd(t_vars *vars, t_command *current_cmd, char *old_pwd)
{
	if (old_pwd != NULL)
		printf("%s\n", old_pwd);
	else
	{
		display_cmd_error(current_cmd, "OLDPWD not set", FALSE);
		return (EXIT_FAILURE);
	}
	chdir(old_pwd);
	old_pwd = find_pwd(vars);
	replace_oldpwd_and_pwd(vars, old_pwd);
	return (EXIT_SUCCESS);
}

int	builtin_cd(t_vars *vars, t_command *current_cmd)
{
	char	*path;
	char	*old_pwd;

	old_pwd = find_old_pwd(vars);
	path = current_cmd->command[1];
	if (path == NULL || ft_strncmp(path, "~", 1) == 0)
		cd_to_home(vars, path);
	else if (ft_strncmp(path, "-", 2) == 0)
		cd_to_oldpwd(vars, current_cmd, old_pwd);
	else
	{
		old_pwd = find_pwd(vars);
		if (chdir(path) == -1)
		{
			display_cmd_error(current_cmd, "No such file or directory", TRUE);
			return (EXIT_FAILURE);
		}
		replace_oldpwd_and_pwd(vars, old_pwd);
	}
	return (EXIT_SUCCESS);
}
