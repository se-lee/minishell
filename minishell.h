#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>

typedef s_list t_list;
typedef s_vars t_vars;

typedef struct s_list {
	char			**command;
	struct s_list	*next;
};

typedef struct s_vars {
	t_list	list;
	int		cmd_number;
};

void parsing(t_vars *vars, char **argv);

#endif
