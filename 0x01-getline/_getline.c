#include "_getline.h"

/**
 * _getline - reads an entire line from a file descriptor
 * @fd: file descriptor to read from
 * Return: pointer to buffer containing read bytes, or NULL on failure or when
 * no more bytes to read
 */
char *_getline(const int fd)
{
	static char read_buf[READ_SIZE];
	char *line = NULL;
	ssize_t bytes_read = 0;
	static size_t line_start, line_end;
	static bool EOF_reached;
	size_t i, line_len;

	/* open() returns -1 on error; screening out stdin, stdout, stderr */
	if (fd < 3)
		return (NULL);
	/* attempt to read into buffer */
	if (!EOF_reached)
	{
		bytes_read = read(fd, read_buf, READ_SIZE);
		if (bytes_read == -1)
			return (NULL);
		if (bytes_read == 0)
			EOF_reached = true;
	}
	/* file contents read and no more lines to return */
	else if (read_buf[line_start] == '\0')
		return (NULL);
	/* parse buffer for line stating and ending indicies */
	for (i = line_start; read_buf[i] != '\n'; i++)
	{}
	line_end = i;
	line_len = (line_end - line_start);
	/* prepare destination string */
	line = malloc(sizeof(char) * (line_len + 1));
	if (!line)
		return (NULL);
	/* copy line */
	for (i = line_start; i < line_end; i++)
		line[i - line_start] = read_buf[i];
	line[line_len] = '\0';
	/* set next starting point after last newline char */
	line_start = line_end + 1;
	return (line);
}
