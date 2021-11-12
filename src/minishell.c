#include "minishell.h"

char	*vars_initializer(t_vars *vars)
{
	char	*str;

	str = ft_strdup("");
	vars->in = NULL;
	vars->out = NULL;
	vars->return_value = 0;
	return (str);
}

int	main(int argc, char **argv, char **envp)
{
	t_vars			vars;
	char			*str;

	(void)argc;
	(void)argv;
	str = vars_initializer(&vars);
	envlist_create(&vars, envp);
	tcgetattr(0, &vars.saved_termios);
	while (str != NULL)
	{
		vars.error = 0;
		if (str[0])
		{
			add_history(str);
			parsing(&vars, str);
			if (vars.error == 0 && vars.cmd != NULL)
				execute_command(&vars);
			free(str);
			loop_free(&vars);
		}
		else
			free(str);
		set_termios();
		signal(SIGINT, control_c);
		str = readline("\x1B[32mminishell$\x1B[0m: ");
	}
	printf("exit\n");//ft_putendl_fd("exit", 0);
}

/*
in var_initializer:
str = ""; --(changed to) -->	str = ft_strdup("");

and added
else free(str);
in main (line 36)
*/

