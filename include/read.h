#ifndef READ_H
#define READ_H

FILE *open_file(const char *filename, const char *mode);

void close_file(FILE *file);

int read_block(FILE *file, size_t block_size, char *buffer, size_t size);

int is_eof(FILE *file);

#endif // READ_H