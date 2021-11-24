#include "minishell.h"

char	*vars_initializer(t_vars *vars)
{
	char	*str;

	str = ft_strdup("");
	vars->inout = NULL;
	vars->return_value = 0;
	return (str);
}

void	main_loop(t_vars *vars, char *str)
{
	if (str[0])
	{
		add_history(str);
		parsing(vars, str);
		if (count_heredoc(vars) > 0)
			update_heredoc(vars);
		if (vars->cmd == NULL && vars->inout)
			vars->return_value = redirect_without_cmd(vars);
		if (vars->error == 0 && vars->cmd != NULL)
			execute_command(vars);
		free(str);
		loop_free(vars);
	}
	else
		free(str);
}

int	main(int argc, char **argv, char **envp)
{
	t_vars			vars;
	char			*str;

	(void)argc;
	(void)argv;
	g_vars = &vars;
	str = vars_initializer(&vars);
	envlist_create(&vars, envp);
	tcgetattr(0, &vars.saved_termios);
	while (str != NULL)
	{
		vars.error = 0;
		vars.cmd = NULL;
		main_loop(&vars, str);
		set_termios();
		signal(SIGINT, control_c);
		str = readline("\x1B[32mminishell$\x1B[0m: ");
	}
	ft_putendl_fd("exit", 0);
	exit(vars.return_value);
}
