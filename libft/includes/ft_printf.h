#ifndef FT_PRINTF_H
#define FT_PRINTF_H

#include "libft.h"
#include <stdarg.h>
#include <stdint.h>

int ft_printf(const char *str, ...);
int print_ptr(void *ptr);
int    print_dec(int num);
int    print_unsigned(unsigned int num);
int print_hex(unsigned int dec, const char format);

#endif