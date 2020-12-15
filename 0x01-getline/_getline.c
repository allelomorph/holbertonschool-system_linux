#include "_getline.h"


/**
 * free_profiles - frees singly linked list of fd profiles and associated
 * buffers
 * @head: double pointer to first node in list
 */
void free_profiles(fd_profile_t **head)
{
	fd_profile_t *temp1 = NULL, *temp2 = NULL;

	temp1 = *head;
	while (temp1)
	{
		temp2 = temp1;
		temp2 = temp2->next;

		if (temp1->read_buf)
			free(temp1->read_buf);

		free(temp1);

		temp1 = temp2;
	}

	*head = NULL;
}


/**
 * add_profile - adds new fd profile to tail of singly linked list
 * @head: double pointer to first node in list
 * @fd: fiel descriptor to process
 * Return: pointer to new node, or NULL on failure
 */
fd_profile_t *add_profile(fd_profile_t **head, int fd)
{
	fd_profile_t *new, *temp;

	new = malloc(sizeof(fd_profile_t));
	if (!new)
	{
		free_profiles(head);
		return (NULL);
	}

	/* init new */
	new->fd = fd;
	new->read_buf = malloc(sizeof(char) * READ_SIZE);
	if (!new->read_buf)
	{
		free_profiles(head);
		return (NULL);
	}
	new->line_start = 0;
	new->next = NULL;

	/* add to tail of profile_list */
	if (*head)
	{
		temp = *head;
		while (temp && temp->next)
			temp = temp->next;
		temp->next = new;
	}
	else
		*head = new;

	return (new);
}


char *join_line(char **unfinished_line, char **new_line,
		size_t unfinished_line_len, size_t line_len)
{
	char *line;
	size_t i;

	line = malloc(sizeof(char) * (unfinished_line_len + line_len + 1));
	if (!line)
	{
		free(*new_line);
		*new_line = NULL;
		free(*unfinished_line);
		*unfinished_line = NULL;
		return (NULL);
	}

	/* Using this method rather than strcpy and strcat to allow for */
	/* \0 bytes appearing mid-line. Only \n observed as delimiter. */
	memcpy(line, *unfinished_line, unfinished_line_len);
	for (i = unfinished_line_len;
	     i < (unfinished_line_len + line_len); i++)
	{
		line[i] = (*new_line)[i - unfinished_line_len];
	}
	line[unfinished_line_len + line_len] = '\0';

	free(*new_line);
	*new_line = NULL;
	free(*unfinished_line);
	*unfinished_line = NULL;

	return (line);
}

/**
 * copy_line - copies a \n-delimited string of bytes from a read buffer
 * @read_buf: buffer to copy from
 * @line_start: refernece to int: starting index in buffer to copy from
 * @overflow: refernce to bool: is the current line straddling a buffer edge?
 * Return: full \n-delimited line copied from buffer, or NULL on failure
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

	/* prepend previous unfinished line */
	if (unfinished_line)
		line = join_line(&unfinished_line, &new_line,
				 unfinished_line_len, line_len);
	else
		line = new_line;

	/* if buffer edge met before delimiter, store line as unfinished */
	if (*overflow)
	{
		unfinished_line_len += line_len;
		unfinished_line = line;
	}
	else
		unfinished_line_len = 0;

	return (line);
}


/**
 * buff_load - reads new group of READ_SIZE bytes into buffer when necessary
 * @fd: file descriptor to read from
 * @read_buf: buffer to read into
 * @line_start: starting index in buffer after last delimiter processed
 * Return: pointer to current buffer or NULL on failure or end of file
 * content reached
 */
char *buff_load(int fd, char *read_buf, size_t line_start)
{
	ssize_t bytes_read = 0;
	bool remainder_empty = true;
	size_t i;

	/* read bytes into buffer if first call with this fd, */
	/* or previous call moved line_start to beginning of new buffer */
	if (line_start == 0)
	{
		/* skip init on stdin */
		if (fd != 0)
			memset(read_buf, '\0', READ_SIZE);
		bytes_read = read(fd, read_buf, READ_SIZE);
		if (bytes_read == -1)
			return (NULL);
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
	static fd_profile_t *profile_list;
	fd_profile_t *temp = NULL;
	char *line = NULL;
	bool overflow = false;

	/* open() returns -1 on error; screening out stdin, stdout, stderr */
	/* need to handle stdin in theory */
	if (fd == -1 && profile_list)
	{
		free_profiles(&profile_list);
		return (NULL);
	}

	/* check for existing fd profile */
	temp = profile_list;
	while (temp && temp->fd != fd)
		temp = temp->next;

	/* if fd not found, add new profile to list */
	if (!temp)
		temp = add_profile(&profile_list, fd);
	if (!temp)
		return (NULL);

	do {
		temp->read_buf = buff_load(temp->fd, temp->read_buf,
					   temp->line_start);
		line = copy_line(temp->read_buf,
				 &(temp->line_start), &overflow);
	} while (overflow && line);

	return (line);
}
