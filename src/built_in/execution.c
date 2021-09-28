#include "minishell.h"

void	builtin_exec(t_vars *vars, char **envp) //+ t_token *current_token
{
	char *command;
	int	len_command;
	t_token	*current_token;

	command = vars->first->buffer.str;
	len_command = ft_strlen(command);
	current_token = vars->first;
//printf("cmd len: %d\n", len_command);
	// if (ft_strcmp(command, "cd") == 0)
		// builtin_cd(vars);
	if (ft_strcmp(command, "echo") == 0)
		builtin_echo(vars, current_token);
	else if (ft_strcmp(command, "env") == 0)
		builtin_env(vars);
	else if (ft_strcmp(command, "exit") == 0)
		builtin_exit();
	else if (ft_strcmp(command, "export") == 0)
		builtin_export(vars, current_token);
	else if (ft_strcmp(command, "pwd") == 0)
		builtin_pwd();
	else if (ft_strcmp(command, "unset") == 0)
		builtin_unset(vars, current_token->next);
}

// execution of commands other than builtin commands ---- execve
void	command_exec();


