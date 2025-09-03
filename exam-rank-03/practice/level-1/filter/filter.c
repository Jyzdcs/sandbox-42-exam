#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Chercher une sous-chaîne dans une chaîne (remplace strstr)
char	*find_substring(char *text, char *search, int search_len)
{
	int	i;
	int	k;

	i = 0;
	while (text[i])
	{
		k = 0;
		while (text[i] == search[k])
		{
			i++;
			k++;
			if (k == search_len)
				return (&text[i] - k);
		}
		i++;
	}
	return (NULL);
}

// Traiter et afficher une ligne avec remplacement
void	filter_line(char *line, char *search, int search_len)
{
	char	*found;
	char	*pos;
	int		i;

	pos = line;
	while (found = find_substring(pos, search, search_len))
	{
		while (pos < found)
		{
			printf("%c", *pos);
			pos++;
		}
		i = 0;
		while (i++ < search_len)
			printf("*");
		pos = found + search_len;
	}
	printf("%s", pos);
}

int	main(int ac, char **av)
{
	// Vérifier les arguments
	if (ac != 2 || !av[1][0])
		return (1);

	char buffer[4096];
	char *test;
	int search_len = strlen(av[1]);
	int bytes;

	// Lire et traiter en continu
	while ((bytes = read(0, buffer, sizeof(buffer) - 1)) > 0)
	{
		buffer[bytes] = '\0';
		// test = find_substring(buffer, av[1], search_len);
		// printf("buffer : %s\n", buffer);
		// printf("Result : %s\n", test);
		filter_line(buffer, av[1], search_len);
	}

	if (bytes < 0)
	{
		perror("Error: ");
		return (1);
	}

	return (0);
}