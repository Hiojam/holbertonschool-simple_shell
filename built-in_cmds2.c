#include "shell.h"

/**
 * cd_cmd - Changes the current directory of the process.
 * @args: Input arguments.
 * Return: void.
*/
void cd_cmd(char **args)
{
	char *home_dir = _getenv("HOME"), *current_dir, *prevDir = NULL;
	char *cmd[] = {"setenv", "PWD", NULL, NULL};

	current_dir = get_current_dir();
	if (!current_dir)
	{
		_perror(custom, "Error: could not get current directory\n");
		return;
	}
	if (!home_dir)
	{
		_perror(custom, "Error: HOME environment variable not set\n");
		free(current_dir);
		return;
	}
	if (!args[1])
	{
		if (chdir(home_dir) != 0)
		{
			_perror(custom, "Error: could not change directory to %s\n", home_dir);
			free(current_dir);
			return;
		}
	}
	else
	{	/* If the argument is "-", change to the previous directory */
		if (strcmp(args[1], "-") == 0)
		{
			if (!prevDir)
			{
				_perror(custom, "Error: no previous directory\n");
				return;
			}
			if (chdir(prevDir) != 0)
			{
				_perror(custom, "Error: could not change directory to %s\n", prevDir);
				free(current_dir);
				return;
			}
		}
		else	/* The argument is a directory */
		{
			if (chdir(args[1]) != 0)
			{
				_perror(custom, "Error: could not change directory to %s\n", args[1]);
				free(current_dir);
				return;
			}
			free(prevDir);			/* free previously saved directory */
			prevDir = strdup(current_dir);
			if (!prevDir)
			{
				_perror(custom, "Error: could not save previous directory\n");
				free(current_dir);
				return;
			}
		}
	}
	cmd[2] = current_dir;
	setenv_cmd(cmd);
	free(current_dir);
	free(home_dir);
}

/**
 * alias_cmd - Define an alias or list them.
 *
 * @args: List of arguments passed to the function.
 * Return: void.
 */
void alias_cmd(char **args)
{
	char *name, *value;
	unsigned int i;

	if (!args[1])
	{
		/* Print all aliases */
		print_aliases();
		return;
	}

	/* Parse arguments */
	for (i = 1; args[i]; i++)
	{
		if (strchr(args[i], '='))
		{
			/* Define an alias */
			name = strtok(args[i], "=");
			value = strtok(NULL, "=");

			if (set_alias(name, value) != 0)
				_perror(custom, "Error: Could not define alias.\n");
		}
		else
		{
			/* Print an alias */
			print_alias(args[i], name, value);
		}
	}
}

/**
 * help_cmd - Define an alias or list them.
 *
 * @args: List of arguments passed to the function.
 * Return: void.
 */
void help_cmd(char **args)
{
	FILE *fp;
	char *file, *extension = ".help";
	char *directory = "./help/";
	char line[1024];

	if (!args[1])
	{
		_perror(cmd, "help (built-in)");
		return;
	}

	file = malloc(strlen(directory) + strlen(args[1]) + strlen(extension) + 1);
	if (!file)
	{
		_perror(mem, strerror(errno));
		return;
	}
	file[0] = '\0';				/* set first byte to zero */
	strcat(file, args[1]);
	strcat(file, extension);

	sprintf(file, "%s%s%s", directory, args[1], extension);
	fp = fopen(file, "r");		/* read-only mode */

	if (!fp)
	{
		_perror(custom, "Error: Unknown command or no help was found.\n");
		free(file);
		return;
	}

	while (fgets(line, 1024, fp))
	{
		printf("%s", line);
	}

	fclose(fp);
	free(file);
}

/**
 * history_cmd - displays the history list, one command by line
 *
 * @args: List of arguments passed to the function.
 * Return: void.
 */
void history_cmd(char **args)
{
	(void) args;

	printHistory();
}
