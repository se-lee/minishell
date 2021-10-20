#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/param.h>
# include <sys/wait.h>
# include <termios.h>
# include "../libft/libft.h"

# define TRUE	1
# define FALSE	0

# define IN		0
# define OUT	1

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
	char		*name;
	char		*value;
	t_envlist	*next;
};

struct s_command {
	char		**command;
	int			pipe;
	int			quotes; //ADD
	int			redirect_right;
	int			redirect_left;
	t_command	*next;
};

struct s_vars {
	t_token			*first;
	t_envlist		*envp;
	t_command		*cmd;
	int				fd_in[2];
	int				fd_out[2];
	struct termios	saved_termios;
	int				return_value;
	int				error;
};

//Parsing fonctions
int			ft_isspecial(char c);
int			ft_isupper(char c);
int			find_space(char *str);
int			ft_piperedirect(int token_type);
void		add_char(t_token *token, char c);
void		update_token(t_token *token, char *var, char *value);
char		*replace(char *full, char *placeholder, char *real);
t_token		*replace_env(t_vars *vars, t_token *token);
void		tokenization(t_vars *vars, char *str);
void		parsing(t_vars *vars, char *str);
void		ft_comandadd_back(t_token **alst, t_token *new);
char		*remove_quotes(char *original, int token_type);

/* built-in utils */
int			format_is_valid(char *str);
int			ft_inenv(t_envlist *envp, char *str);
int			export_syntax(char *str, int quotes);

/* built-in commands */
void		builtin_cd(t_vars *vars, t_command *current_cmd);
void		builtin_echo(t_vars *vars, t_command *current_cmd);
void		builtin_env(t_vars *vars);
void		builtin_exit(t_command *current_cmd);
void		builtin_export(t_vars *vars, t_command *current_cmd);
void		builtin_unset(t_vars *vars, t_command *current_cmd);
void		builtin_pwd(void);

/* execution */
void		run_command_builtin(t_vars *vars, t_command *current_cmd);
void		run_command_non_builtin(t_envlist *envlist, t_command *current_cmd);
int			envlist_count(t_envlist *envp);
char		**envlist_to_char_array(t_envlist *envp);
void		execute_pipe_commands(t_vars *vars);

/* command utils */
char		*get_command_path(t_envlist *envp, char *command);
int			count_command(t_command *cmd);
int			command_is_builtin(char **command);
char		*get_env_value(t_envlist *envp, char *env_name);
char		**envlist_to_char_array(t_envlist *envp);
void		print_commands(t_command *cmd);
/* envlist utils */
void		envlist_create(t_vars *vars, char **envp);
void		envlist_free(t_envlist *to_free);
char		*env_separation(char *str, int part);
void		envlist_print_all(t_envlist *envp);
t_envlist	*envlist_duplicate(t_envlist *envp);
t_envlist	*envlist_sort_ascii(t_vars *vars);

/* miscellaneous utils */
void		ft_append(char **a, char *str);
void		*protected_malloc(size_t count, size_t size);
void		free_array(char **array);

/* test functions to be removed later */
void		test_function_print_envarr(char **env, t_envlist *envlist);
void		test_func_print_commands(t_command *current_cmd);

#endif
