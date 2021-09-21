#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include "../libft/libft.h"

typedef struct s_vars t_vars;
typedef struct s_command t_command;
typedef struct s_token t_token;
typedef struct s_string t_string;

enum TYPE
{
	word,
	quote,
	single_quote,
	space,
	pipe_sign,
	redirect
};

struct s_string {
	char	*str;
	int		len;
};

struct s_token {
	t_string	*buffer;
	enum TYPE	token_type;
};

struct s_command {
	t_token		token;
	t_command	*next;
};

struct s_vars {
	t_command	first;
	int			cmd_number;
};

void parsing(t_vars *vars, char *str);

#endif
