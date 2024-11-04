#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

void init_lexer(Lexer *lexer, FILE *file)
{
    lexer->file = file;
    lexer->buffer_pos = 0;
    lexer->buffer_size = 0;
}

int fill_buffer(Lexer *lexer)
{
    lexer->buffer_size = fread(lexer->buffer, 1, BUFFER_SIZE, lexer->file);
    lexer->buffer_pos = 0;
    return lexer->buffer_size;
}

int get_next_char(Lexer *lexer)
{
    if (lexer->buffer_pos >= lexer->buffer_size)
    {
        if (fill_buffer(lexer) == 0)
        {
            return EOF;
        }
    }
    return lexer->buffer[lexer->buffer_pos++];
}

void unget_char(Lexer *lexer)
{
    if (lexer->buffer_pos > 0)
    {
        lexer->buffer_pos--;
    }
}

int skip_whitespace(Lexer *lexer)
{
    int ch;
    while ((ch = get_next_char(lexer)) != EOF)
    {
        if (!isspace(ch))
        {
            return ch;
        }
    }
    return EOF;
}

void read_identifier(Lexer *lexer, char *buffer, size_t size)
{
    int ch;
    size_t i = 0;
    while ((ch = get_next_char(lexer)) != EOF && (isalnum(ch) || ch == '_'))
    {
        if (i < size - 1)
        {
            buffer[i++] = ch;
        }
    }
    buffer[i] = '\0';
    if (ch != EOF)
    {
        unget_char(lexer);
    }
}

void read_number(Lexer *lexer, char *buffer, size_t size)
{
    int ch;
    size_t i = 0;
    while ((ch = get_next_char(lexer)) != EOF && isdigit(ch))
    {
        if (i < size - 1)
        {
            buffer[i++] = ch;
        }
    }
    buffer[i] = '\0';
    if (ch != EOF)
    {
        unget_char(lexer);
    }
}

void read_string_literal(Lexer *lexer, char *buffer, size_t size)
{
    int ch;
    size_t i = 0;
    while ((ch = get_next_char(lexer)) != EOF && ch != '"')
    {
        if (i < size - 1)
        {
            buffer[i++] = ch;
        }
    }
    buffer[i] = '\0';
}

Token get_next_token(Lexer *lexer)
{
}