#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1
#endif

char	*ft_strdup(char *src)
{
	int		i;
	char	*str;

	i = 0;
	while (src[i])
		i++;
	str = malloc(sizeof(char) * (i + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (src[i])
	{
		str[i] = src[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*gnl(int fd)
{
	static int	buff_read;
	static int	buff_pos;
	static char	buffer[BUFFER_SIZE];
	int			i;
	char		line[70000];

	i = 0;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while (1)
	{
		if (buff_pos == buff_read)
		{
			buff_read = read(fd, buffer, BUFFER_SIZE);
			buff_pos = 0;
			if (buff_read <= 0)
				break ;
		}
		line[i++] = buffer[buff_pos++];
		if (line[i - 1] == '\n')
			break ;
	}
	line[i] = '\0';
	if (i <= 0)
		return (NULL);
	return (ft_strdup(line));
}

// void	main(int ac, char **av)
// {
// 	int fd = open("test.txt", O_RDONLY);
// 	char *str;
// 	str = gnl(fd);
// 	printf("%s", str);
// 	str = gnl(fd);
// 	printf("%s", str);
// }