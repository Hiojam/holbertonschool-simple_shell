#include "shell.h"

/**
 * get_current_dir - Get the current working directory.
 *
 * Return: A pointer to the current directory string.
*/
char *get_current_dir(void)
{
	char *dir;
	size_t size = 1024;

	dir = malloc(size * sizeof(char));
	if (!dir)
	{
		_perror(mem, strerror(errno));
		exit(1);
	}

	while (getcwd(dir, size) == NULL)
	{
		/* If the buffer is too small, double its size and try again */
		if (errno == ERANGE)
		{
			size *= 2;
			dir = realloc(dir, size * sizeof(char));
			if (!dir)
			{
				_perror(mem, strerror(errno));
				exit(1);
			}
		}
		else
		{
			_perror(cwd, strerror(errno));
			exit(1);
		}
	}

	return (dir);
}

/**
 * free_current_dir - Frees the memory allocated by get_current_dir.
 * @dir: A pointer to the current directory
 *
 */
void free_current_dir(char *dir)
{
	if (dir)
		free(dir);
}

/**
 * _getenv - Returns the value of an env var or NULL if it doesn't exists.
 *
 * @name: Name of the environment variable.
 * Return: env var or NULL.
*/
char *_getenv(const char *name)
{
	unsigned int i = 0;
	char **str;
	char *val;

	if (environ == NULL)
		return (NULL);

	for (; environ[i] != NULL; i++)
	{
		str = split_str(environ[i], "=");

		if (!str)
			return (NULL);

		if (strcmp(name, str[0]) == 0)
		{
			val = strdup(str[1]);

			if (!val)
			{
				free_array(str);
				return (NULL);
			}

			free_array(str);
			return (val);
		}

		free_array(str);
	}

	return (NULL);
}
