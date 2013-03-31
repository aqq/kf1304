#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen("result.log", "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);
	bool ok = false;
	while ((read = getline(&line, &len, fp)) != -1) {
		if(line[0]=='\r'&&line[1]=='\n')
		{
			ok = true;
		}
		if(ok)
		printf("%s",line);
	}

	free(line);
	exit(EXIT_SUCCESS);
}

