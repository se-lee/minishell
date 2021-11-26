/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envlist_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 07:42:14 by acusanno          #+#    #+#             */
/*   Updated: 2021/11/26 07:44:15 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	envlist_free(t_envlist *to_free)
{
	t_envlist	*current_env;
	t_envlist	*temp;

	current_env = to_free;
	while (current_env)
	{
		temp = current_env->next;
		free(current_env->name);
		if (current_env->value)
			free(current_env->value);
		free(current_env);
		current_env = temp;
	}
	if (current_env)
		free(current_env);
}

void	envlist_print_all(t_envlist *envp)
{
	t_envlist	*current_env;

	current_env = envp;
	while (current_env != NULL)
	{
		if (current_env->value)
		{
			printf("%s=", current_env->name);
			if (ft_strncmp(current_env->value, "", 2) == 0)
				printf("\"\"\n");
			else
				printf("\"%s\"\n", current_env->value);
		}
		else
			printf("%s\n", current_env->name);
		current_env = current_env->next;
	}
	if (current_env && current_env->value)
	{
		printf("%s=", current_env->name);
		printf("%s\n", current_env->value);
	}
}

char	*separate_part(char *str, int *i, int *j)
{
	char	*new_str;

	new_str = protected_malloc((*i) + 1, sizeof(char));
	while (*j < *i)
	{
		new_str[*j] = str[*j];
		(*j)++;
	}
	new_str[*j] = '\0';
	return (new_str);
}

char	*env_separation(char *str, int part)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (part == 0)
		return (separate_part(str, &i, &j));
	if (str[i] && part == 1)
	{
		new_str = protected_malloc(ft_strlen(str) - i + 1, sizeof(char));
		while (i < (int)ft_strlen(str))
		{
			new_str[j] = str[i + 1];
			j++;
			i++;
		}
		new_str[j] = '\0';
		return (new_str);
	}
	return (NULL);
}

void	default_envlist(t_envlist **current_envlist)
{
	char		pwd[MAXPATHLEN];

	(*current_envlist)->name = ft_strdup("PWD");
	(*current_envlist)->value = ft_strdup(getcwd(pwd, MAXPATHLEN));
	(*current_envlist)->next = malloc(sizeof(t_envlist));
	(*current_envlist) = (*current_envlist)->next;
	(*current_envlist)->name = ft_strdup("SHLVL");
	(*current_envlist)->value = ft_strdup("1");
	(*current_envlist)->next = malloc(sizeof(t_envlist));
	(*current_envlist) = (*current_envlist)->next;
	(*current_envlist)->name = ft_strdup("_");
	(*current_envlist)->value = ft_strdup("/usr/bin/env");
	(*current_envlist)->next = malloc(sizeof(t_envlist));
	(*current_envlist) = (*current_envlist)->next;
	(*current_envlist)->name = ft_strdup("OLDPWD");
	(*current_envlist)->value = NULL;
	(*current_envlist)->next = NULL;
}
