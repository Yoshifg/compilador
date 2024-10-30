#include <stdio.h>
#include <stdlib.h>
#include "read.h"

FILE *open_file(const char *filename, const char *mode)
{
    FILE *file = fopen(filename, mode);
    if (!file)
    {
        fprintf(stderr, "Failed to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    return file;
}

void close_file(FILE *file)
{
    if (fclose(file) == EOF)
    {
        fprintf(stderr, "Failed to close file\n");
        exit(EXIT_FAILURE);
    }
}

int read_block(FILE *file, size_t block_size, char *buffer, size_t size)
{
    size_t bytes_read = fread(buffer, 1, block_size, file);
    if (ferror(file))
    {
        fprintf(stderr, "Failed to read file\n");
        exit(EXIT_FAILURE);
    }

    return bytes_read;
}

int is_eof(FILE *file)
{
    return feof(file);
}