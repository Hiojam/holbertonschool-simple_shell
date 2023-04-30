#include "shell.h"

/**
 * clear_cmd - Clear the cmd.
 * @args: Input arguments.
 * Return: void.
*/
void clear_cmd(char **args)
{
	(void) args;
	system("printf '\033[H\033[J'");
}

/**
 * exit_cmd - Built-in to exit the shell.
 * @args: Input arguments.
 * Return: void.
*/
void exit_cmd(char **args)
{
	/*if args[1] is not present, last_status will be set to 0*/
	if (args[1] != NULL)
		last_status = atoi(args[1]);

	/* Free the arguments array */
	if (args)
		free_array(args);

	/* Saves the command history */
	saveHistory();

	/* Free commands array */
	free_commands();

	exit(last_status);
}

/**
 * env_cmd - Prints the environment.
 * @args: Input arguments.
 * Return: void.
*/
void env_cmd(char **args)
{
	unsigned int i = 0;

	(void) args;
	for (; environ[i] != NULL; i++)
	{
		printf("%s\n", environ[i]);
	}
}

/**
 * setenv_cmd - Initialize a new environment variable
 *				or modify an existing one
 * @args: Input arguments.
 * Return: void.
*/
void setenv_cmd(char **args)
{
	extern char **environ;
	char **new_environ;
	char *env_var, *name, *value;
	unsigned int i = 0;

	if (!args[1] || !args[2])
	{
		_perror(cmd, "setenv (var) (value)");
		return;
	}
	name = args[1];
	value = args[2];
	/* Create a string in the format "MY_VAR=my_value" */
	env_var = malloc(strlen(name) + strlen(value) + 2);
	if (env_var == NULL)
	{
		_perror(mem, strerror(errno));
		return;
	}
	sprintf(env_var, "%s=%s", name, value);
	for (; environ[i] != NULL; i++)		/* Loop to find the variable to modify */
	{
		if (strncmp(environ[i], name, strlen(name)) == 0
					&& (environ[i])[strlen(name)] == '=')
		{
			/* Replace the value of the environment variable*/
			environ[i] = env_var;
			return;
		}
	}
	for (; environ[i] != NULL; i++)
		;
	new_environ = malloc(sizeof(char *) * (i + 2));
	if (new_environ == NULL)
	{
		_perror(mem, strerror(errno));
		free(env_var);
		return;
	}
	memcpy(new_environ, environ, sizeof(char *) * i);
	new_environ[i] = env_var;
	new_environ[i + 1] = NULL;
	environ = new_environ;
}

/**
 * unsetenv_cmd - Remove an environment variable
 * @args: Input arguments.
 * Return: void.
*/
void unsetenv_cmd(char **args)
{
	extern char **environ;
	char *name = args[1];
	unsigned int found = 0, j, i = 0;

	if (!name)
	{
		_perror(cmd, "unsetenv (var)");
		return;
	}
	/* Loop through the environment variables to find the variable to remove */
	for (; environ[i] != NULL; i++)
	{
		if (strncmp(environ[i], name, strlen(name)) == 0
					&& environ[i][strlen(name)] == '=')
		{
			/* Left shifting  the env variables, overwriting the variable to remove */
			for (j = i; environ[j] != NULL; j++)
			{
				environ[j] = environ[j + 1];
			}
			found = 1;
			break;
		}
	}
	if (!found)
		_perror(custom, "Error: Variable %s not found\n", name);
}
