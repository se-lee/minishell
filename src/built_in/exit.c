/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 07:43:01 by acusanno          #+#    #+#             */
/*   Updated: 2021/11/26 07:43:04 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long long int	ft_atolli(const char *str)
{
	int				i;
	long long int	out;
	int				sign;
	int				len;

	len = 0;
	i = 0;
	sign = 1;
	out = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
		sign = -sign;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		out = out * 10 + (str[i++] - 48);
		len++;
	}
	if (len > 19 && sign == 1)
		return (-1);
	else if (len > 19 && sign == -1)
		return (0);
	return (sign * out);
}

void	display_error_and_exit(t_command *current_cmd,
	char *message, int exit_status)
{
	display_cmd_error(current_cmd, message, TRUE);
	exit(exit_status);
}

int	exit_uint_return_value(int return_value)
{
	if (return_value > 255 || return_value < 0)
		return_value = return_value % 256;
	return (return_value);
}

void	exit_with_arg(t_vars *vars, t_command *current_cmd)
{
	if (strisnum(current_cmd->command[1]) == 1
		&& current_cmd->command[2] == NULL)
	{
		vars->return_value = ft_atolli(current_cmd->command[1]);
		if (value_exceeds_llint(current_cmd->command[1],
				vars->return_value) == TRUE)
			display_error_and_exit(current_cmd,
				"numeric argument required", 255);
		vars->return_value = exit_uint_return_value(vars->return_value);
		if (count_command(vars->cmd) == 1)
			exit(exit_simple(vars->return_value));
	}
	else if (strisnum(current_cmd->command[1]) == 1
		&& current_cmd->command[2] != NULL)
	{
		ft_putstr_fd("exit\n", 1);
		display_cmd_error(current_cmd, "too many arguments", FALSE);
		vars->return_value = 1;
	}
	else
	{
		ft_putstr_fd("exit\n", 1);
		display_error_and_exit(current_cmd, "numeric argument required", 255);
	}
}

void	builtin_exit(t_vars *vars, t_command *current_cmd)
{
	if (current_cmd->command[1] == NULL && count_command(vars->cmd) == 1)
		exit(exit_simple(vars->return_value));
	else if (current_cmd->command[1] == NULL && count_command(vars->cmd) > 1)
		return ;
	else if (current_cmd->command[1])
	{
		if (current_cmd->command[1][0] == ' ')
			remove_space(&current_cmd->command[1]);
		exit_with_arg(vars, current_cmd);
	}
}
