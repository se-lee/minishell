#include "minishell.h"

void	builtin_env(t_vars *vars)
{
	envlist_print_all(vars->envp);
}
