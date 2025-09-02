#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Chercher une sous-chaîne dans une chaîne (remplace strstr)
char	*find_substring(char *text, char *search)
{
	int	search_len;
	int	i;

	search_len = strlen(search);
	while (*text)
	{
		i = 0;
		while (i < search_len && text[i] == search[i])
			i++;
		if (i == search_len)
			return (text);
		text++;
	}
	return (NULL);
}

// Traiter et afficher une ligne avec remplacement
void	filter_line(char *line, char *search, int search_len)
{
	char	*pos;
	char	*found;
	int		i;

	pos = line;
	while ((found = find_substring(pos, search)))
	{
		// Afficher caractère par caractère avant l'occurrence
		while (pos < found)
		{
			printf("%c", *pos);
			pos++;
		}
		// Afficher les étoiles
		i = 0;
		while (i++ < search_len)
			printf("*");
		pos = found + search_len;
	}
	// Afficher le reste
	printf("%s", pos);
}

int	main(int ac, char **av)
{
	// Vérifier les arguments
	if (ac != 2 || !av[1][0])
		return (1);

	char buffer[4096];
	int search_len = strlen(av[1]);
	int bytes;

	// Lire et traiter en continu
	while ((bytes = read(0, buffer, sizeof(buffer) - 1)) > 0)
	{
		buffer[bytes] = '\0';
		filter_line(buffer, av[1], search_len);
	}

	if (bytes < 0)
	{
		perror("Error: ");
		return (1);
	}

	return (0);
}