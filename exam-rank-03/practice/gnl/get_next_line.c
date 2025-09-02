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
	return (0);
}

int	match_char(FILE *f, char c)
{
	int	r;

	r = fgetc(f);
	if (r == EOF)
		return (-1);
	if (r == c)
		return (1);
	else
	{
		ungetc(r, f);
		return (0);
	}
}

int	scan_char(FILE *f, va_list ap)
{
	char	*ptr;
	int		c;

	ptr = va_arg(ap, char *);
	c = fgetc(f);
	if (c != EOF)
	{
		*ptr = c;
		return (1);
	}
	return (-1);
}

int	scan_int(FILE *f, va_list ap)
{
	int	c;
	int	sign;
	int	i;
	int	digit;
	int	*ptr;

	sign = 1;
	i = 0;
	digit = 0;
	ptr = va_arg(ap, int *);
	if ((c = fgetc(f)) == '-')
		sign = -1;
	else if (c == '+')
	{
		sign = 1;
	}
	else
	{
		if (c == EOF)
			return (-1);
		ungetc(c, f);
	}
	while (isdigit(c = fgetc(f)) && c != EOF)
	{
		i = i * 10 + (c - 48);
		digit++;
	}
	if (!digit)
		return (0);
	if (c != EOF)
		ungetc(c, f);
	*ptr = i * sign;
	return (1);
}

int	scan_string(FILE *f, va_list ap)
{
	int		readed;
	int		i;
	char	*ptr;
	int		c;

	readed = 0;
	i = 0;
	ptr = va_arg(ap, char *);
	while ((c = fgetc(f)) != EOF && !isspace(c))
	{
		ptr[i++] = c;
		readed++;
	}
	if (!readed)
		return (0);
	if (c != EOF)
		ungetc(c, f);
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
	va_list ap;
	va_start(ap, format);
	int ret = ft_vfscanf(stdin, format, ap);
	va_end(ap);
	return (ret);
}