#ifdef EMBEDDED

#include "../include/embedded.h"

#define UNUSED(x) (void)(x)

// Macros to mock standard library functions for embedded builds

#define MOCK_FUNC0(ret, name)                             \
    ret name(void) __attribute__((warning(#name " is not implemented for embedded builds"))); \
    ret name(void) {                                      \
        __builtin_trap();                                 \
    }

#define MOCK_FUNC1(ret, name, T1)                         \
    ret name(T1 a) __attribute__((warning(#name " is not implemented for embedded builds"))); \
    ret name(T1 a) { UNUSED(a); __builtin_trap(); }

#define MOCK_FUNC2(ret, name, T1, T2)                     \
    ret name(T1 a, T2 b) __attribute__((warning(#name " is not implemented for embedded builds"))); \
    ret name(T1 a, T2 b) { UNUSED(a); UNUSED(b); __builtin_trap(); }

#define MOCK_FUNC3(ret, name, T1, T2, T3)                 \
    ret name(T1 a, T2 b, T3 c) __attribute__((warning(#name " is not implemented for embedded builds"))); \
    ret name(T1 a, T2 b, T3 c) { UNUSED(a); UNUSED(b); UNUSED(c); __builtin_trap(); }

#define MOCK_FUNC4(ret, name, T1, T2, T3, T4)             \
    ret name(T1 a, T2 b, T3 c, T4 d) __attribute__((warning(#name " is not implemented for embedded builds"))); \
    ret name(T1 a, T2 b, T3 c, T4 d) { UNUSED(a); UNUSED(b); UNUSED(c); UNUSED(d); __builtin_trap(); }


// Actual mock implementations for embedded builds

// Memory allocation mockups
MOCK_FUNC1(void *, __embedded_malloc, size_t)
MOCK_FUNC2(void *, __embedded_realloc, void *, size_t)
MOCK_FUNC1(void, __embedded_free, void *)

// File I/O mockups
MOCK_FUNC2(FILE *, __embedded_fopen, const char *, const char *)
MOCK_FUNC1(int, __embedded_fclose, FILE *)
MOCK_FUNC3(int, __embedded_fseek, FILE *, long, int)
MOCK_FUNC1(long, __embedded_ftell, FILE *)
MOCK_FUNC1(void, __embedded_rewind, FILE *)
MOCK_FUNC4(size_t, __embedded_fread, void *, size_t, size_t, FILE *)

// Standard output / diagnostics
MOCK_FUNC1(int, __embedded_printf, const char *)
MOCK_FUNC1(void, __embedded_perror, const char *)
MOCK_FUNC3(int, __embedded_snprintf, char *, size_t, const char *)

// String manipulation mockups
MOCK_FUNC3(char *, __embedded_strncpy, char *, const char *, size_t)
MOCK_FUNC3(char *, __embedded_strncat, char *, const char *, size_t)
MOCK_FUNC1(char *, __embedded_strdup, const char *)
MOCK_FUNC2(int, __embedded_strcmp, const char *, const char *)
MOCK_FUNC3(int, __embedded_strncmp, const char *, const char *, size_t)
MOCK_FUNC2(char *, __embedded_strchr, const char *, int)
MOCK_FUNC2(char *, __embedded_strstr, const char *, const char *)

// Memory copy/move
MOCK_FUNC3(void *, __embedded_memcpy, void *, const void *, size_t)
MOCK_FUNC3(void *, __embedded_memmove, void *, const void *, size_t)

// Exit process
MOCK_FUNC1(void, __embedded_exit, int)

#endif