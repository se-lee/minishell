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
		if (vars->error == 0 && vars->cmd != NULL)
			execute_command(vars);
		free(str);
		loop_free(vars);
	}
	else
		free(str);
}

void	envlist_finish(t_vars *vars)
{
	t_envlist	*current_envlist;

	if (ft_inenv(vars->envp, "PWD") == 0)
		replace_oldpwd_and_pwd(vars, NULL);
	if (ft_inenv(vars->envp, "OLDPWD") == 0)
		add_new_var_to_list(vars, "OLDPWD");
	if (ft_inenv(vars->envp, "SHLVL") == 0)
		add_new_var_to_list(vars, "SHLVL=1");
	if (ft_inenv(vars->envp, "_") == 0)
		add_new_var_to_list(vars, "_=/usr/bin/env");
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
	envlist_finish(&vars);
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
	if (count_heredoc(&vars) > 0)
		unlink(".heredoc");
	ft_putendl_fd("exit", 0);
}
