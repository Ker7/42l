#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "get_next_line.h"

char	*get_next_line(int fd);

int main(int argc, char **argv)
{
	int		i;
	char	*path;
	FILE	*file;
	char	*curline;
	int		_last_line_flag;	// Take some extra scans 

	_last_line_flag = 2;
	i = 0;
	if  (argc < 1)
		return (EXIT_FAILURE);
	path = argv[1];
	file = fopen(path, "r");
	if (!file)
	{
		perror(path);
		return (EXIT_FAILURE);		
	}
//	printf("\nBUFFER_SIZE=%d\n", BUFFER_SIZE); // This is different variable //
	printf("\nThe file descriptor of %s is: %d.\n", path, fileno(file));

	curline = malloc(1000);
	curline = get_next_line(fileno(file));
	while (_last_line_flag > 0)
	{
		printf("%d'th run: \"%s\"", i, curline);
		free(curline);
		curline = malloc(1000);
		curline = get_next_line(fileno(file));
		i++;
		if (curline == NULL)
		{
			_last_line_flag--;
		}
	}	
	free(curline);

	if (fclose(file))
	{
		return (EXIT_FAILURE);
		perror(path);
	}

	return (0);
}
