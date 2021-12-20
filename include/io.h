#ifndef GRAPHRANKER_IO_H
#define GRAPHRANKER_IO_H

#include <stddef.h>
#include <stdint.h>

extern size_t io_flush_stdin(void);
extern size_t io_skip_spaces(void);
extern size_t io_read_line(char *buffer, int size);
extern int io_read_uint(uint32_t *value);
extern int io_read_char(unsigned char c);
extern int io_read_matrix(uint32_t *buffer, uint32_t size);

#endif /* GRAPHRANKER_IO_H */
