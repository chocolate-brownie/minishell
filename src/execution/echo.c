#include "minishell.h"

void    ft_putstr_fd(char *s, int fd)
{
        int     i;

        i = 0;
        while (s[i])
        {
                write(fd, &s[i], 1);
                i++;
        }
}

void    echo(char **args)
{
    int i;

    i = 1;
    while (args[i])
    {
        ft_putstr_fd(args[i], 1);
        if (args[i + 1])
            ft_putstr_fd(" ", 1);
        i++;
    }
    write(1, "\n", 1);  
}

int main()
{
    char *line;

    while (1)
    {
        line = readline("minishell>");
        if (!line)
        {
            printf("exit\n");
            break;
        }
        if (*line)
            add_history(line);
        char **args = ft_split(line, ' ');
        if (args && args[0] && (strcmp(args[0], "echo") == 0))
            echo(args);
        free(line);
    }
    return (0);
}