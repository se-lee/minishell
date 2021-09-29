#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/param.h>
# include <termios.h>
# include "../libft/libft.h"

# define TRUE	1
# define FALSE	0

typedef struct s_vars		t_vars;
typedef struct s_token		t_token;
typedef struct s_string		t_string;
typedef struct s_envlist	t_envlist;
typedef struct s_command	t_command;
typedef enum e_type			t_type;

enum e_type
{
	WORD,
	QUOTE,
	SINGLE_QUOTE,
	PIPE_SIGN,
	REDIRECT_RIGHT,
	REDIRECT_LEFT
};

struct s_string {
	char	*str;
	int		len;
};

struct s_token {
	t_string	buffer;
	t_type		token_type;
	int			quote_nb;
	t_token		*next;
};

struct s_envlist {
	char		*str;
	t_envlist	*next;
};

struct s_command {
	char		**command;
	int			len_cmd;
	int			pipe;
	int			redirect_right;
	int			redirect_left;
	t_command	*next;
};

struct s_vars {
	t_token			*first;
	t_envlist		*envp;
	t_command		*cmd;
	struct	termios	t;
	struct	termios	not_t;
	int				return_value;
	int				error;
};

//Parsing fonctions
int		ft_isspecial(char c);
int		ft_isupper(char c);
int		ft_piperedirect(int token_type);
void	*protected_malloc(size_t count, size_t size);
void	add_char(t_token *token, char c);
void	update_token(t_token *token, char *var, char *value);
char	*replace(char *full, char *placeholder, char *real);
void	replace_env(t_token *token);
void	tokenization(t_vars *vars, char *str);
void	parsing(t_vars *vars, char *str);
void	ft_comandadd_back(t_token **alst, t_token *new);

/* built-in utils */
int		count_env(char **envp);
void	print_all_env(t_envlist *envp);
void	create_envlist(t_vars *vars, char **envp);

/* built-in commands */
void	builtin_cd(t_vars *vars);
void	builtin_echo(t_vars *vars, t_token *current_token);
void	builtin_env(t_vars *vars);
void	builtin_exec(t_vars *vars, char **envp);
void	builtin_exit(void);
void	builtin_export(t_vars *vars, t_token *current_token);
void	builtin_unset(t_vars *vars, t_token *current_token);
void	builtin_pwd(void);

#endif
