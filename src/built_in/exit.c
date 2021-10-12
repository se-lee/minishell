#include "minishell.h"

void	builtin_exit(void)
{
	exit(0);
}
// need to set exit status (echo $?)
// print "exit" only when there is no pipe
// if there is  pipe, don't print "exit"
