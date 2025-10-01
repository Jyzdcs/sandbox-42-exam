#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void	ft_putstr_fd2(char *s, char *arg)
{
	int	i;

	i = 0;
	while (s[i])
		write(2, &s[i++], 1);
	i = 0;
	if (arg)
		while (arg[i])
			write(2, &arg[i++], 1);
	write(2, "\n", 1);
}

void	ft_exec(char **av, int input_fd, int i, char **envp)
{
	av[i] = NULL;
	dup2(input_fd, STDIN_FILENO);
	close(input_fd);
	execve(av[0], av, envp);
	ft_putstr_fd2("error cmd ", av[0]);
	exit(EXIT_FAILURE);
}

int	main(int ac, char **av, char **envp)
{
	int i = 0;
	int input_fd = dup(STDIN_FILENO);
	int fd[2];

	(void)ac;
	while (av[i] && av[i + 1])
	{
		av = &av[i + 1];
		i = 0;
		while (av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
			i++;
		if (!strcmp(av[0], "cd"))
		{
			if (i != 2)
				ft_putstr_fd2("Wrong arg num ", NULL);
			else if (chdir(av[1]) != 0)
				ft_putstr_fd2("Wrong path ", av[1]);
		}
		else if (i != 0 && (av[i] == NULL || !strcmp(av[i], ";")))
		{
			if (fork() == 0)
				ft_exec(av, input_fd, i, envp);
			else
			{
				close(input_fd);
				while (waitpid(-1, NULL, WUNTRACED) != -1)
					;
				input_fd = dup(STDIN_FILENO);
			}
		}
		else if (i != 0 && !strcmp(av[i], "|"))
		{
			pipe(fd);
			waitpid(-1, NULL, WUNTRACED);
			if (fork() == 0)
			{
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				ft_exec(av, input_fd, i, envp);
			}
			else
			{
				close(fd[1]);
				close(input_fd);
				input_fd = fd[0];
			}
		}
	}
	close(input_fd);
	return (0);
}