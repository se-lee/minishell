#include "../../include/minishell.h"

int		count_env(char **envp)
{
	int		envp_count;
	int		i;

	i = 0;
	envp_count = 0;
	while (envp[i])
	{
		envp_count++;
		i++;
	}
	return(envp_count);
}

char  **builtin_unset(char **envp, char *var_to_unset) // Modify later: struct t_var for getting command & argument
{
	char **new_env;
	int	env_count;
	int	i;

	env_count = count_env(envp);
	new_env = malloc(sizeof(char *) * env_count);

// check if var_to_unset is in correct form
// check if var_to_unset already exists

	if (getenv(var_to_unset) != NULL)


	i = 0;
	while (envp[i])
	{
		new_env[i] = envp[i];
		i++;
	}
	new_env[i + 1] = NULL;
	return (new_env);
}

int main(int argc, char **argv, char **envp)
{
	int	i = 0;
	char **new;

	builtin_env(envp);
	printf("\n\n-------<<    export     >>---------\n\n");
	new = builtin_unset(envp, argv[1]);
	while (new[i])
	{
		printf("%s\n", new[i]);
		i++;
	}
	return (0);
}

// t_vars vars->first->buffer->str  will give me the command
