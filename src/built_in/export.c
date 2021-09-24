#include "../../include/minishell.h"

/*
export A=123  will add A=123 to env variable

Export environment variables, create new ones and replace old ones
Check them with env

if only "export" without any arguments,
print the list of all exported variables. 

in case of 'export a' (one argument without =VALUE), 
'a' will not be saved into env variable

in case of 'export a=123' (NAME=VALUE),
- if it is a new variable,
	create a new variable
新しい環境変数であれば既存のサイズ＋２の**newを作成
そこに既存の環境変数をDupし、
新たな環境変数とNULLを追加
既存の環境変数をフリー →  **newに入れ替える

*/

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

// check input
// find '=' and check if there is a value after '='

char  **builtin_export(char **envp, char *new_var) // Modify later: struct t_var for getting command & argument
{
	char **new_env;
	int	env_count;
	int	i;

	env_count = count_env(envp);
	new_env = malloc(sizeof(char *) * env_count + 2);
	i = 0;
	while (envp[i])
	{
		new_env[i] = envp[i];
		i++;
	}
	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	return (new_env);
}

// int main(int argc, char **argv, char **envp)
// {
// 	int	i = 0;
// 	char **new;
//
// 	builtin_env(envp);
// 	printf("\n\n-------<<    export     >>---------\n\n");
// 	new = builtin_export(envp, argv[1]);
// 	while (new[i])
// 	{
// 		printf("%s\n", new[i]);
// 		i++;
// 	}
// 	return (0);
// }
