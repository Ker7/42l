/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmottus <kmottus@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 09:50:00 by kmottus           #+#    #+#             */
/*   Updated: 2022/06/16 09:50:00 by kmottus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "get_next_line.h"



/* Function returns the lenght of the line OR (-1) should there be no '\n'
 */
static int	ft_linelen(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
//printf(">%c", (char)s[i]);
		//if (s[i] == '\n' || s[i + 1] == '\0')
		if (s[i] == '\n')// || s[i + 1] == '\0')
		{
//			if (s[i + 1] == '\0')
//				return (-1);
			return (i);
		}
		i++;
	}
	return (-1);
}

/* Reading part. Here we malloc the BUFFER_SIZE + 1 (for ending \n)
 * While reading produces an output and no \n is being read we cycle and glue
 * the read bytes onto the variable for current stream or characters - flow.
 */

char	*ft_get_chars(int fd, char *flow)
{
	int		i;
	char	*tmp;

	i = 1;
	tmp = (char *)ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!tmp)
		return (NULL);
	while (i > 0 && ft_linelen(flow) == -1)
	{
		i = read(fd, tmp, BUFFER_SIZE);
//printf(">%s<", tmp);
		if (i < 1 && !tmp[0] && ft_strlen(flow) == 0)	//Removed ! from tmp[0] here
		{
//printf("#(((((");	// <-- this is where we read nothing but have sth in the box
			free(tmp);
			free(flow);
			return (NULL);
		}
		tmp[i] = 0; // Because I calloc this is not needed?
		if (i < 1 && tmp[0])
		{
//printf("# ¤¤ #");	// <-- this is where we read nothing but have sth in the box
			free(tmp);
			return (flow);
		}
		flow = ft_strjoin(flow, tmp);
	}
	free(tmp);
	return (flow);
}

/* Here we take out the excess characters from the flow.
 * First condition refers to a flow containing no \n, so we can strlen() it.
 * For the second we can use our ft_linelen to measure out the part we return.
 */

char	*ft_trim_line(char *flow)
{
	char *line;

//printf("¤¤linelen:%d", ft_linelen(flow));
	if (ft_linelen(flow) == -1)
		line = (ft_substr(flow, 0, ft_strlen(flow)));
	else
		line = (ft_substr(flow, 0, ft_linelen(flow) + 1));
//printf("¤¤line:%s;", line);
	return (line);
}

/* Here we push the pointer one line onwards, that is when there is a new \n
 * @clear:Shouldnt here be an error when the last line does not hold a \n char??
 */
char	*ft_push(char *flow)
{
	char	*next_line;

	if (ft_linelen(flow) == -1)
	{
//printf("pushNoLineEnd");
		free(flow);
		flow = NULL;
		return (NULL);
	}
//printf("\nflw1;%s;", flow);
	next_line = ft_strdup(&flow[ft_linelen(flow) + 1]);
//printf("¤nl¤:%s¤", next_line);
	free(flow);
	return (next_line);
}

/* Main function takes in filedescriptor and returns a line of text ending with
 * a '\n' unless end of file is reached and it does not end with a \n character
 *
 * line	- strings in the working
 * flow	- buffer for containing the characters that might of been read the last
 * 		  time, but not printed out, because the line was already returned. The
 * 		  value is stored as a static variable, for it to remain persistent in
 * 		  between different calls to the get_next_line function.
 *
 * @returns a pointer to the character string containing the one line of text
 */

char	*get_next_line(int fd)
{
	char		*line;
	static char	*flow;

//printf("BUFFER_SIZE:\t%d\n", BUFFER_SIZE);
	
	if (BUFFER_SIZE < 1 || fd < 0 || fd > FD_MAX)
		return (NULL);
	if (!flow)
	{
		flow = ft_calloc(1, sizeof(char));
		//flow = malloc(sizeof(char));
		if (!flow)
			return (NULL);
		flow[0] = 0; //We calloced and therefor dont need it yes?
	}
	else
	{
//printf("~flow~");
	}
//printf("\nflw1:%s;pos:%d;", flow, ft_linelen(flow));
	if (ft_linelen(flow) == -1)
	{
//printf("}noNl;");

//printf("\nflw4:%s;pos:%d;len:%ld;", flow, ft_linelen(flow), ft_strlen(flow));
		flow = ft_get_chars(fd, flow);
//printf("\nflw5:%s;;", flow);
		//if (flow == NULL || ft_strlen(flow) == 0)
		if (flow == NULL)
		{
//printf("155-no-flow");
			return (NULL);
		}
	}
	line = ft_trim_line(flow);
//printf("\nline: %s;", line);
//printf("\nflw2:%s;pos:%d;", flow, ft_linelen(flow));
	flow = ft_push(flow);
//printf("\nflw3:%s;pos:%d;", flow, ft_linelen(flow));
	return (line);
}
