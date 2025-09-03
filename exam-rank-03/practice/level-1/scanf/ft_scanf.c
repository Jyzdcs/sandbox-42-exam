#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>

int	match_space(FILE *f)
{
	// You may insert code here
	return (0);
}

int	match_char(FILE *f, char c)
{
	// You may insert code here
	return (0);
}

int	scan_char(FILE *f, va_list ap)
{
	return (-1);
}

int	scan_int(FILE *f, va_list ap)
{
	int	i;
	int	c;
	int	*ptr;
	int	count;
	int	sign;

	ptr = va_arg(ap, int *);
	count = 0;
	sign = 1;
	i = 0;
	c = fgetc(f);
	if (c == '-')
		sign = -1;
	else if (c == '+')
		sign = 1;
	else if (c == EOF)
		return (-1);
	else
		ungetc(c, f);
	while (isdigit(c = fgetc(f)) && c != EOF)
	{
		printf("\n%d: %c\n", count, c);
		i = i * 10 + (c - '0');
		count++;
	}
	if (count == 0)
		return (0);
	*ptr = i * sign;
	return (1);
}

int	scan_string(FILE *f, va_list ap)
{
	int		c;
	char	*ptr;
	int		read;
	int		i;

	read = 0;
	i = 0;
	ptr = va_arg(ap, char *);
	while ((c = fgetc(f)))
	{
		ptr[i++] = c;
		read++;
	}
	if (c == EOF)
		return (-1);
	if (read == 0)
		return (0);
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

int	main(void)
{
	int		age;
	char	name[50] = {0};
	char	grade;
	int		result;

	grade = '1';
	printf("=== TESTS SIMPLES DE FT_SCANF ===\n\n");
	scanf("%s", name);
	printf("%s\n", name);
	// TEST 1: Un seul entier
	// printf("1. Entrez votre âge: ");
	// result = ft_scanf("%d", &age);
	// printf("   Résultat: %d conversion, âge = %d\n", result, age);
	// printf("2. Entrez votre nom: ");
	// result = ft_scanf("%s", name);
	// printf("   Résultat: %d conversion, nom = '%s'\n", result, name);
	// printf("3. Entrez âge et nom ensemble: ");
	// result = ft_scanf("%d %s", &age, name);
	// printf("Adresse de age: %p\n", &age);
	// printf("Adresse de name: %p\n", name);
	return (0);
}
