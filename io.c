#include "io.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t io_flush_stdin(void)
{
	int c;
	size_t skipped;

	for (skipped = 1; (c = getchar()) != '\n' && c != EOF; skipped++)
		;
	if (c == EOF)
		skipped--;
	return skipped;
}

size_t io_skip_spaces(void)
{
	int c;
	size_t skipped;
	for (skipped = 0; isspace((c = getchar())) && c != EOF; skipped++)
		;
	if (c != EOF)
		ungetc(c, stdin);
	else
		skipped--;
	return skipped;
}

size_t io_read_line(char *buffer, int size)
{
	assert(buffer);
	if (size <= 0 || fgets(buffer, size, stdin) == NULL)
		return 0;

	size_t len = strlen(buffer);
	if (buffer[len - 1] == '\n')
		buffer[len - 1] = '\0';
	else
		len += io_flush_stdin();

	return len;
}

int io_read_uint(uint32_t *value)
{
	assert(value);
	char buffer[11];
	char *ptr = buffer;

	memset(buffer, '\0', 11);

	io_skip_spaces();
	while (1) {
		int c = getchar();
		if (c == EOF || c == '\n') {
			if (ptr == buffer) {
				fputs("Nothing read. Reached EOF.\n", stderr);
				return EOF;
			}
			break;
		}
		if (isdigit(c)) {
			*ptr = (char)c;
			ptr++;
		} else if (ptr == buffer) {
			fputs("Input error: expected number.\n", stderr);
			return -2;
		} else {
			ungetc(c, stdin);
			break;
		}
		if (ptr > &buffer[9]) {
			fputs("Input is too long.\n", stderr);
			return -3;
		}
	}

	unsigned long long result = strtoull(buffer, NULL, 10);
	if (result > UINT32_MAX) {
		fputs("Input overflow: number is too large.\n", stderr);
		return -4;
	}
	*value = (uint32_t)result;
	return 0;
}

int io_read_char(unsigned char c)
{
	int read = getchar();
	if (read == c)
		return 0;
	ungetc(read, stdin);
	return -1;
}

int io_read_matrix(uint32_t *buffer, uint32_t size)
{
	assert(buffer);
	for (uint32_t i = 0; i < size; i++)
		for (uint32_t j = 0; j < size; j++) {
			uint32_t value;
			if (io_read_uint(&value) != 0)
				return -1;
			buffer[i * size + j] = value;
			if (j < size - 1 && io_read_char(',') != 0) {
				fputs("Input error: expected ','.\n", stderr);
				return -1;
			}
		}
	return 0;
}
