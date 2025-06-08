#ifndef EMBEDDED_H
#define EMBEDDED_H

#ifdef EMBEDDED

#include <stddef.h>
#include <stdio.h>

// Redefine standard library functions to embedded versions
// These macros ensure that any use of standard functions in the code
// will instead invoke the embedded-safe mock implementations
#define malloc(size)         __embedded_malloc(size)
#define realloc(ptr, size)   __embedded_realloc(ptr, size)
#define free(ptr)            __embedded_free(ptr)
#define fopen(filename, mode) __embedded_fopen(filename, mode)
#define fclose(stream)       __embedded_fclose(stream)
#define fseek(stream, offset, whence) __embedded_fseek(stream, offset, whence)
#define ftell(stream)        __embedded_ftell(stream)
#define rewind(stream)       __embedded_rewind(stream)
#define fread(ptr, size, nmemb, stream) __embedded_fread(ptr, size, nmemb, stream)
#define printf(...)          __embedded_printf(__VA_ARGS__)
#define perror(s)            __embedded_perror(s)
#define snprintf(...)        __embedded_snprintf(__VA_ARGS__)
#define strncpy(dest, src, n) __embedded_strncpy(dest, src, n)
#define strncat(dest, src, n) __embedded_strncat(dest, src, n)
#define strdup(s)            __embedded_strdup(s)
#define strcmp(s1, s2)       __embedded_strcmp(s1, s2)
#define strncmp(s1, s2, n)   __embedded_strncmp(s1, s2, n)
#define strchr(s, c)         __embedded_strchr(s, c)
#define strstr(haystack, needle) __embedded_strstr(haystack, needle)
#define memcpy(dest, src, n) __embedded_memcpy(dest, src, n)
#define memmove(dest, src, n) __embedded_memmove(dest, src, n)
#define exit(status)         __embedded_exit(status)

void *__embedded_malloc(size_t size);
void *__embedded_realloc(void *ptr, size_t size);
void __embedded_free(void *ptr);
FILE *__embedded_fopen(const char *filename, const char *mode);
int __embedded_fclose(FILE *stream);
int __embedded_fseek(FILE *stream, long offset, int whence);
long __embedded_ftell(FILE *stream);
void __embedded_rewind(FILE *stream);
size_t __embedded_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
int __embedded_printf(const char *format, ...);
void __embedded_perror(const char *s);
int __embedded_snprintf(char *str, size_t size, const char *format, ...);
char *__embedded_strncpy(char *dest, const char *src, size_t n);
char *__embedded_strncat(char *dest, const char *src, size_t n);
char *__embedded_strdup(const char *s);
int __embedded_strcmp(const char *s1, const char *s2);
int __embedded_strncmp(const char *s1, const char *s2, size_t n);
char *__embedded_strchr(const char *s, int c);
char *__embedded_strstr(const char *haystack, const char *needle);
void *__embedded_memcpy(void *dest, const void *src, size_t n);
void *__embedded_memmove(void *dest, const void *src, size_t n);
void __embedded_exit(int status);

#endif

#endif
