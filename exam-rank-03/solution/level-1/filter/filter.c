#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int	strcmpa(char *s1, char *s2, char len)
{
	int	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i] && i < len)
		i++;
	if (len == i)
		return (0);
	else
		return (1);
}

void	read_buffer(char *buffer, char *str, int len_search)
{
	int		i;
	int		bytes_read;
	char	current_char;
	int		y;

	i = 0;
	bytes_read = 1;
	while ((bytes_read = read(0, &current_char, 1)) > 0)
	{
		buffer[i] = current_char;
		i++;
		if (i >= 10000)
			break ;
	}
	buffer[i] = '\0';
	i = 0;
	y = 0;
	while (buffer[i])
	{
		if (strcmpa(buffer + i, str, len_search) == 0 && i
			+ len_search <= 10000)
		{
			y = 0;
			while (y < len_search)
			{
				write(1, "*", 1);
				y++;
			}
			i += len_search;
		}
		else
		{
			write(1, &buffer[i], 1);
			i++;
		}
	}
}

int	main(int ac, char **av)
{
	char	*buffer;
	int		len;

	if (ac != 2)
		return (1);
	buffer = malloc(sizeof(char) * 10001);
	len = strlen(av[1]);
	if (!buffer)
		return (1);
	read_buffer(buffer, av[1], len);
	free(buffer);
}
