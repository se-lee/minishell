#include "minishell.h"

int main(int argc, char **argv)
{
	t_vars	vars;

	(void)argc;
	parsing(&vars, argv);
	return (0);
}
