/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 07:42:40 by acusanno          #+#    #+#             */
/*   Updated: 2021/11/26 07:42:42 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_command_builtin(t_vars *vars, t_command *current_cmd)
{
	char	*command;

	command = current_cmd->command[0];
	if (ft_strcmp(command, "cd") == 0)
		vars->return_value = builtin_cd(vars, current_cmd);
	else if (ft_strcmp(command, "echo") == 0)
		vars->return_value = builtin_echo(current_cmd);
	else if (ft_strcmp(command, "env") == 0)
		vars->return_value = builtin_env(vars);
	else if (ft_strcmp(command, "exit") == 0)
		builtin_exit(vars, current_cmd);
	else if (ft_strcmp(command, "export") == 0)
		vars->return_value = builtin_export(vars, current_cmd);
	else if (ft_strcmp(command, "pwd") == 0)
		vars->return_value = builtin_pwd();
	else if (ft_strcmp(command, "unset") == 0)
		vars->return_value = builtin_unset(vars, current_cmd, 0);
}

int	check_stat_and_access(t_vars *vars, t_command *current_cmd, char *path)
{
	struct stat	buff;

	if (path != NULL)
	{
		if (stat(path, &buff) < 0)
		{
			display_cmd_error(current_cmd, "No such file or directory", FALSE);
			vars->return_value = 127;
			return (FALSE);
		}
		else if (S_ISDIR(buff.st_mode))
		{
			display_cmd_error(current_cmd, "is a directory", FALSE);
			vars->return_value = 126;
			return (FALSE);
		}
		else if (access(path, X_OK) < 0)
		{
			display_cmd_error(current_cmd, "Permission denied", FALSE);
			vars->return_value = 126;
			return (FALSE);
		}
	}
	return (TRUE);
}

void	run_command_non_builtin(t_vars *vars,
	t_envlist *envlist, t_command *current_cmd)
{
	char		*path;
	char		**env;

	env = envlist_to_char_array(envlist);
	path = get_command_path(envlist, current_cmd->command[0], 0);
	if (check_stat_and_access(vars, current_cmd, path) == FALSE)
		exit (vars->return_value);
	if (path != NULL)
	{
		if (execve(path, current_cmd->command, env) < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(current_cmd->command[0]);
			vars->return_value = 126;
		}
	}
	else
	{
		display_cmd_error(current_cmd, "command not found", FALSE);
		vars->return_value = 127;
	}
	free(path);
	exit(vars->return_value);
}

void	run_command_and_exit(t_vars *vars, t_command *current_cmd)
{
	if (command_is_builtin(current_cmd->command) == TRUE)
	{
		run_command_builtin(vars, current_cmd);
		exit(vars->return_value);
	}
	else
	{
		run_command_non_builtin(vars, vars->envp, current_cmd);
		exit(vars->return_value);
	}
}

void	redirect_and_run_cmd(t_vars *vars, t_command *current_cmd, int builtin)
{
	if (builtin == TRUE)
	{
		redirection(vars, current_cmd);
		run_command_builtin(vars, current_cmd);
	}
	else
	{
		redirection(vars, current_cmd);
		run_command_non_builtin(vars, vars->envp, current_cmd);
	}
}
