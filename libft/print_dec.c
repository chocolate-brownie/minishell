#include "ft_printf.h"

int    print_dec(int num)
{
    char *num_str;
    int len;

    num_str = ft_itoa(num);
    len = ft_strlen(num_str);
    if (num_str)
    {
        ft_putstr_fd(num_str, 1);
        free(num_str);
    }
    return (len);
}