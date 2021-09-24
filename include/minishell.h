#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include "../libft/libft.h"

typedef struct s_list t_list;
typedef struct s_vars t_vars;

/* built-in commands */
void	builtin_env(char **envp);
void	builtin_pwd(void);

#endif
