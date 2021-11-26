/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 07:42:58 by acusanno          #+#    #+#             */
/*   Updated: 2021/11/26 07:42:59 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	format_is_valid(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (TRUE);
		i++;
	}
	return (FALSE);
}

int	ft_inenv(t_envlist *envp, char *str)
{
	t_envlist	*current_env;
	char		*name;

	current_env = envp;
	name = env_separation(str, 0);
	while (current_env && (ft_strncmp(current_env->name,
				name, (ft_strlen(name) + 1)) != 0))
		current_env = current_env->next;
	if (current_env)
	{
		free(name);
		return (1);
	}
	free(name);
	return (0);
}

int	export_syntax(char *str, int quotes)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
	{
		if ((ft_isalnum(str[i]) != 1 && (str[i] != '_'))
			|| (str[i] >= '0' && str[i] <= '9' && i == 0))
			return (0);
		i++;
	}
	if (i == 0)
		return (0);
	while (str[i] && quotes == 0)
	{
		if (ft_isalnum(str[i]) != 1 && (str[i] != '-') && (str[i] != '_')
			&& (str[i] != ' ') && str[i] != '=' && str[i] != '.'
			&& str[i] != '/' && str[i] != '?' && str[i] != '\''
			&& str[i] != '"' && str[i] != ':')
			return (0);
		i++;
	}
	return (1);
}

void	add_new_var_to_list(t_vars *vars, char *new_var)
{
	t_envlist	*current_env;

	if (vars->envp == NULL)
	{
		vars->envp = protected_malloc(1, sizeof(t_envlist));
		current_env = vars->envp;
		current_env->name = env_separation(new_var, 0);
		current_env->value = env_separation(new_var, 1);
		current_env->next = NULL;
	}
	else
	{
		current_env = vars->envp;
		while (current_env->next != NULL)
			current_env = current_env->next;
		current_env->next = protected_malloc(1, sizeof(t_envlist));
		current_env = current_env->next;
		current_env->name = env_separation(new_var, 0);
		current_env->value = env_separation(new_var, 1);
		current_env->next = NULL;
	}
}

void	free_env(t_envlist *current_env)
{
	free(current_env->name);
	free(current_env->value);
	free(current_env);
}
