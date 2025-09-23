#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>

int	match_space(FILE *f)
{
	int	c;

	while (isspace(c = fgetc(f)) && c != EOF)
		;
	if (c == EOF)
		return (-1);
	ungetc(c, f);
	return (1);
}

int	match_char(FILE *f, char c)
{
	int	input;

	input = fgetc(f);
	if (input == c)
		return (1);
	else if (input == EOF)
		return (-1);
	return (0);
}

int	scan_char(FILE *f, va_list ap)
{
	int		c;
	char	*ptr;

	c = fgetc(f);
	if (c != EOF)
	{
		ptr = va_arg(ap, char *);
		*ptr = c;
		return (1);
	}
	return (-1);
}

int	scan_int(FILE *f, va_list ap)
{
	int	nb;
	int	c;
	int	*ptr;
	int	sign;
	int	i;

	nb = 0;
	sign = 1;
	i = 0;
	if ((c = fgetc(f)) == '-')
		sign = -1;
	else if (c == '+')
		sign = 1;
	else if (c == EOF)
		return (-1);
	else if (!isdigit(c))
		return (0);
	else
		ungetc(c, f);
	ptr = va_arg(ap, int *);
	while (isdigit(c = fgetc(f)) && c != EOF)
	{
		nb = nb * 10 + (c - '0');
		i++;
	}
	if (c != EOF)
		ungetc(c, f);
	*ptr = nb * sign;
	printf("RETURNED\n");
	return (1);
}

int	scan_string(FILE *f, va_list ap)
{
	char	*ptr;
	int		c;
	int		i;

	i = 0;
	if ((c = fgetc(f)) == EOF)
		return (-1);
	ptr = va_arg(ap, char *);
	ungetc(c, f);
	while ((c = fgetc(f)) != EOF && !isspace(c))
		ptr[i++] = c;
	ptr[i] = '\0';
	return (1);
}

int	match_conv(FILE *f, const char **format, va_list ap)
{
	switch (**format)
	{
	case 'c':
		return (scan_char(f, ap));
	case 'd':
		match_space(f);
		return (scan_int(f, ap));
	case 's':
		match_space(f);
		return (scan_string(f, ap));
	case EOF:
		return (-1);
	default:
		return (-1);
	}
}

int	ft_vfscanf(FILE *f, const char *format, va_list ap)
{
	int	nconv;
	int	c;

	nconv = 0;
	c = fgetc(f);
	if (c == EOF)
		return (EOF);
	ungetc(c, f);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (match_conv(f, &format, ap) != 1)
				break ;
			else
				nconv++;
		}
		else if (isspace(*format))
		{
			if (match_space(f) == -1)
				break ;
		}
		else if (match_char(f, *format) != 1)
			break ;
		format++;
	}
	if (ferror(f))
		return (EOF);
	return (nconv);
}

int	ft_scanf(const char *format, ...)
{
	int		ret;
	va_list	ap;

	va_start(ap, format);
	ret = ft_vfscanf(stdin, format, ap);
	va_end(ap);
	return (ret);
}

int	main(int ac, char **av)
{
	char c;
	int nb_1 = 0;
	int nb_2 = 0;
	char str[100];

	// ft_scanf("%s %d", str, &nb);
	// printf("%s %d", str, nb);
	// ft_scanf("%d %d", &nb_1, &nb_2);
	ft_scanf("%d %d", &nb_1, &nb_2);
	printf("%d", nb_1);
	// printf("%d %d", nb_1, nb_2);
}