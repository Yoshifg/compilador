#ifndef READ_H
#define READ_H

#include <string>

FILE *open_file(const char *filename, const char *mode);

FILE *close_file(FILE *file);



#endif // READ_H