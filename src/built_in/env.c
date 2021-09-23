#include "../../include/minishell.h"

/* 
print all env variables
*/

void	print_all_env(char **envp)
{
	int	i;

	i = 1;

	while (envp[i] != NULL)
	{
		printf("%s\n", envp[i]);
		i++;
	}
}

// set an environment variable - env NAME=VALUE ??
void	create_env_variable();



void	builtin_env(char **envp)
{
	print_all_env(envp);
}


// int main (int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	print_all_env(envp);
// 	printf("PATH : %s\n", getenv("PATH"));
// 	printf("USER : %s\n", getenv("USER"));
// 	printf("HOME : %s\n", getenv("HOME"));
// 	printf("ROOT : %s\n", getenv("ROOT"));
// 	printf("AAA : %s\n", getenv("AAA"));

//    return(0);
// }

