#include "minishell.h"

void	test_function_print_envarr(char **env, t_envlist *envlist)
{
	int	i = 0;
	printf("\n--------------[CHAR ** ENV]----------------\n");
	while (env[i])
	{
		printf("env[%d]:%s\n", i, env[i]);
		i++;
	}
	printf("\n--------------[LINKED LIST ENV]----------------\n");
	envlist_print_all(envlist);
	printf("\n-----------------------------------------------\n");
}

void	test_func_print_commands(t_command *current_cmd)
{
	while (current_cmd)
	{
		printf("cmd:%s\n", current_cmd->command[0]);
		current_cmd = current_cmd->next;
	}
}
