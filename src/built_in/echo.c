/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 07:43:11 by acusanno          #+#    #+#             */
/*   Updated: 2021/11/26 07:43:12 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo_n(char *str)
{
	int	i;

	if (str[0] != '-')
		return (0);
	if (ft_strlen(str) == 1)
		return (0);
	i = 1;
	while (str[i] && str[i] == 'n')
		i++;
	if (str[i])
		return (0);
	return (1);
}

void	echo_print_args(t_command *current_cmd, int i)
{
	while (current_cmd->command[i])
	{
		printf("%s", current_cmd->command[i]);
		i++;
		if (current_cmd->command[i])
			printf(" ");
	}	
}

int	builtin_echo(t_command *current_cmd)
{
	int	n_option;
	int	i;

	n_option = 0;
	i = 1;
	if (current_cmd && current_cmd->command[1] != NULL)
	{
		if (current_cmd->command[1][0] == '-')
		{
			while (current_cmd->command[i]
				&& echo_n(current_cmd->command[i]) == 1)
			{
				n_option = 1;
				i++;
			}
		}
		if (n_option == 1 && current_cmd->command[i] == NULL)
			return (EXIT_SUCCESS);
		echo_print_args(current_cmd, i);
	}
	if (n_option == 0)
		printf("\n");
	return (EXIT_SUCCESS);
}
