#include <unistd.h>		// exec family
#include <sys/ddi.h>	// strcpy(..)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>		// strchr(..)

extern char** environ;

// str must have type "NAME=VALUE"
char* get_envname(char const* str)
{
	char* separator = strchr(str, '=');
	if (separator == NULL)
	{
		return NULL;
	}
	size_t name_len = separator - str;
	char* name = (char*)malloc(name_len + 1);
	if (name == NULL)
	{
		return NULL;
	}
	memcpy(name, str, name_len);
	name[name_len] = '\0';
	return name;
}

char* get_envval(char const* str)
{
	char* separator = strchr(str, '=');
	if (separator == NULL)
	{
		return NULL;
	}
	size_t str_len = strlen(str);
	size_t val_len = str_len - (separator - str) - 1;
	char* val = (char*)malloc(val_len + 1);
	if (val == NULL)
	{
		return NULL;
	}
	memcpy(val, separator + 1, val_len);
	val[val_len] = '\0';
	return val;
}

void replace_env(char* const* new_env)
{
	for (char** env = environ; *env != NULL; ++env)
	{
		char const* envname = get_envname(*env);
		if (unsetenv(envname) == -1)
		{
			printf("FUCK\n");
		}
		free(envname);
	}
	for (char* const* env = new_env; *env != NULL; ++env)
	{
		char const* envname = get_envname(*env);
		char const* envval = get_envval
	}
}

int main()
{
	replace_env(NULL);
	for (char *const* env = environ; *env != NULL; ++env)
	{
		printf("name:\t%s\tval:\t%s\n", get_envname(*env), get_envval(*env));
		//break;
	}
}
