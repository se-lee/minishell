#include "minishell.h"

void	create_envlist(t_vars *vars, char **envp)
{
	int	i;
	t_envlist	*current_envlist;

	vars->envp = malloc(sizeof(t_envlist));
	current_envlist = vars->envp;
	current_envlist->str = ft_strdup(envp[0]);
	current_envlist->next = NULL;
	i = 1;
	while (envp[i])
	{
		current_envlist->next = malloc(sizeof(t_envlist));
		current_envlist = current_envlist->next;
		current_envlist->str = ft_strdup(envp[i]);
printf("[%d]%s\n", i, current_envlist->str);
		current_envlist->next = NULL;
		i++;
	}
}



