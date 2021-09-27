#include "../../include/minishell.h"

static void	print_all_env(char **envp)
{
	int	i;

	i = 1;

	while (envp[i] != NULL)
	{
		printf("[%d]%s\n", i, envp[i]);
		i++;
	}
}

static void	delete_env_var(char **envp, int i)
{
	while (envp[i])
	{
		envp[i] = envp[i + 1];
		i++;
	}
}

static char *join_all_var_str(char *var_to_unset);

void	builtin_unset(t_vars *vars, t_token *current_token)
{
	char *var_str;
	char *temp;
	char *var_to_unset;
	int	i;
	
	while (current_token)
	{
		if (current_token->token_type == WORD)
		{
			var_to_unset = current_token->buffer.str;
			break ;
		}
		current_token = current_token->next;
	}
// printf("var_to_unset: %s\n", var_to_unset);
	if (getenv(var_to_unset) == NULL)
		return ;
	temp = ft_strjoin(var_to_unset, "=");
	var_str = ft_strjoin(temp, getenv(var_to_unset));
	free(temp);
	i = 0;
	while (vars->envp[i])
	{
		if ((ft_strcmp(vars->envp[i], var_str) == 0))
			delete_env_var(vars->envp, i);
		i++;
	}
}

/*
int main(int argc, char **argv, char **envp)
{
	int	i = 0;
	char **new;
	// print_all_env(envp);
	// printf("\n\n-------<<    unset     >>---------\n\n");
	builtin_unset(envp, argv[1]);
	// print_all_env(envp);
	return (0);
}
*/