#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <termcap.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/includes/libft.h"

typedef enum e_redir_type {
    REDIR_IN,       // <
    REDIR_OUT,      // >
    REDIR_APPEND,   // >>
    HEREDOC         // <<
} t_redir_type;

typedef struct s_redir {
    t_redir_type    type;
    char            *filename;
    struct s_redir  *next;
} t_redir;

typedef struct s_command {
    char        **argv;         // Commande + args ex: {"grep", "hello", NULL}
    t_redir     *redirs;        // Liste chaînée de redirections
    int         pipe_in;        // 1 si reçoit depuis un pipe
    int         pipe_out;       // 1 si envoie vers un pipe
    int         is_builtin;     // 1 si builtin, sinon 0
} t_command;


#endif