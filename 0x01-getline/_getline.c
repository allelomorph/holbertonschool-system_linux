#include "_getline.h"

/**
 * copy_line - 
 * @read_buf: 
 * @line_start:
 * @overflow: 
 * Return: 
 */
char *copy_line(char *read_buf, size_t *line_start, bool *overflow)
{
	size_t i, line_end, line_len;
	char *line = NULL, *new_line = NULL;
	static char *unfinished_line;
        static size_t unfinished_line_len;

	if (!read_buf)
		return (NULL);

	/* parse buffer for line starting and ending indicies */
	for (i = *line_start; i < READ_SIZE && read_buf[i] != '\n'; i++)
	{}
	*overflow = (i == READ_SIZE);
	line_end = i;
	line_len = (line_end - *line_start);

        new_line = malloc(sizeof(char) * (line_len + 1));
	if (!new_line)
		return (NULL);
	for (i = *line_start; i < line_end; i++)
		new_line[i - *line_start] = read_buf[i];
        new_line[line_len] = '\0';
	*line_start = *overflow ? 0 : (line_end + 1) % READ_SIZE;

	/* append previous unfinished line */
	if (unfinished_line)
	{
		line = malloc(sizeof(char) * (unfinished_line_len + line_len + 1));
		if (!line)
			return (NULL);
		strcpy(line, unfinished_line);
		line = strcat(line, new_line);
		free(new_line);
		new_line = NULL;
		free(unfinished_line);
		unfinished_line = NULL;
	}
	else
		line = new_line;

	/* store current as unfinished */
	if (*overflow)
	{
	        unfinished_line_len += line_len;
		unfinished_line = line;
	}
	return (line);
}


/**
 * buff_load - 
 * @fd: file descriptor to read from
 * @read_buf:
 * @line_start:
 * Return: 
 */
char *buff_load(int fd, char *read_buf, size_t line_start)
{
	ssize_t bytes_read = 0;
	bool remainder_empty = true;
	size_t i;

	/* attempt to read into buffer, if first call, or last call moved start to beginning of new buffer */
	if (line_start == 0)
	{
		memset(read_buf, '\0', READ_SIZE);
		bytes_read = read(fd, read_buf, READ_SIZE);
		if (bytes_read == -1)
		{
			/* what needs to be freed?
			1st call - nothing
			subsequent call with no overflow - buffer
			inside overflow loop - unfinished_line, buffer */
			return (NULL);
		}
	}

	/* bytes_read == 0 when EOF occurs within current buffer */
	/* thorough check here allows for \0 chars to appear in body of file */
	if (bytes_read == 0)
	{
		for (i = line_start; i < READ_SIZE; i++)
			if (read_buf[i] != '\0')
				remainder_empty = false;

		/* file contents read and no more lines to return */
		if (remainder_empty)
		{
			free(read_buf);
			return (NULL);
		}
	}

	return (read_buf);
}


/**
 * _getline - reads an entire line from a file descriptor
 * @fd: file descriptor to read from
 * Return: pointer to buffer containing read bytes, or NULL on failure or when
 * no more bytes to read
 */
char *_getline(const int fd)
{
	static char *read_buf;
	char *line = NULL;
	static size_t line_start;
	bool overflow = false;

	/* open() returns -1 on error; screening out stdin, stdout, stderr */
	/* need to handle stdin in theory */
	if (fd < 3)
		return (NULL);

	if (!read_buf)
		read_buf = malloc(sizeof(char) * READ_SIZE);
	if (!read_buf)
		return (NULL);

	do {
		read_buf = buff_load(fd, read_buf, line_start);
		line = copy_line(read_buf, &line_start, &overflow);
	} while (overflow && line);

	return (line);
}
