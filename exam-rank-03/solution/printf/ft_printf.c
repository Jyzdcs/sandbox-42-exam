#include <stdarg.h>
#include <unistd.h>

void	putstr(char *str, int *size)
{
	if (!str)
		str = "(null)";
	while (*str)
	{
		write(1, str, 1);
		*size = *size + 1;
		str++;
	}
}

void	putnbr(long long int nbr, int base, int *size)
{
	char	*digits;

	digits = "0123456789abcdef";
	if (nbr < 0)
	{
		*size = *size + 1;
		write(1, "-", 1);
		nbr = nbr * -1;
	}
	if (nbr >= base)
		putnbr(nbr / base, base, size);
	write(1, &digits[nbr % base], 1);
	*size = *size + 1;
}

int	ft_printf(const char *format, ...)
{
	int		size;
	va_list	pointer;

	size = 0;
	va_start(pointer, format);
	while (*format)
	{
		if (*format == '%' && (*(format + 1) == 's' || *(format + 1) == 'd'
				|| *(format + 1) == 'x'))
		{
			format++;
			if (*format == 's')
				putstr(va_arg(pointer, char *), &size);
			else if (*format == 'd')
				putnbr((long long int)va_arg(pointer, int), 10, &size);
			else if (*format == 'x')
				putnbr((long long int)va_arg(pointer, unsigned int), 16, &size);
		}
		else
		{
			write(1, format, 1);
			size++;
		}
		format++;
	}
	va_end(pointer);
	return (size);
}
