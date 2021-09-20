#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>

typedef struct s_list t_list;
typedef struct s_vars t_vars;

struct s_list {
	char			**command;
	struct s_list	*next;
};

struct s_vars {
	t_list	list;
	int		cmd_number;
};

void parsing(t_vars *vars, char **argv);

#endif
