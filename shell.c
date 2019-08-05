#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "header.h"
#include <signal.h>
/*
void sigHandler(int sig_num)
{
	signal(SIGINT, sigHandler);
	fflush(stdout);
}
*/


int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
	char **buffer;
	char *get = NULL, *first, *checker, *gcopy, *com = NULL;
	size_t y = 0;
	int x = 0, z, args = 0, i = 1, arg1;
	pid_t child2;
	list *path = getpath();
	list *env = genv();

	signal(SIGINT, sigHandler);


	while (x != -1)
	{
		if (isatty(STDIN_FILENO))
			write(STDIN_FILENO, "divalicious$ ", 13);

		y = 0;
		get = NULL;
		com = NULL;

		x = getline(&get, &y, stdin);
		if (x == -1)
		{
			if (isatty(fileno(stdin)))
				_putchar('\n');

			free(get);
			break;
		}
		get[x - 1] = '\0';
		arg1 = _strn(get);
		if (arg1 == -1 || arg1 == 0)
			continue;
		com = _strtok(get, "#");
		gcopy = _strdup(com);
		args = _strn(com);
		buffer = malloc(sizeof(char *) * (args + 1));
		if (!buffer)
		{
			free(get);
			break;
		}
		first = _strtok(com, " ");

		if (is_a(first) == 1) /*check if its a path name*/
		{
			if (built_in(gcopy, env) == 0)/*check built in*/
			{
				free(buffer);
				free(get);
				free(gcopy);
				continue;
			}
			checker = _which(first, path);

			if (checker != NULL) /*check if its a path*/
				buffer[0] = checker;
			else
			{
				write(STDERR_FILENO, "Command not found\n", 18);
			/*	write(STDERR_FILENO ,"%s: %d: %s: not found\n", argv[0], linecount, first);*/
				free(buffer);
				free(get);
				free(gcopy);
				continue;
			}
		}
		else /*/bin/ls */
		{
			buffer[0] = _strdup(first);
		}
		for (i = 1; i < args ; i++)
			buffer[i] = _strtok(NULL, " ");

		buffer[i] = NULL;

		child2 = fork();

		if (child2 > 0)
		{
			free(buffer[0]);
			free(buffer);
			free(get);
			free(gcopy);
			wait(NULL);
		}
		if (child2 == 0)
		{
			z = execve(buffer[0], buffer, NULL);
			if (z == -1)
			{
				perror("Error: ");
				break;
			}
		}
	}
	free_list(&path);
	free_list(&env);
	return (0);
}
