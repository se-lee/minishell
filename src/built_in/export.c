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

// find '=' and check if there is a value after '=', return 1;

static int		var_value_is_valid(char *new_var)
{
	int	i;
	return (i);
}

void	free_element(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static void 	free_and_replace_envp(char **env, char **new_env)
{
	int		i;

	free_element(env);
	i = 0;
printf("new_env count:%d\n", count_env(new_env));
	env = malloc(sizeof(char *) * (count_env(new_env)));
	while (new_env[i])
	{
		env[i] = ft_strdup(new_env[i]);
		i++;
	}
}



void	builtin_export(t_vars *vars, t_token *current_token)
{
	char *new_var;
	char **new_env;
	int	i;

	while (current_token)
	{
		if (current_token->token_type == WORD)
		{
			new_var = current_token->buffer.str;
			break ;
		}
		current_token = current_token->next;
	}
	printf("new_var: %s\n", new_var);
	new_env = malloc(sizeof(char *) * (count_env(vars->envp) + 2));
	i = 0;
	while (vars->envp[i])
	{
		new_env[i] = ft_strdup(vars->envp[i]);
		i++;
	}
	new_env[i] = ft_strdup(new_var);
	new_env[i + 1] = NULL;
	free_and_replace_envp(vars->envp, new_env);
}


